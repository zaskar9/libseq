#!/usr/bin/env python3
"""diffversions.py A B: where two versions' oracle results differ, per suite. Lines that only
differ because of the version's own calls (SeqWire, the load and save layouts, SeqOpen's result)
are left out. Prints each differing call with both results, grouped by function, so the deltas
between engine versions can be read off."""
import collections, difflib, os, re, sys

a, b = sys.argv[1], sys.argv[2]
SKIP = re.compile(r'^(SEQWIRE|SEQOPEN|load )')


def norm(line):
    # the load and save calls differ in layout between versions, not in meaning
    line = re.sub(r'^(SEQLOADCAKEWALKFILE\("[^"]*"),0000(,null,0000,0000)?\)', r'\1)', line)
    line = re.sub(r'^(SEQSAVECAKEWALKFILE\(\w+,"[^"]*",d:00000000)(,null,d:00000000)?\)', r'\1)', line)
    line = re.sub(r'^SEQSTART\(null,0000,0000\)', 'SEQPLAYSTART(null)', line)
    line = re.sub(r'^SEQSTOP\(\)', 'SEQPLAYSTOP()', line)
    return line


for s in sorted(f[:-4] for f in os.listdir(a) if f.endswith('.out')):
    pb = os.path.join(b, s + '.out')
    if not os.path.exists(pb):
        continue
    la = [norm(l) for l in open(os.path.join(a, s + '.out')).read().splitlines() if not SKIP.match(l)]
    lb = [norm(l) for l in open(pb).read().splitlines() if not SKIP.match(l)]
    sm = difflib.SequenceMatcher(None, la, lb, autojunk=False)
    byfn = collections.OrderedDict()
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == 'equal':
            continue
        # attribute the change to the last call line before it
        k = i1
        while k > 0 and not re.match(r'^[A-Z0-9_]+\(', la[k - 1] if k - 1 < len(la) else ''):
            k -= 1
        call = la[k - 1] if k > 0 else '(start)'
        fn = call.split('(')[0]
        byfn.setdefault(fn, []).append((call, la[i1:i2], lb[j1:j2]))
    if not byfn:
        continue
    print(f'== {s}: {sum(len(v) for v in byfn.values())} differences')
    for fn, items in byfn.items():
        call, x, y = items[0]
        print(f'  {fn} ({len(items)}x), e.g. after {call}')
        print(f'    {a}: {" | ".join(x[:3])}')
        print(f'    {b}: {" | ".join(y[:3])}')
