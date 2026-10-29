#!/usr/bin/env python3
"""mkvariants.py vars|trk BASE.WRK OUTDIR [plus1|toggle]: writes copies of a song with one byte
changed, to map file bytes to engine parameters (sweep/varsmap.sh, sweep/trkmap.sh).

  vars   V_BASE.WRK and V_00.WRK .. V_88.WRK: byte NN of the VARS chunk changed
  trk    T_BASE.WRK and TccNNp.WRK / TccNNt.WRK: byte NN of the first chunk of type cc
         (1 TRACK, 9 TRKOFFS, 12 TRKREPS, 14 TRKPATCH, 19 TRKVOL, 24 TRKNAME) changed

plus1 adds one to the byte (mod 256); toggle sets 0 to 255 and anything else to 0 (vars: one mode
per run, default toggle; trk: both, as the p and t files). A byte that holds a string length can
make the engine crash on loading; trkmap.sh runs each variant in its own process for that reason.
The maps in docs/reverse-engineering/params/ were made from songs/engine/ENG_ORDR.WRK."""
import os, struct, sys


def chunks(d):
    p, out = 11, []
    while d[p] != 0xFF:
        n = struct.unpack('<I', d[p + 1:p + 5])[0]
        out.append((d[p], p + 5, n))
        p += 5 + n
    return out


def change(b, mode):
    return (b + 1) & 0xFF if mode == 'plus1' else (255 if b == 0 else 0)


kind, base, outdir = sys.argv[1], sys.argv[2], sys.argv[3]
mode = sys.argv[4] if len(sys.argv) > 4 else 'toggle'
d = open(base, 'rb').read()
os.makedirs(outdir, exist_ok=True)
if kind == 'vars':
    open(os.path.join(outdir, 'V_BASE.WRK'), 'wb').write(d)
    _, start, n = next(c for c in chunks(d) if c[0] == 3)
    for i in range(n):
        v = bytearray(d); v[start + i] = change(v[start + i], mode)
        open(os.path.join(outdir, f'V_{i:02d}.WRK'), 'wb').write(v)
else:
    open(os.path.join(outdir, 'T_BASE.WRK'), 'wb').write(d)
    for cc in (1, 9, 12, 14, 19, 24):
        c = next((c for c in chunks(d) if c[0] == cc), None)
        if c is None:
            continue
        _, start, n = c
        for i in range(n):
            for m, tag in (('plus1', 'p'), ('toggle', 't')):
                v = bytearray(d); v[start + i] = change(v[start + i], m)
                open(os.path.join(outdir, f'T{cc:02d}{i:02d}{tag}.WRK'), 'wb').write(v)
