#!/usr/bin/env python3
"""mtcexp.py VER SONG [OPTIONS]: one MIDI Time Code experiment with the probe on one TTSSEQ version.

The song (from C:\\PROBE\\ORC) is loaded as sequence 1 with clock source 3 (time code), the
sequence's frame rate and SMPTE offset are set as asked, and the engine is started with a callback;
the fake input sends quarter frames (F1). Prints the probe's output (the start callback's words,
Now after the start and later) and the notes the engine played with their times.
Options:
  --code H:M:S:F    the time code the input starts with (default 0:0:10:0)
  --rate R          the rate the code says it is (24, 25, 29 = 30 drop frame, 30; default 30)
  --speed X         the code runs X times as fast as its rate says (default 1)
  --from MS --to MS when the code runs, in ms after the engine connects (default 1500 to 6000)
  --fps N           SeqSetFPS(1, N) before the start (24, 25, 29, 30)
  --ofs H:M:S:F     SeqSetFrameOfs(1, frames), the frames from SeqHMSF2Frame(1, h, m, s, f, 0)
  --late            set the rate and offset after SeqSetPlayingSeq instead of before
  --hold MS         how long to let it play after the start call returns (default 1500)
  --step MS         read Now every MS during that time (default once, at the end)
  --then FROM-TO@H:M:S:F[xSPEED]   another stretch of code (a jump, a restart; repeatable)
  --ini KEY=VALUE   an [Options] line for TTSSEQ.INI during the run (repeatable)
  --pre LINE        a probe line after loading the song (repeatable)
Callbacks run at interrupt time (WINE_WIN16_IRQ_CALLBACKS=serial). Results: $TTSPROBE_WORK/mtc/.
"""
import os, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__))
work = os.environ.get('TTSPROBE_WORK', os.path.expanduser('~/ttsprobe-work'))
S = os.path.join(work, 'mtc'); os.makedirs(S, exist_ok=True)
sys.path.insert(0, os.path.join(here, '..', 'oracles'))
from gen import qframes  # noqa: E402
a = sys.argv[1:]
ver, song = a[0], a[1]
opt = {'code': '0:0:10:0', 'rate': '30', 'speed': '1', 'from': '1500', 'to': '6000', 'hold': '1500', 'step': '0'}
flags, segs, ini, pre = set(), [], [], []
i = 2
while i < len(a):
    k = a[i][2:]
    if k == 'late': flags.add(k); i += 1
    elif k == 'then': segs.append(a[i + 1]); i += 2
    elif k == 'ini': ini.append(a[i + 1]); i += 2
    elif k == 'pre': pre.append(a[i + 1]); i += 2
    else: opt[k] = a[i + 1]; i += 2
rate = int(opt['rate'])
code = tuple(int(x) for x in opt['code'].split(':'))
L = qframes(float(opt['from']), float(opt['to']) - float(opt['from']), rate, *code, speed=float(opt['speed']))
for seg in segs:  # FROM-TO@H:M:S:F[xSPEED]
    span, _, rest = seg.partition('@')
    t0, t1 = (float(x) for x in span.split('-'))
    hmsf, _, sp = rest.partition('x')
    L += qframes(t0, t1 - t0, rate, *(int(x) for x in hmsf.split(':')), speed=float(sp or 1))
L.sort(key=lambda x: float(x.split()[0]))
open(f'{S}/in.txt', 'w').write('\n'.join(L) + '\n')

T = ['@open', f'@load {song}.WRK', 'buf 1 16', 'buf 2 16', 'call.w SEQGETFPS 0001', 'call.d SEQGETFRAMEOFS 0001', '?101 call.w SEQSETSRCCLOCK 0003',
     '?!101 call.w SEQSETSRCCLOCK 0001 0003']
setup = []
if 'fps' in opt: setup.append(f'call.w SEQSETFPS 0001 {int(opt["fps"]):04x}')
if 'ofs' in opt:
    oh, om, os_, of = (int(x) for x in opt['ofs'].split(':'))
    setup += [f'call.d SEQHMSF2FRAME 0001 {oh:04x} {om:04x} {os_:04x} {of:04x} 0000', 'let 3',
              'call.w SEQSETFRAMEOFS 0001 %d3', 'call.d SEQGETFRAMEOFS 0001']
if setup: setup += ['call.w SEQGETFPS 0001']
T += pre
T += [] if 'late' in flags else setup
T += ['call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000']
T += setup if 'late' in flags else []
T += ['cbret 0', 'cbyield 1', '?101,210 call.w SEQPLAYSTART cb3', '?300 call.w SEQSTART cb3 @2', '?300 dump 2 0 4',
      'call.d SEQGETNOW 0001', 'call.w SEQTXMIDIMSG 0000 000f 00b0 0055 0000']
step, hold = int(opt['step']), int(opt['hold'])
for n in range(1, (hold // step if step else 1) + 1):
    T += [f'idle {step if step else hold}', 'call.d SEQGETNOW 0001', f'call.w SEQTXMIDIMSG 0000 000f 00b0 0055 {n:04x}']
T += ['call.w SEQEXPIRED', '@stop', '@close']
open(f'{S}/t.tpl', 'w').write('\n'.join(T) + '\n')
env = dict(os.environ, TTSPROBE_WORK=work, FAKELOG=f'{S}/out.mid', FAKESEQ_INPUT=f'{S}/in.txt')
env.setdefault('WINE_WIN16_IRQ_CALLBACKS', 'serial')
open(f'{S}/out.mid', 'w').close()
prefix = os.environ.get('WINEPREFIX', os.path.join(work, 'prefix'))
ini_path = os.path.join(prefix, 'drive_c', 'PROBE', 'V' + ver, 'TTSSEQ.INI')
usual = open(ini_path, newline='').read()
if ini:
    with open(ini_path, 'w', newline='') as fh:
        fh.write(usual.replace('[Options]\r\n', '[Options]\r\n' + ''.join(x + '\r\n' for x in ini)))
try:
    out = subprocess.run([os.path.join(here, '..', 'oracles', 'try.sh'), ver, '60'], stdin=open(f'{S}/t.tpl'),
                         capture_output=True, text=True, env=env).stdout
finally:
    with open(ini_path, 'w', newline='') as fh:
        fh.write(usual)
open(f'{S}/out.txt', 'w').write(out)
keep = [x for x in out.splitlines() if not x.startswith(('load', 'SEQWIRE', 'SEQOPEN', 'SEQLOAD', 'SEQSETPLAYINGSEQ',
                                                          'SEQSETSTANDBY', 'SEQTXMIDI', 'SEQSETSRCCLOCK', 'SEQCLOSE'))]
cbs = [x for x in keep if x.startswith('callback')]
nows = [int(x.split()[-1], 16) for x in keep if x.startswith('SEQGETNOW')]
rest = [x for x in keep if not x.startswith(('callback', 'SEQGETNOW'))]
print('\n'.join(rest))
four = [x for x in cbs if x.startswith('callback cb3(0004')]
other = [x for x in cbs if not x.startswith('callback cb3(0004')]
print('callbacks:', '; '.join(other[:4]), f'| (4, ...) x{len(four)}:', four[0] if four else '', '...', four[-1] if four else '')
line = []
for x in open(f'{S}/out.mid'):
    p = x.split()
    if len(p) < 3: continue
    t = float(p[0])
    if p[1] == 'short' and p[2] == 'bf' and p[3] == '55':
        j = int(p[4], 16); line.append(f'{t:.0f}:Now={nows[j] if j < len(nows) else "?"}')
    elif p[1] == 'short' and p[2][0] == '9' and not (len(p) > 4 and p[4] == '00'): line.append(f'{t:.0f}:{".".join(p[2:4])}')
    elif p[1] == 'short' and p[2] == 'b0' and p[3] == '7b': line.append(f'{t:.0f}:AllNotesOff')
    elif p[1] == 'in' and line == []: line.append(f'{t:.0f}:first-code')
print('  '.join(line))
