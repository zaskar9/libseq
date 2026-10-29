#!/usr/bin/env python3
"""Turns varsmap.sh output into a map: track-record byte -> stream parameters that changed. varsmap.py OUT..."""
import re, sys
for path in sys.argv[1:]:
    blocks, cur = {}, None
    for line in open(path):
        m = re.match(r'=== (\S+)', line)
        if m:
            cur = m.group(1); blocks[cur] = {}
            continue
        m = re.match(r'SEQGETSTREAMPARAM\(0100,([0-9a-f]+)\) -> ([0-9a-f]{4}):([0-9a-f]{4})', line)
        if m and cur:
            blocks[cur][int(m.group(1), 16)] = (m.group(2), m.group(3))
    base = blocks['T_BASE']
    print(f'== {path}')
    for k in sorted((k for k in blocks if k != 'T_BASE')):
        ch = []
        for p, (dx, ax) in blocks[k].items():
            bdx, bax = base[p]
            if ax != bax or (dx != bdx and dx not in ('0000', 'ffff') and bdx not in ('0000', 'ffff')):
                ch.append(f'p{p}: {bdx}:{bax} -> {dx}:{ax}')
            elif dx != bdx:
                ch.append(f'p{p}: {bdx}:{bax} -> {dx}:{ax}')
        print(f'  {k}: ' + ('; '.join(ch) if ch else '-'))
