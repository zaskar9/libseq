#!/usr/bin/env python3
"""syncexp.py VER SONG SPEC...: one external-sync experiment with the probe on one TTSSEQ version.

The song (from C:\\PROBE\\ORC) is loaded as sequence 1 with clock source 1 (src:N for another),
set before SeqSetPlayingSeq. SPEC items, in any order:
  T:BYTES        a message for the fake MIDI input at T ms after the engine connects (bytes in hex,
                 joined with dots: 3000:fa, 1500:f2.08.00)
  clk:A-B/P      clocks (F8) from A ms up to B ms, every P ms
  src:N          the clock source
  play / stop    @play / @stop;  now  SeqGetNow(1), with a marker message in the MIDI log
  idle:MS / sleep:MS / exp (SeqExpired) / p:LINE (a probe line; _ for spaces)
Prints the probe's results, then one line of what happened in time: input (IN-..), Now samples
(at their marker's time), notes on, program changes and All Notes Off. Callbacks run at interrupt
time (WINE_WIN16_IRQ_CALLBACKS=serial; IRQ=1 for the plain mode, NOIRQ=1 for none).
Example (Song Position, then Continue and clocks):
  syncexp.py 300 QUANT 2000:f2.10.00 3000:fb clk:3200-3450/50 3600:fc idle:1500 play now idle:500 now
"""
import os, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__))
work = os.environ.get('TTSPROBE_WORK', os.path.expanduser('~/ttsprobe-work'))
S = os.path.join(work, 'sync'); os.makedirs(S, exist_ok=True)
ver, song, spec = sys.argv[1], sys.argv[2], sys.argv[3:]
ev, steps, src = [], [], '0001'
for s in spec:
    k, _, v = s.partition(':')
    if k == 'clk':
        a, b = v.split('/')[0].split('-'); p = float(v.split('/')[1]); t = float(a)
        while t < float(b) - 0.01: ev.append((t, 'f8')); t += p
    elif k.replace('.', '').isdigit(): ev.append((float(k), v.replace('.', ' ')))
    elif k == 'src': src = '%04x' % int(v)
    else: steps.append((k, v))
ev.sort(key=lambda e: e[0])
open(f'{S}/in.txt', 'w').write(''.join(f'{t:.1f} {e}\n' for t, e in ev))
L = ['@open', f'@load {song}.WRK', f'?101 call.w SEQSETSRCCLOCK {src}', f'?!101 call.w SEQSETSRCCLOCK 0001 {src}',
     'call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000']
m = 0
for k, v in steps:
    if k == 'now': L += ['call.d SEQGETNOW 0001', 'call.w SEQTXMIDIMSG 0000 000f 00b0 0055 %04x' % m]; m += 1
    elif k in ('idle', 'sleep'): L.append(f'{k} {v}')
    elif k in ('play', 'stop'): L.append('@' + k)
    elif k == 'exp': L.append('call.w SEQEXPIRED')
    elif k == 'p': L.append(v.replace('_', ' '))
L += ['@stop', '@close']
open(f'{S}/t.tpl', 'w').write('\n'.join(L) + '\n')
env = dict(os.environ, TTSPROBE_WORK=work, FAKELOG=f'{S}/out.mid', FAKESEQ_INPUT=f'{S}/in.txt')
if not os.environ.get('NOIRQ'): env['WINE_WIN16_IRQ_CALLBACKS'] = os.environ.get('IRQ', 'serial')
open(f'{S}/out.mid', 'w').close()
out = subprocess.run([os.path.join(here, '..', 'oracles', 'try.sh'), ver, '30'], stdin=open(f'{S}/t.tpl'),
                     capture_output=True, text=True, env=env).stdout
res = [x for x in out.splitlines() if not x.startswith(('load', 'SEQWIRE', 'SEQOPEN', 'SEQLOAD', 'SEQSETPLAYINGSEQ',
                                                         'SEQSETSTANDBY', 'SEQTXMIDI', 'SEQSETSRCCLOCK', 'SEQCLOSE'))]
nows = [int(x.split()[-1], 16) for x in res if x.startswith('SEQGETNOW')]
print(' | '.join(x if not x.startswith('SEQGETNOW') else 'Now' for x in res))
line = []
for x in open(f'{S}/out.mid'):
    p = x.split()
    if len(p) < 3: continue
    t = float(p[0])
    if p[1] == 'in' and p[2] not in ('f8', 'f1'): line.append(f'{t:.0f}:IN-{".".join(p[2:])}')
    elif p[1] == 'short' and p[2] == 'bf' and p[3] == '55':
        i = int(p[4], 16); line.append(f'{t:.0f}:Now={nows[i] if i < len(nows) else "?"}')
    elif p[1] == 'short' and p[2][0] == '9' and not (len(p) > 4 and p[4] == '00'): line.append(f'{t:.0f}:{".".join(p[2:4])}')
    elif p[1] == 'short' and p[2][0] == 'c': line.append(f'{t:.0f}:{".".join(p[2:4])}')
    elif p[1] == 'short' and p[2] == 'b0' and p[3] == '7b': line.append(f'{t:.0f}:AllNotesOff')
print('  '.join(line))
