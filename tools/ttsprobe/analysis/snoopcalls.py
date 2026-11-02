#!/usr/bin/env python3
"""snoopcalls.py LOG [FROM_LINE] [--all] : the calls into TTSSEQ in a +snoop log, in order, with
their arguments and results, one line each. Wine prints the arguments on the CALL line once it
knows their number, and on the first RET line before that; both are used. The calls a program
makes to poll and to redraw its position displays (SeqIdle, SeqGetNow, SeqTick2MBTStr ...) are
left out unless --all; repeats of the same call in a row are counted. FROM_LINE starts at that
line of the log, to see only what one action in the program did."""
import re, sys
args = [a for a in sys.argv[1:] if a != '--all']
log, start = args[0], int(args[1]) if len(args) > 1 else 0
NOISE = {'SEQIDLE', 'SEQGETNOW', 'SEQGETPPQ', 'SEQEXPIRED', 'SEQTICK2MBT', 'SEQTICK2FRAME',
         'SEQFRAME2HMSF', 'SEQGETTEMPO', 'SEQGETMETER', 'SEQMEASHASEVTS', 'SEQTICK2MEAS',
         'SEQTICK2MBTSTR', 'SEQTICK2HMSFSTR', 'SEQWHICHTEMPORATIO', 'SEQGETTEMPORATIO',
         'SEQGETMARKERTIME', 'SEQMARKERFORTIME', 'SEQMETERIXBYTICK', 'SEQGETEND', 'SEQMBT2TICK'}
CALL = re.compile(r'CALL TTSSEQ\.\d+: (\w+)\((.*?)\) ret=')
RET = re.compile(r'RET  TTSSEQ\.\d+: (\w+)\((.*?)\) retval = ([0-9a-f]{4}:[0-9a-f]{4})')
pending = {}
last, n = None, 0
def flush():
    if last: print(last[0] + (f'   (x{n})' if n > 1 else ''))
with open(log, errors='replace') as f:
    for i, line in enumerate(f, 1):
        if i < start: continue
        m = CALL.search(line)
        if m:
            pending.setdefault(m.group(1), []).append((i, m.group(2))); continue
        m = RET.search(line)
        if not m: continue
        name = m.group(1)
        ci, a = pending[name].pop() if pending.get(name) else (i, '')
        if a.startswith('<unknown') or not a: a = m.group(2)
        if '--all' not in sys.argv and name in NOISE: continue
        key = f'{name}({a}) -> {m.group(3)}'
        if last and last[1] == key: n += 1; continue
        flush(); last, n = (f'{ci:8d} {key}', key), 1
flush()
