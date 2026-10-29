#!/usr/bin/env python3
"""Prints the VARS chunk (type 3) of .WRK files as offset:byte for the non-zero bytes. wrkvars.py FILE..."""
import struct, sys
for path in sys.argv[1:]:
    b = open(path, 'rb').read()
    i = 11
    while i < len(b) and b[i] != 0xff:
        t, n = b[i], struct.unpack_from('<I', b, i + 1)[0]
        if t == 3:
            v = b[i + 5:i + 5 + n]
            print(path.split('/')[-1], len(v), ' '.join(f'{k}:{x:02x}' for k, x in enumerate(v) if x))
            break
        i += 5 + n
