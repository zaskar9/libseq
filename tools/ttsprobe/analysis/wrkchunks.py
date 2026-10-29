#!/usr/bin/env python3
"""wrkchunks.py FILE...: lists the chunks of Cakewalk WRK files (id, length, first bytes)."""
import struct, sys
for f in sys.argv[1:]:
    d = open(f, 'rb').read()
    print(f, 'version', d[9:11].hex())
    p = 11
    while p < len(d):
        t = d[p]
        if t == 0xff: print('  end'); break
        n = struct.unpack('<I', d[p+1:p+5])[0]
        print(f'  {t:3d} len {n:5d} {d[p+5:p+5+min(n,24)].hex()}')
        p += 5 + n
