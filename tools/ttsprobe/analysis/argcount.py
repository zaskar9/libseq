#!/usr/bin/env python3
"""Argument sizes of a DLL's exports, from Wine +snoop logs (sweeps and program traces).

argcount.py MODULE exports.txt LOG...   prints: ordinal name words source result
Wine prints a function's argument words on the first RET line (and on later CALL lines); the count
is what the callee removed from the stack, so it does not depend on what the caller pushed.
A function seen in no log, or only crashing, is reported with '?'."""
import re
import sys

mod, exports_path, logs = sys.argv[1], sys.argv[2], sys.argv[3:]
exports = []
for line in open(exports_path):
    o, n = line.split()
    exports.append((int(o), n))
call = re.compile(r'^[0-9a-f]+:CALL ' + re.escape(mod) + r'\.(\d+): ([A-Z0-9_?@]*)\((.*)\) ret=')
ret = re.compile(r'^[0-9a-f]+:RET  ' + re.escape(mod) + r'\.(\d+): ([A-Z0-9_?@]*)\((.*)\) retval = (\S+)')
words, source, result, crashed = {}, {}, {}, set()
for log in logs:
    pending = {}
    for line in open(log, errors='replace'):
        m = call.match(line)
        if m:
            name, a = m.group(2), m.group(3).strip()
            pending[name] = True
            if not a.startswith('<unknown'):
                words.setdefault(name, 0 if a == '' else len(a.split(',')))
                source.setdefault(name, log.split('/')[-2] if '/' in log else log)
            continue
        m = ret.match(line)
        if m:
            name, a, r = m.group(2), m.group(3).strip(), m.group(4)
            pending.pop(name, None)
            if name not in words:
                words[name] = 0 if a == '' else len(a.split(','))
                source[name] = log.split('/')[-2] if '/' in log else log
            result.setdefault(name, r)
    crashed.update(pending)
for o, n in exports:
    w = words.get(n)
    print(f"{o:4} {n:28} {'?' if w is None else w:>3} {source.get(n, 'crash' if n in crashed else '-'):24} {result.get(n, '')}")
