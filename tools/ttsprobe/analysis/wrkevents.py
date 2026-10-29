#!/usr/bin/env python3
"""wrkevents.py FILE: prints the events of every stream chunk (2) of a WRK file: tick, bytes, duration."""
import struct, sys
d = open(sys.argv[1], 'rb').read(); p = 11
while d[p] != 0xff:
    t = d[p]; n = struct.unpack('<I', d[p+1:p+5])[0]; c = d[p+5:p+5+n]
    if t == 2:
        print('track', struct.unpack('<H', c[:2])[0])
        for k in range(4, n, 8):
            e = c[k:k+8]; print(' ', e[0] | e[1] << 8 | e[2] << 16, e[3:6].hex(), struct.unpack('<H', e[6:8])[0])
    p += 5 + n
