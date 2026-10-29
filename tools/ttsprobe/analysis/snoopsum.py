#!/usr/bin/env python3
"""Summarises a Wine +snoop log for one 16-bit DLL: per exported function, how often it was called,
how many argument words it takes (as Wine's snoop measured on return), and a few distinct argument
and return-value samples.  snoopsum.py LOG MODULE [exports.txt]"""
import re, sys
from collections import OrderedDict, defaultdict
log, mod = sys.argv[1], sys.argv[2]
exports = {}
if len(sys.argv) > 3:
    for l in open(sys.argv[3]):
        o, n = l.split()
        exports[n] = int(o)
# Wine prints the arguments on CALL lines once it knows their size, and on the first call's RET line.
call = re.compile(r'^[0-9a-f]+:CALL ' + re.escape(mod) + r'\.(\d+): ([A-Z0-9_?@]*)\((.*)\) ret=')
ret = re.compile(r'^[0-9a-f]+:RET  ' + re.escape(mod) + r'\.(\d+): ([A-Z0-9_?@]*)\((.*)\) retval = ([0-9a-f]{4}):([0-9a-f]{4})')
calls = defaultdict(int); words = {}; args = defaultdict(OrderedDict); rets = defaultdict(OrderedDict); ords = {}
def addargs(name, a):
    a = a.strip()
    if a.startswith('<unknown'):
        return
    words[name] = max(words.get(name, 0), 0 if a == '' else len(a.split(',')))
    if a and len(args[name]) < 4: args[name][a] = 1
for line in open(log, errors='replace'):
    m = call.match(line)
    if m:
        calls[m.group(2)] += 1; ords[m.group(2)] = int(m.group(1))
        addargs(m.group(2), m.group(3))
        continue
    m = ret.match(line)
    if not m:
        continue
    o, name, a, dx, ax = m.groups()
    ords[name] = int(o)
    addargs(name, a)
    if len(rets[name]) < 4: rets[name][f'{dx}:{ax}'] = 1
print(f"# {mod}: {len(calls)} functions called" + (f" of {len(exports)} exported" if exports else ""))
print(f"# ordinal name calls argwords | argument samples | return samples (DX:AX)")
for name in sorted(calls, key=lambda n: ords[n]):
    print(f"{ords[name]:4} {name:26} {calls[name]:7} {words[name]:2} | {' ; '.join(args[name])} | {' '.join(rets[name])}")
if exports:
    missing = sorted((o, n) for n, o in exports.items() if n not in calls)
    print(f"# not called ({len(missing)}): " + ' '.join(n for o, n in missing))
