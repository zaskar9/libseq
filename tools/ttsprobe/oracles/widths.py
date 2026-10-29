#!/usr/bin/env python3
"""widths.py OUT...: classifies return types from a raw probe run made twice, once with "dxs 1111"
and once with "dxs 2222" (the same calls in the same order after each dxs line). Prints
"NAME TYPE AX1 AX2 DX1 DX2" per function:
  v  AX and DX came back unchanged (nothing returned)
  w  DX unchanged or the same non-zero junk in both runs: a word in AX
  d  DX zero or a value: a dword in DX:AX (also the safe choice when DX is 0 in both)
Results for the same function across files are merged; conflicts are shown as "?"."""
import re, sys
res = {}
for f in sys.argv[1:]:
    block = None; runs = {'1111': [], '2222': []}
    for line in open(f):
        line = line.strip()
        if line in ('dxs 1111', 'dxs 2222'):
            block = line[4:]; continue
        m = re.match(r'(\w+)\((.*)\) -> ([0-9a-f]{4}):([0-9a-f]{4})$', line)
        if m and block:
            runs[block].append((m[1], m[3], m[4]))
    for (n, d1, a1), (n2, d2, a2) in zip(runs['1111'], runs['2222']):
        assert n == n2, (n, n2)
        if a1 == '1111' and a2 == '2222' and d1 == '1111' and d2 == '2222': t = 'v'
        elif d1 == '1111' and d2 == '2222': t = 'w'
        elif d1 == d2 and d1 == '0000': t = 'd'
        elif d1 == d2 and a1 == a2: t = 'w?'   # constant junk in DX, or a constant dword
        else: t = 'd?'
        old = res.get(n)
        if old and old[0] != t:
            t = old[0] + '|' + t
        res[n] = (t, a1, a2, d1, d2)
for n, (t, a1, a2, d1, d2) in sorted(res.items()):
    print(f'{n:28s} {t:5s} {d1}:{a1} {d2}:{a2}')
