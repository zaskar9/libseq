#!/usr/bin/env python3
"""Writes a copy of Wine's 32-bit winealsa.so whose MIDI ports report manufacturer ID 2 instead of
255 (Wine's placeholder). TTSSEQ 2.10 and 3.00 skip ports with manufacturer 255, output and input;
1.01 does not. This changes Wine only.  patch-winealsa.py IN OUT"""
import sys
b = bytearray(open(sys.argv[1], 'rb').read())
# mov dword [edi+0x22], 0x000100ff: wMid = 0xff, wPid = 1 of an output port's MIDIOUTCAPS;
# mov dword [esi+0x28], 0x000100ff: the same for an input port's MIDIINCAPS (Wine 9.0)
for name, pat in (('output', 'c74722ff000100'), ('input', 'c74628ff000100')):
    pat = bytes.fromhex(pat)
    i = b.find(pat)
    assert i >= 0 and b.find(pat, i + 1) < 0, f'{name} pattern not found exactly once (other Wine version?)'
    b[i + 3] = 0x02
    print(f'{name}: patched at {i + 3:#x}')
open(sys.argv[2], 'wb').write(b)
