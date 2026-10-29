#!/usr/bin/env python3
"""coverage.py VER [DATA]: which exports of the version the rendered suites (DATA/VER/*.txt, DATA
defaulting to tests/data/ttsseq) call, and which they do not."""
import glob, os, re, sys
ver = sys.argv[1]
here = os.path.dirname(os.path.abspath(__file__))
exports = []
repo = os.path.normpath(os.path.join(here, '..', '..', '..'))
data = sys.argv[2] if len(sys.argv) > 2 else os.path.join(repo, 'tests', 'data', 'ttsseq')
for l in open(os.path.join(repo, 'docs', 'reverse-engineering', 'exports', f'exports{ver}.txt')):
    p = l.split()
    if p:
        exports.append(p[-1])
used = set()
for f in glob.glob(os.path.join(data, ver, '*.txt')):
    for l in open(f):
        m = re.match(r'call(?:\.\w)?\s+(\w+)', l)
        if m:
            used.add(m[1])
used |= {'SEQIDLE'}  # called by "idle"
skip = {'WEP', 'DLGPROC', 'TIMERTRAP', 'RXTRAP', '___EXPORTEDSTUB', '?DLGPROC@@ZCHPBUHWND__@@IIJ@Z'}
api = [e for e in exports if e not in skip]
missing = [e for e in api if e not in used]
print(f'{ver}: {len(api) - len(missing)} of {len(api)} API exports covered')
print('not covered:', ' '.join(missing))
