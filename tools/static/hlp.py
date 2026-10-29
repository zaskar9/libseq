#!/usr/bin/env python3
"""Minimal WinHelp 3.1 (.HLP) text extractor, written for WCWLITE.HLP (Cakewalk Apprentice).

    hlp.py FILE.HLP --list      list the internal files
    hlp.py FILE.HLP --topics    print every topic's title and text (LZ77 + phrase decompression)
    hlp.py FILE.HLP             print the printable runs of the raw decompressed |TOPIC stream

Based on the public description of the WinHelp file format (helpfile.txt / helpdeco). Formatting
is dropped; a few runs are cut short where formatting codes interrupt the text."""
import struct, sys, re

data = open(sys.argv[1], 'rb').read()
magic, dirstart, firstfree, size = struct.unpack_from('<IiiI', data, 0)
assert magic == 0x00035F3F, hex(magic)


def internal_file(off):
    reserved, used, flags = struct.unpack_from('<IIB', data, off)
    return data[off + 9: off + 9 + used]


# directory: a B+ tree; walk it
btree = internal_file(dirstart)
(magic2, flags, pagesize, structure, mustbezero, pagesplits, rootpage, mustbeneg1,
 totalpages, nlevels, totalentries) = struct.unpack_from('<HHH16sHHHhHHI', btree, 0)
hdr = 38
files = {}


def leaf(pageno):
    base = hdr + pageno * pagesize
    unused, nentries, prev, nxt = struct.unpack_from('<HHhh', btree, base)
    p = base + 8
    for _ in range(nentries):
        end = btree.index(b'\0', p)
        name = btree[p:end].decode('latin1')
        p = end + 1
        (off,) = struct.unpack_from('<i', btree, p)
        p += 4
        files[name] = off
    return nxt


# find the first leaf: descend from root along first pointer
page = rootpage
for _ in range(nlevels - 1):
    base = hdr + page * pagesize
    unused, nentries, prevpage = struct.unpack_from('<HHh', btree, base)
    page = prevpage
while page != -1:
    page = leaf(page)

if '--list' in sys.argv:
    for n, o in files.items():
        print(n, o, len(internal_file(o)))
    sys.exit()

system = internal_file(files['|SYSTEM'])
smagic, minor, _, gendate, sflags = struct.unpack_from('<HHHIH', system, 0)
compressed = sflags & 0x4 or sflags & 0x8
print(f'# |SYSTEM minor={minor} flags={sflags:#x} compressed={bool(compressed)}', file=sys.stderr)
# system records
p = 12
while p + 4 <= len(system):
    rtype, rsize = struct.unpack_from('<HH', system, p)
    p += 4
    rec = system[p:p + rsize]
    p += rsize
    if rtype in (1, 2, 3, 4):
        print(f'# sys{rtype}: {rec.rstrip(b"\0").decode("latin1", "replace")}', file=sys.stderr)


def lz77(buf):
    out = bytearray()
    i = 0
    while i < len(buf):
        flags = buf[i]
        i += 1
        for bit in range(8):
            if i >= len(buf):
                break
            if flags & (1 << bit):
                code = struct.unpack_from('<H', buf, i)[0]
                i += 2
                length = (code >> 12) + 3
                dist = (code & 0xfff) + 1
                for _ in range(length):
                    out.append(out[-dist])
            else:
                out.append(buf[i])
                i += 1
    return bytes(out)


topic = internal_file(files['|TOPIC'])
blocks = []
BLOCK = 4096
for b in range(0, len(topic), BLOCK):
    blk = topic[b:b + BLOCK]
    lastlink, firstlink, lasthdr = struct.unpack_from('<iii', blk, 0)
    body = blk[12:]
    blocks.append(lz77(body) if compressed else body)
stream = b''.join(blocks)
bounds = []
acc = 0
for b in blocks:
    acc += len(b); bounds.append(acc)
print('# block sizes', [len(b) for b in blocks][:10], file=sys.stderr)

# Topic links: walk the decompressed stream. TOPICLINK: BlockSize i32, DataLen2 i32, PrevBlock i32,
# NextBlock i32, DataLen1 i32, RecordType u8. Text runs are in the second data part.
# Simpler and robust enough for us: emit printable runs of the whole stream in order.
if '--topics' not in sys.argv:
    for t in re.findall(rb'[\x09\x20-\x7e\x80-\xff]{3,}', stream):
        print(t.decode('cp1252', 'replace'))

# ---- second pass: TOPICLINK walk with phrase decompression
if '--topics' in sys.argv:
    ph = internal_file(files['|Phrases'])
    nph, onehundred = struct.unpack_from('<hH', ph, 0)
    p = 4
    if compressed:
        (decsize,) = struct.unpack_from('<i', ph, p); p += 4
    offs = struct.unpack_from('<%dH' % (nph + 1), ph, p)
    base = p
    phrasedata = ph[base + 2 * (nph + 1):]
    if compressed:
        phrasedata = lz77(phrasedata)
    # offsets are relative to the start of the offset table
    tablelen = 2 * (nph + 1)
    phrases = [phrasedata[offs[i] - tablelen: offs[i + 1] - tablelen] for i in range(nph)]
    print(f'# {nph} phrases', file=sys.stderr)

    def dephrase(b):
        out = bytearray(); i = 0
        while i < len(b):
            c = b[i]
            if 1 <= c <= 15 and i + 1 < len(b):
                n = ((c - 1) * 256 + b[i + 1]) // 2
                if n < len(phrases):
                    out += phrases[n]
                    if b[i + 1] & 1:
                        out += b' '
                i += 2
            else:
                out.append(c); i += 1
        return bytes(out)

    pos = 0; ntop = 0
    while pos + 21 <= len(stream):
        bs, dl2, prev, nxt, dl1, rt = struct.unpack_from('<iiiiiB', stream, pos)
        if bs <= 0 or bs > 0x100000 or dl1 < 21 or dl1 > bs:
            nb = [x for x in bounds if x > pos]
            if not nb:
                break
            pos = nb[0]
            continue
        d1 = stream[pos + 21: pos + dl1]
        d2 = stream[pos + dl1: pos + bs]
        if rt == 2:
            # topic header: BlockSize i32, BrowseBck, BrowseFor, TopicNum, then NonScroll, Scroll, NextTopic
            txt = dephrase(d2).split(b'\0')
            title = txt[0].decode('cp1252', 'replace') if txt else ''
            print(f'\n==== TOPIC: {title}')
            ntop += 1
        elif rt in (0x20, 0x23):
            txt = dephrase(d2)
            for run in txt.split(b'\0'):
                if run.strip():
                    print(run.decode('cp1252', 'replace'))
        pos += bs
    print(f'# {ntop} topics', file=sys.stderr)
