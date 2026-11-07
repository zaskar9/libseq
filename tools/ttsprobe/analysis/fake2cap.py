#!/usr/bin/env python3
"""Converts a fakeseq log (ms kind bytes) into midicap's format (ms bytes), keeping MIDI messages only.
With two fake output ports the kinds carry the port ("short@2"); the port then goes before the bytes
("@2 90 3c 64"). The scripted input the fake port delivered stays too, as "ms < bytes", on the same
clock: the script runner replays the DLL's timeline from it. fake2cap.py IN OUT"""
import sys
out = open(sys.argv[2], 'w')
for line in open(sys.argv[1]):
    p = line.split()
    if len(p) < 3:
        continue
    kind, _, port = p[1].partition('@')
    if kind in ('short', 'sysex'):
        out.write(f"{float(p[0]):11.3f} {'@' + port + ' ' if port else ''}{' '.join(p[2:])}\n")
    elif kind == 'in':
        out.write(f"{float(p[0]):11.3f} < {' '.join(p[2:])}\n")
