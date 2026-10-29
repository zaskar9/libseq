#!/usr/bin/env python3
"""render.py VER SUITE.tpl > script: turns a version-independent oracle suite into a ttsprobe script
for one TTSSEQ version (101, 210, 300).

Template lines are ttsprobe commands, plus:
  ?VERS line      the line only for the listed versions (comma-separated, e.g. ?210,300)
  ?!VERS line     the line for all versions except the listed ones
  @open           load the DLL, SeqWire(1) where needed, SeqOpen(0, 0)
  @load FILE      SeqLoadCakewalkFile("C:\\PROBE\\ORC\\FILE") in the version's layout; result word
  @save SEQ FILE  SeqSaveCakewalkFile(SEQ, "C:\\PROBE\\ORC\\OUT\\FILE") without a program callback
  @savemid SEQ FILE   SeqSaveMidiFile(SEQ, "C:\\PROBE\\ORC\\OUT\\FILE", 0)
  @play           start playback (SeqPlayStart(null), or SeqStart(null, 0) in 3.00)
  @stop           stop playback
  @close          SeqClose()
Every script starts with "mode oracle"."""
import sys

ver = sys.argv[1]
dll = 'TTSSEQ30.DLL' if ver == '300' else 'TTSSEQ.DLL'
import os
out = [] if os.environ.get('RAW') else ['mode oracle']


def path(f, sub=''):
    return '"C:\\PROBE\\ORC\\' + sub + f + '"'


for raw in open(sys.argv[2]):
    line = raw.rstrip('\n')
    if line.startswith('?'):
        cond, _, line = line[1:].partition(' ')
        neg = cond.startswith('!')
        vers = cond.lstrip('!').split(',')
        if (ver in vers) == neg:
            continue
    words = line.split()
    if not words:
        out.append(line); continue
    cmd = words[0]
    if cmd == '@open':
        out.append(f'load {dll}')
        if ver != '101':
            out.append('call.w SEQWIRE 0001')
        out.append('call.w SEQOPEN 0000 0000')
    elif cmd == '@load':
        if ver == '300':
            out.append(f'call.w SEQLOADCAKEWALKFILE {path(words[1])} 0000')
        else:
            out.append(f'call.w SEQLOADCAKEWALKFILE {path(words[1])} 0000 null 0000 0000')
    elif cmd == '@save':
        if ver == '300':
            out.append(f'call.w SEQSAVECAKEWALKFILE {words[1]} {path(words[2], "OUT\\")} d:00000000')
        else:
            out.append(f'call.w SEQSAVECAKEWALKFILE {words[1]} {path(words[2], "OUT\\")} d:00000000 null d:00000000')
    elif cmd == '@savemid':
        out.append(f'call.w SEQSAVEMIDIFILE {words[1]} {path(words[2], "OUT\\")} d:00000000')
    elif cmd == '@play':
        out.append('call.w SEQSTART null 0000 0000' if ver == '300' else 'call.w SEQPLAYSTART null')
    elif cmd == '@stop':
        out.append('call.w SEQSTOP' if ver == '300' else 'call.w SEQPLAYSTOP')
    elif cmd == '@close':
        out.append('call.w SEQCLOSE')
    else:
        out.append(line)
print('\n'.join(out))
