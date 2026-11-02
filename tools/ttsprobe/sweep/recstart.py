#!/usr/bin/env python3
"""recstart.py VER [OPTIONS] WORDS... : one recording with the five leading words of
SeqRecordStart set to WORDS (render.py syntax: 0001, d:0000012c, %1 ...). 1.01 and 2.10 only.

Records like the oracle suite `record` (one count-in measure at 100 bpm, 120 ppq, input IN3.TXT)
and prints the start call's result, the take's events (tick, message, duration), the streams
asked for and the MIDI the engine sent (ms after the first count-in click). Found the punch
(stream, in32, out32); see docs/reverse-engineering/ttsseq-api.md, Recording. Options:
  --input FILE    other scripted MIDI input
  --set P=V       set sequence parameter P to V (both hex) before the start
  --pre LINE      a probe line before the start call (make a stream, `buf 2 64` for @2, ...)
  --started LINE  a line right after the start call returns
  --post LINE     a line at the end of the take, before SeqRecordStop
  --stopped LINE  a line between SeqRecordStop and SeqRecordCleanup
  --after LINE    a line after SeqRecordCleanup (%b1 is the take's stream)
  --keep W        SeqRecordCleanup's second word (default 0001)
  --walk H        list stream H after the take as well (%N: a result kept with `let N`)
It runs with WINE_WIN16_IRQ_CALLBACKS=serial unless that is set otherwise (a start call that
waits without a callback blocks Wine without it). Results are kept in $TTSPROBE_WORK/recstart/.
"""
import os, re, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__))
work = os.environ.get('TTSPROBE_WORK', os.path.expanduser('~/ttsprobe-work'))
data = os.path.join(here, '..', '..', '..', 'tests', 'data', 'ttsseq', 'input')
args = sys.argv[1:]
inp, sets, secs, pre, post, walks, keep, after, stopped, started = os.path.join(data, 'IN3.TXT'), [], '40', [], [], [], '0001', [], [], []
while len(args) > 1 and args[1].startswith('--'):
    if args[1] == '--input': inp = os.path.abspath(args[2])
    elif args[1] == '--set': sets.append(args[2].split('='))
    elif args[1] == '--secs': secs = args[2]
    elif args[1] == '--pre': pre.append(args[2])
    elif args[1] == '--walk': walks.append(args[2])
    elif args[1] == '--keep': keep = args[2]
    elif args[1] == '--after': after.append(args[2])
    elif args[1] == '--stopped': stopped.append(args[2])
    elif args[1] == '--started': started.append(args[2])
    elif args[1] == '--post': post.append(args[2])
    del args[1:3]
ver, words = args[0], args[1:]
out_dir = os.path.join(work, 'recstart'); os.makedirs(out_dir, exist_ok=True)
tag = re.sub(r'[^A-Za-z0-9_-]', '_', '_'.join([ver] + (['keep' + keep] if keep != '0001' else []) + words + [f'{p}-{v}' for p, v in sets]))
tpl = ['@open', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQSETSEQPARAM 0001 0010 d:00000001']
tpl += [f'call.w SEQSETSEQPARAM 0001 {int(p, 16):04x} d:{int(v, 16):08x}' for p, v in sets]
tpl += pre + ['sleep 800', 'call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000', 'cbret 0', 'cbyield 1',
        'call.w SEQRECORDSTART ' + ' '.join(words) + ' cb3'] + started + ['idle 2500'] + post + [
        'call.w SEQRECORDSTOP', 'call.d SEQRECCOUNT'] + stopped + ['buf 1 16', 'call.w SEQRECORDCLEANUP 0001 ' + keep + ' 0000 @1',
        'dump 1 0 2', 'buf 0 16'] + after
for h in (['%b1'] if keep != '0000' else []) + walks:
    tpl += ['echo == stream ' + h, f'call.d SEQSTREAMLEN {h}', f'call.d SEQSEEK {h} 0000 d:00000000']
    for _ in range(14):
        tpl += ['fill 0 0 ee ee ee ee ee ee ee ee', f'call.w SEQGETEVT {h} @0', 'dump 0 0 8', f'call.w SEQNXTEVT {h}']
tpl += ['call.d SEQGETSEQPARAM 0001 0003', 'call.d SEQGETNOW 0001', '@close']
fake = os.path.join(out_dir, tag + '.fake'); open(fake, 'w').close()
env = dict(os.environ, TTSPROBE_WORK=work, FAKELOG=fake, FAKESEQ_INPUT=inp)
env.setdefault('WINE_WIN16_IRQ_CALLBACKS', 'serial')
res = subprocess.run([os.path.join(here, '..', 'oracles', 'try.sh'), ver, secs], input='\n'.join(tpl) + '\n',
                     capture_output=True, text=True, env=env).stdout
open(os.path.join(out_dir, tag + '.out'), 'w').write(res)
lines = res.splitlines()
for i, l in enumerate(lines):
    if l.startswith(('SEQRECORDSTART', 'SEQRECCOUNT', 'SEQRECORDCLEANUP', 'SEQSTREAMLEN', 'SEQGETSEQPARAM', 'SEQGETNOW', 'SEQGETSTREAMPARAM', 'SEQSTREAMLEN(%1')):
        print(l)
    if l.startswith('== stream') or (i and lines[i - 1].startswith('dump 1')):
        print(l)
    if l.startswith(('cb3', 'dump 2', 'dump 3')) or (i and lines[i - 1].startswith(('dump 2', 'dump 3'))):
        print('  ' + l)
    if l.startswith('SEQGETEVT') and l.endswith('0001'):
        b = lines[i + 2].split()
        if len(b) < 8: continue
        t = int(b[2] + b[1] + b[0], 16)
        print(f'  event {t:5d}  {" ".join(b[3:6])}  dur {int(b[7] + b[6], 16)}')
print('-- MIDI out (ms after the first count-in click)')
t0 = None
for l in open(fake):
    p = l.split()
    if len(p) > 1 and p[1] in ('short', 'sysex', 'long'):
        if t0 is None and p[1:3] == ['short', '99']: t0 = float(p[0])
        t = f'{float(p[0]) - t0:7.0f}' if t0 is not None else '       '
        print(f'  {t} ' + ' '.join(p[1:]))
