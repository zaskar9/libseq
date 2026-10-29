#!/usr/bin/env python3
"""Prints the Now time stored in a .WRK file's VARS chunk (type 3). wrknow.py FILE"""
import struct, sys
b = open(sys.argv[1], 'rb').read()
i = 11  # "CAKEWALK", 0x1a, version (2 bytes)
while i < len(b):
    t = b[i]
    if t == 0xff:
        break
    n = struct.unpack_from('<I', b, i + 1)[0]
    if t == 3:
        print(struct.unpack_from('<I', b, i + 5)[0])
        break
    i += 5 + n
