#!/usr/bin/env python3
"""Converts a fakeseq log (ms kind bytes) into midicap's format (ms bytes), keeping MIDI messages only.
fake2cap.py IN OUT"""
import sys
out = open(sys.argv[2], 'w')
for line in open(sys.argv[1]):
    p = line.split()
    if len(p) >= 3 and p[1] in ('short', 'sysex'):
        out.write(f"{float(p[0]):11.3f} {' '.join(p[2:])}\n")
