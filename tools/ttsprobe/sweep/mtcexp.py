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
Callbacks run at interrupt time (WINE_WIN16_IRQ_CALLBACKS=serial). Results: $TTSPROBE_WORK/mtc/.
"""
import os, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__))
work = os.environ.get('TTSPROBE_WORK', os.path.expanduser('~/ttsprobe-work'))
S = os.path.join(work, 'mtc'); os.makedirs(S, exist_ok=True)
a = sys.argv[1:]
ver, song = a[0], a[1]
opt = {'code': '0:0:10:0', 'rate': '30', 'speed': '1', 'from': '1500', 'to': '6000', 'hold': '1500'}
flags = set(); i = 2
while i < len(a):
    k = a[i][2:]
    if k == 'late': flags.add(k); i += 1
    else: opt[k] = a[i + 1]; i += 2
h, m, s, f = (int(x) for x in opt['code'].split(':'))
rate = int(opt['rate']); nominal = 30 if rate == 29 else rate
rcode = {24: 0, 25: 1, 29: 2, 30: 3}[rate]


def frames_to_hmsf(n):
    """frame count since 0:0:0:0 to h, m, s, f (drop frame for 29: frames 0 and 1 of each minute
    but every tenth are skipped)"""
    if rate == 29:
        d, mm = divmod(n, 17982)
        n += 18 * d + (2 * ((mm - 2) // 1798) if mm >= 2 else 0)
    ff = n % nominal; t = n // nominal
    return t // 3600, t // 60 % 60, t % 60, ff


start = ((h * 60 + m) * 60 + s) * nominal + f
if rate == 29:  # the frame count of a drop-frame code
    mins = h * 60 + m
    start -= 2 * (mins - mins // 10)
qf = 1000.0 / (nominal * (1000 / 1001 if rate == 29 else 1) * 4) / float(opt['speed'])
L, k, t = [], 0, float(opt['from'])
while t <= float(opt['to']):
    piece = k % 8
    hh, mi, ss, ff = frames_to_hmsf(start + (k // 8) * 2)
    vals = [ff & 15, ff >> 4, ss & 15, ss >> 4, mi & 15, mi >> 4, hh & 15, (hh >> 4) | (rcode << 1)]
    L.append(f'{t:.1f} f1 {(piece << 4) | vals[piece]:02x}')
    k += 1; t = float(opt['from']) + k * qf
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
T += [] if 'late' in flags else setup
T += ['call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000']
T += setup if 'late' in flags else []
T += ['cbret 0', 'cbyield 1', '?101,210 call.w SEQPLAYSTART cb3', '?300 call.w SEQSTART cb3 @2', '?300 dump 2 0 4',
      'call.d SEQGETNOW 0001', 'call.w SEQTXMIDIMSG 0000 000f 00b0 0055 0000', f'idle {opt["hold"]}',
      'call.d SEQGETNOW 0001', 'call.w SEQTXMIDIMSG 0000 000f 00b0 0055 0001', '@stop', '@close']
open(f'{S}/t.tpl', 'w').write('\n'.join(T) + '\n')
env = dict(os.environ, TTSPROBE_WORK=work, FAKELOG=f'{S}/out.mid', FAKESEQ_INPUT=f'{S}/in.txt')
env.setdefault('WINE_WIN16_IRQ_CALLBACKS', 'serial')
open(f'{S}/out.mid', 'w').close()
out = subprocess.run([os.path.join(here, '..', 'oracles', 'try.sh'), ver, '40'], stdin=open(f'{S}/t.tpl'),
                     capture_output=True, text=True, env=env).stdout
open(f'{S}/out.txt', 'w').write(out)
keep = [x for x in out.splitlines() if not x.startswith(('load', 'SEQWIRE', 'SEQOPEN', 'SEQLOAD', 'SEQSETPLAYINGSEQ',
                                                          'SEQSETSTANDBY', 'SEQTXMIDI', 'SEQSETSRCCLOCK', 'SEQCLOSE'))]
cbs = [x for x in keep if x.startswith('callback')]
rest = [x for x in keep if not x.startswith('callback')]
print('\n'.join(rest))
four = [x for x in cbs if x.startswith('callback cb3(0004')]
other = [x for x in cbs if not x.startswith('callback cb3(0004')]
print('callbacks:', '; '.join(other[:4]), f'| (4, ...) x{len(four)}:', four[0] if four else '', '...', four[-1] if four else '')
line = []
for x in open(f'{S}/out.mid'):
    p = x.split()
    if len(p) < 3: continue
    t = float(p[0])
    if p[1] == 'short' and p[2] == 'bf' and p[3] == '55': line.append(f'{t:.0f}:Now#{int(p[4], 16)}')
    elif p[1] == 'short' and p[2][0] == '9' and not (len(p) > 4 and p[4] == '00'): line.append(f'{t:.0f}:{".".join(p[2:4])}')
    elif p[1] == 'short' and p[2] == 'b0' and p[3] == '7b': line.append(f'{t:.0f}:AllNotesOff')
    elif p[1] == 'in' and line == []: line.append(f'{t:.0f}:first-code')
print('  '.join(line))
