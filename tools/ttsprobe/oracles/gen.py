#!/usr/bin/env python3
"""gen.py: writes the oracle suites (suites/*.tpl). Each suite is a version-independent template
for render.py. Run it after changing a suite; mkoracles.sh then renders and runs them."""
import json
import os
import random

SUITES = {}
HERE = os.path.dirname(os.path.abspath(__file__))



def suite(name):
    def deco(fn):
        SUITES[name] = fn
        return fn
    return deco


STREAMS = json.load(open(os.path.join(HERE, 'streams-101.json')))  # stream handles per fixture (1.01)
SONGS_PARAMS = ['ENG_PARM', 'ENG_SYNC', 'ENG_METR', 'E3_SHTL', 'E3_SPPD', 'ENG_ORDR', 'SILENT', 'SYN_MAPS']


@suite('lifecycle')
def _():
    L = ['@open', 'call.w SEQGETVERSION']
    L += ['# handles: the lowest free number', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQCREATESEQ 0000 0000',
          'call.w SEQDESTROYSEQ 0001', 'call.w SEQCREATESEQ 0000 0000', '@load ENG_PARM.WRK',
          'call.w SEQDESTROYSEQ 0002', '@load ENG_PARM.WRK']
    for s in (1, 2, 3):
        L.append(f'call.w SEQDESTROYSEQ {s:04x}')
    L += ['@close']
    return L


@suite('seqparams')
def _():
    L = ['@open', '# a new sequence', 'call.w SEQCREATESEQ 0000 0000']
    L += [f'call.d SEQGETSEQPARAM 0001 {p:04x}' for p in range(64)]
    L += ['# set and read back: which parameters exist, which are flags, how wide they are']
    for p in range(64):
        for v in ('00000005', 'ffffffff', '00012345', '00000000'):
            L.append(f'call.w SEQSETSEQPARAM 0001 {p:04x} d:{v}')
            L.append(f'call.d SEQGETSEQPARAM 0001 {p:04x}')
    L += ['call.w SEQDESTROYSEQ 0001', '# loaded songs']
    for s in SONGS_PARAMS:
        L += [f'echo -- {s}', f'@load {s}.WRK']
        L += [f'call.d SEQGETSEQPARAM 0001 {p:04x}' for p in range(64)]
        L += ['call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


@suite('streamparams')
def _():
    L = ['@open', 'buf 0 256']
    for s in ['ENG_PARM', 'ENG_ORDR', 'ENG_MULT', 'ENG_LOOP', 'SYN_PARM', 'XPOSE']:
        L += [f'echo -- {s}', f'@load {s}.WRK']
        for hs in STREAMS[s]:
            h = int(hs, 16)
            L += [f'call.d SEQSTREAMLEN {h:04x}', f'call.d SEQSTREAMDUR {h:04x}']
            L += [f'call.d SEQGETSTREAMPARAM {h:04x} {p:04x}' for p in range(18)]
            L += ['fill 0 0 ' + ' '.join(['ee'] * 40), f'call.w SEQGETSTREAMNAME {h:04x} @0 0081', 'dump 0 0 40']
            L += ['fill 0 0 ' + ' '.join(['ee'] * 40), f'call.w SEQGETSTREAMALLPARAMS {h:04x} @0', 'dump 0 0 40']
        L += ['call.w SEQDESTROYSEQ 0001']
    L += ['# set and read back on stream 0 of ENG_ORDR', '@load ENG_ORDR.WRK']
    for p in range(18):
        for v in ('00000005', 'ffffffff', '00012345', '0000007f', '00000000'):
            L += [f'call.w SEQSETSTREAMPARAM 0100 {p:04x} d:{v}', f'call.d SEQGETSTREAMPARAM 0100 {p:04x}']
    L += ['@close']
    return L



def walk(h, n, buf=0):
    """every event of stream h (n events): seek to each index and copy it out"""
    L = [f'call.d SEQSEEK {h} 0000 d:00000000']
    for i in range(n + 1):
        L += [f'fill {buf} 0 ee ee ee ee ee ee ee ee', f'call.w SEQGETEVT {h} @{buf}', f'dump {buf} 0 8',
              f'call.d SEQTELL {h}', f'call.w SEQEOS {h}', f'call.w SEQNXTEVT {h}']
    return L


def walkall(song):
    L = []
    for h, n in STREAMS[song].items():
        L += [f'echo stream {h}', f'call.d SEQSTREAMLEN {h}'] + walk(h, n)
    return L


@suite('events')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64']
    for song in ['ENG_ORDR', 'ENG_PARM', 'SYN_KIND', 'SYN_MIN', 'SYN_OVLP', 'XPOSE', 'EFILT', 'ENG_SYSX']:
        L += [f'echo -- {song}', f'@load {song}.WRK'] + walkall(song)
        for h, n in STREAMS[song].items():
            for t in (0, 1, 0x77, 0x78, 0xf0, 0x1e0, 0x1df, 0x3c0, 0x10000):
                L += [f'call.d SEQEVTATTIMEGE {h} d:{t:08x}', f'call.d SEQEVTATTIMEGT {h} d:{t:08x}']
            L += [f'call.d SEQSTREAMDUR {h}'] + [f'call.w SEQMEASHASEVTS {h} {m:04x}' for m in range(1, 12)]
        L += ['call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


@suite('eventedit')
def _():
    n = STREAMS['ENG_ORDR']['0100']
    h = '0100'
    L = ['@open', 'buf 0 64', 'buf 1 64', '@load ENG_ORDR.WRK']
    steps = [
        ('put an event over index 2', ['call.d SEQSEEK 0100 0000 d:00000002', 'fill 1 0 f0 00 00 90 3c 64 78 00', 'call.w SEQPUTEVT 0100 @1']),
        ('put an event with a later time over index 3', ['call.d SEQSEEK 0100 0000 d:00000003', 'fill 1 0 00 03 00 91 40 50 10 00', 'call.w SEQPUTEVT 0100 @1']),
        ('insert an event at index 0', ['call.d SEQSEEK 0100 0000 d:00000000', 'fill 1 0 78 00 00 b0 07 64 00 00', 'call.w SEQINSEVT 0100 @1']),
        ('insert an event at the end', ['call.d SEQSEEK 0100 0000 d:00000019', 'fill 1 0 00 10 00 c0 05 00 00 00', 'call.w SEQINSEVT 0100 @1']),
        ('delete index 5', ['call.d SEQSEEK 0100 0000 d:00000005', 'call.w SEQDELEVT 0100']),
        ('move index 1 to tick 0x300', ['call.d SEQSEEK 0100 0000 d:00000001', 'call.d SEQMOVEEVT 0100 d:00000300']),
        ('move index 6 to tick 0', ['call.d SEQSEEK 0100 0000 d:00000006', 'call.d SEQMOVEEVT 0100 d:00000000']),
        ('copy of index 4', ['call.d SEQSEEK 0100 0000 d:00000004', 'call.w SEQGETEVTCOPY 0100 @1', 'dump 1 0 8']),
        ('temporal sort', ['call.d SEQTEMPORALSORT 0100']),
    ]
    for title, cmds in steps:
        L += [f'echo == {title}'] + cmds + walk(h, n + 4)
    L += ['@close']
    return L


@suite('ranges')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64']
    cases = [
        ('delete range 0x78..0x1e0', 'call.w SEQDELETERANGE 0100 d:00000078 d:000001e0'),
        ('delete range 0..0x78', 'call.w SEQDELETERANGE 0100 d:00000000 d:00000078'),
        ('displace earlier from 0x1e0 by 0x10', 'call.d SEQDISPLACEEARLIER 0100 d:000001e0 d:00000010'),
        ('displace earlier from 0x1e0 by 0x200', 'call.d SEQDISPLACEEARLIER 0100 d:000001e0 d:00000200'),
        ('displace later from 0xf0 by 0x78', 'call.d SEQDISPLACELATER 0100 d:000000f0 d:00000078'),
        ('displace later from 0 by 0x1e0', 'call.d SEQDISPLACELATER 0100 d:00000000 d:000001e0'),
        
    ]
    for title, c in cases:
        L += [f'echo == {title}', '@load ENG_ORDR.WRK', c] + walk('0100', STREAMS['ENG_ORDR']['0100'] + 2) + ['call.w SEQDESTROYSEQ 0001']
    # SeqInsertRange(dst, index32, src) inserts copies of all events of src into dst before the
    # index, keeping their times (the stream is not sorted again); src stays as it is. An index
    # past the end of dst crashes, and so does a src that is not a stream.
    def source(events):
        R = ['call.w SEQCREATESEQ 0000 0000', 'call.w SEQCREATESTREAM 0002 0000 0000', 'let 1']
        for i, e in enumerate(events):
            R += [f'fill 1 0 {e}', f'call.d SEQSEEK %1 0000 d:{i:08x}', 'call.w SEQINSEVT %1 @1']
        return R
    two = ['00 05 00 91 50 51 10 00', '50 00 00 b2 07 33 00 00']
    for title, at, events in [('insert range: two events at index 0', 0, two),
                              ('insert range: two events at index 3', 3, two),
                              ('insert range: two events at the end (index 19)', 19, two),
                              ('insert range: an empty stream at index 3', 3, [])]:
        n = STREAMS['ENG_ORDR']['0100'] + len(events)
        L += [f'echo == {title}', '@load ENG_ORDR.WRK'] + source(events) + \
             ['call.d SEQSEEK 0100 0000 d:00000005', f'call.d SEQINSERTRANGE 0100 d:{at:08x} %1', 'call.d SEQTELL 0100'] + \
             walk('0100', n + 1) + ['call.d SEQSTREAMLEN %1'] + walk('%1', len(events) + 1) + \
             ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    L += ['echo == insert range, then sort', '@load ENG_ORDR.WRK'] + source(two) + \
         ['call.d SEQINSERTRANGE 0100 d:00000000 %1', 'call.d SEQTEMPORALSORT 0100'] + walk('0100', STREAMS['ENG_ORDR']['0100'] + 3) + \
         ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


# Streams built event by event for the order-changing calls: data1 holds the event's original
# index, so the result shows where each event went.
SORT_CASES = [[5, 1, 1, 1, 1], [1, 1, 1, 1, 0], [2, 1, 1, 1, 1, 0], [3, 2, 1], [1, 0, 0, 0, 0, 0, 0, 0, 0],
              [9, 1, 1, 1, 1, 1, 1, 1, 1], [0, 0, 0, 1, 0], [1, 1, 0, 0], [2, 2, 1, 1, 0, 0], [5, 4, 3, 2, 1, 0],
              [1, 0] * 4, [3, 3, 3, 1, 1, 1, 2, 2, 2]]
_rnd = random.Random(7)
for _n in (6, 8, 10, 12, 16, 20, 24, 30):
    SORT_CASES += [[_rnd.randint(0, 4) for _ in range(_n)] for _ in range(3)]
DISPLACE_CASES = [([0, 10, 20, 30, 40, 50], 'EARLIER', 20, 15), ([0, 10, 20, 30, 40, 50], 'EARLIER', 20, 25),
                  ([0, 10, 20, 20, 20, 30, 30, 40], 'EARLIER', 20, 25), ([0, 10, 20, 20, 20, 30, 30, 40], 'EARLIER', 20, 12),
                  ([0, 10, 20, 30], 'LATER', 10, 0xfffff0), ([0, 10, 10, 10, 20, 30], 'LATER', 10, 0xfffff5),
                  ([5, 0, 10, 3, 20], 'EARLIER', 10, 2), ([5, 0, 10, 3, 20], 'LATER', 4, 1), ([0, 10, 20], 'EARLIER', 100, 5),
                  ([0, 10, 20, 30, 40, 50], 'EARLIER', 25, 30)]


def built(times):
    """a new stream of sequence 1 (%1) with notes at the given times, data1 = index"""
    L = ['call.w SEQCREATESTREAM 0001 0000 0000', 'let 1']
    for i, t in enumerate(times):
        L += [f'fill 1 0 {t & 0xff:02x} {(t >> 8) & 0xff:02x} {(t >> 16) & 0xff:02x} 90 {i:02x} 40 00 00',
              f'call.d SEQSEEK %1 0000 d:{i:08x}', 'call.w SEQINSEVT %1 @1']
    return L


@suite('sorting')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64', 'call.w SEQCREATESEQ 0000 0000']
    for times in SORT_CASES:
        L += ['echo == sort ' + ' '.join(map(str, times))] + built(times) + ['call.d SEQTEMPORALSORT %1'] + \
            walk('%1', len(times) - 1) + ['call.w SEQDESTROYSTREAM %1']
    for times, way, frm, amount in DISPLACE_CASES:
        L += [f'echo == displace {way.lower()} from {frm} by {amount:#x}: ' + ' '.join(map(str, times))] + built(times) + \
            ['call.d SEQSEEK %1 0000 d:00000002', f'call.d SEQDISPLACE{way} %1 d:{frm:08x} d:{amount:08x}',
             'call.d SEQTELL %1', 'call.d SEQSTREAMLEN %1'] + walk('%1', len(times) - 1) + ['call.w SEQDESTROYSTREAM %1']
    # SeqDeleteRange(stream, first, end) with indices; ENG_ORDR's stream has 19 events
    L += ['call.w SEQDESTROYSEQ 0001']
    for first, end in [(2, 2), (2, 3), (5, 3), (0x11, 0x19), (0x12, 0x12), (0x13, 0x13), (0x13, 0x14), (0x78, 0x1e0),
                       (0, 0x12), (0, 0x13), (0x14, 0x16)]:
        L += [f'echo == delete range {first:#x} to {end:#x}', '@load ENG_ORDR.WRK', 'call.d SEQSEEK 0100 0000 d:00000004',
              f'call.w SEQDELETERANGE 0100 d:{first:08x} d:{end:08x}', 'call.d SEQSTREAMLEN 0100', 'call.d SEQTELL 0100',
              'call.d SEQSEEK 0100 0000 d:00000002', 'fill 0 0 ee ee ee ee ee ee ee ee', 'call.w SEQGETEVT 0100 @0',
              'dump 0 0 8', 'call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


@suite('streams')
def _():
    L = ['@open', 'buf 0 64', '@load ENG_ORDR.WRK',
         'echo == create', 'call.w SEQCREATESTREAM 0001 0000 0000', 'let 1', 'call.w SEQENUMSTREAMS 0001 cb4 d:00000000',
         'echo == adopt', 'call.w SEQADOPTSTREAM 0001 %1', 'let 2', 'call.w SEQENUMSTREAMS 0001 cb4 d:00000000',
         'call.d SEQSTREAMLEN %2'] + [f'call.d SEQGETSTREAMPARAM %2 {p:04x}' for p in range(16)]
    L += ['echo == name', 'call.w SEQSETSTREAMNAME %2 "New stream"', 'fill 0 0 ' + 'ee ' * 32,
          'call.w SEQGETSTREAMNAME %2 @0 0081', 'dump 0 0 32',
          'echo == clone with flag 0', 'call.w SEQCLONESTREAM 0001 0100 0000', 'let 3', 'call.d SEQSTREAMLEN %3'] + \
        [f'call.d SEQGETSTREAMPARAM %3 {p:04x}' for p in range(16)] + \
        ['echo == clone with flag 1', 'call.w SEQCLONESTREAM 0001 0100 0001', 'let 4', 'call.d SEQSTREAMLEN %4',
         'call.w SEQENUMSTREAMS 0001 cb4 d:00000000',
         'echo == destroy', 'call.w SEQDESTROYSTREAM %3', 'call.w SEQENUMSTREAMS 0001 cb4 d:00000000']
    L += ['echo == enumerate the streams of each fixture']
    for song in STREAMS:
        L += [f'@load {song}.WRK', 'call.w SEQENUMSTREAMS 0001 cb4 d:00000000', 'call.w SEQDESTROYSEQ 0001']
    L += ['call.w SEQDESTROYSEQ 0001', '@close']
    return L



TEMPO_SONGS = ['ENG_TMPO', 'SYN_MAPS', 'ENG_ORDR', 'FURELISE', 'E3_DENS']
TICKS = [0, 1, 0x77, 0x78, 0xef, 0xf0, 0x1df, 0x1e0, 0x1e1, 0x3c0, 0x5a0, 0x777, 0x1000, 0x2345, 0x10000]


def tempomap(seq='0001', n=8):
    L = ['call.w SEQGETTEMPOMAPLEN ' + seq]
    for i in range(n):
        L += ['fill 0 0 ee ee ee ee', f'call.w SEQGETTEMPOATIX {seq} {i:04x} @0', 'dump 0 0 4']
    L += [f'call.w SEQENUMTEMPOS {seq} d:00000000 d:ffffffff cb4 d:00000000']
    return L


@suite('tempo')
def _():
    L = ['@open', 'buf 0 64', 'call.w SEQGETMAXPPQ', 'call.w SEQGETMINTEMPO', 'call.w SEQGETMAXTEMPO']
    for song in TEMPO_SONGS:
        L += [f'echo -- {song}', f'@load {song}.WRK', 'call.w SEQGETPPQ 0001'] + tempomap()
        for t in TICKS:
            L += [f'call.d SEQGETTEMPO 0001 d:{t:08x}', f'call.w SEQTEMPOIXBYTICK 0001 d:{t:08x}']
        L += ['call.w SEQDESTROYSEQ 0001']
    L += ['echo == edits on ENG_TMPO', '@load ENG_TMPO.WRK']
    for title, c in [('set at 0x100', 'call.w SEQSETTEMPO 0001 d:00000100 1234'),
                     ('insert at 0x200', 'call.w SEQINSTEMPO 0001 d:00000200 2345'),
                     ('insert at an existing tick 0x1e0', 'call.w SEQINSTEMPO 0001 d:000001e0 1111'),
                     ('set at 0x200', 'call.w SEQSETTEMPO 0001 d:00000200 3456'),
                     ('delete at 1', 'call.w SEQDELTEMPO 0001 d:00000001'),
                     ('delete at 0x3c0', 'call.w SEQDELTEMPO 0001 d:000003c0'),
                     ('recalc', 'call.w SEQRECALCTEMPOMAP 0001'),
                     ('clear to 50.00', 'call.w SEQCLEARTEMPOMAP 0001 1388')]:
        L += [f'echo == {title}', c] + tempomap()
    L += ['echo == ratios'] + [f'call.w SEQGETTEMPORATIO 0001 {i:04x}' for i in range(3)] + ['call.w SEQWHICHTEMPORATIO 0001']
    L += ['call.w SEQSETTEMPORATIO 0001 0001 0050', 'call.w SEQGETTEMPORATIO 0001 0001', 'call.w SEQSELECTTEMPORATIO 0001 0002',
          'call.w SEQWHICHTEMPORATIO 0001']
    for r in (0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x80, 0x100, 0x41, 0x7f, 1):
        L += ['fill 0 0 ' + 'ee ' * 12, f'call.w SEQTEMPORATIO2STR {r:04x} @0', 'dump 0 0 12']
    L += ['echo == ppq', 'call.w SEQSETPPQ 0001 00f0', 'call.w SEQGETPPQ 0001'] + tempomap() + walk('0100', 4)
    for t, a, b in [(0x1e0, 0x78, 0xf0), (0x1e0, 0xf0, 0x78), (7, 0x78, 0x30), (0x12345, 0x60, 0x1e0), (1, 0x30, 0x1e0)]:
        L += [f'call.d SEQRESCALETIME d:{t:08x} {a:04x} {b:04x}']
    L += ['echo == swap', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQSWAPTEMPOMAPS 0001 0002'] + tempomap('0001') + tempomap('0002')
    L += ['@close']
    return L


def metermap(seq='0001'):
    return ['call.w SEQGETMETERMAPLEN ' + seq] + \
        sum([['fill 0 0 ee ee ee ee', f'call.d SEQGETMETERATIX {seq} {i:04x} @0', 'dump 0 0 4'] for i in range(6)], []) + \
        [f'call.w SEQENUMMETERS {seq} 0000 ffff cb4 d:00000000']


def positions(seq='0001'):
    L = []
    for t in TICKS:
        L += [f'call.d SEQGETMETER {seq} d:{t:08x}', f'call.d SEQTICK2MEAS {seq} d:{t:08x}',
              'fill 0 0 ee ee ee ee ee ee', f'call.d SEQTICK2MBT {seq} d:{t:08x} @0 @0+2 @0+4', 'dump 0 0 6',
              'fill 1 0 ' + 'ee ' * 16, f'call.w SEQTICK2MBTSTR {seq} d:{t:08x} @1 0000', 'dump 1 0 16',
              'fill 1 0 ' + 'ee ' * 16, f'call.w SEQTICK2MBTSTR {seq} d:{t:08x} @1 0001', 'dump 1 0 16',
              f'call.d SEQTICKNEXTMEAS {seq} d:{t:08x}', f'call.d SEQTICKPREVMEAS {seq} d:{t:08x}',
              f'call.d SEQTICKNEXTBEAT {seq} d:{t:08x}', f'call.d SEQTICKPREVBEAT {seq} d:{t:08x}']
    for m, b, t in [(1, 1, 0), (2, 1, 0), (2, 3, 17), (5, 2, 119), (128, 1, 0), (3, 7, 0), (0, 1, 0), (2, 1, 500)]:
        L += [f'call.d SEQMBT2TICK {seq} {m:04x} {b:04x} {t:04x}']
    for st in ['1:1:000', '2:1:0', '3:2:010', '4', '4:3', ' 2 : 2 : 5', '9:9:999', 'x', '', '1.2.3', '0:0:0']:
        L += ['fill 0 0 ee ee ee ee ee ee', f'call.w SEQMBTSTR2MBT {seq} "{st}" @0 @0+2 @0+4', 'dump 0 0 6']
    return L


@suite('meter')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64']
    for song in ['ENG_ORDR', 'SYN_MAPS', 'E3_DENS', 'ENG_METR']:
        L += [f'echo -- {song}', f'@load {song}.WRK'] + metermap() + positions() + ['call.w SEQDESTROYSEQ 0001']
    # A meter entry is a key signature (sharps, negative for flats) and a meter word: the power of two
    # of the denominator in the high byte, the numerator in the low byte (0x0204 = 4/4) [trace of
    # Cakewalk Apprentice 1.04's Meter/Key view]. SeqDelMeter and SeqSetMeter act on the entry in
    # effect at a measure or a tick.
    L += ['echo == edits', '@load ENG_ORDR.WRK']
    for title, c in [('insert 3/4 at measure 3', 'call.w SEQINSMETER 0001 0003 0000 0203'),
                     ('insert 6/8, 2 sharps at measure 5', 'call.w SEQINSMETER 0001 0005 0002 0306'),
                     ('insert 5/4, 3 flats at measure 3 again', 'call.w SEQINSMETER 0001 0003 fffd 0205'),
                     ('set 7/4, 1 flat for the entry in effect at tick 0x20002', 'call.w SEQSETMETER 0001 d:00020002 ffff 0207'),
                     ('delete the entry in effect at measure 2', 'call.w SEQDELMETER 0001 0002'),
                     ('recalc', 'call.w SEQRECALCMETERMAP 0001'),
                     ('delete the entry in effect at measure 1', 'call.w SEQDELMETER 0001 0001'),
                     ('clear to 3/2, 4 sharps', 'call.w SEQCLEARMETERMAP 0001 0004 0103')]:
        L += [f'echo == {title}', c] + metermap() + positions()
    L += ['echo == swap', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQSWAPMETERMAPS 0001 0002'] + metermap('0001') + metermap('0002')
    # SeqSetMeter(seq, tick32, key, meter) changes the entry in effect at the tick
    L += ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    for t in (0, 0x3bf, 0x3c0, 0x68f, 0x690, 0xffffffff):
        L += [f'echo == set 7/8, 1 sharp at tick {t:#x}: entries at measures 1 (4/4), 3 (3/4) and 5 (6/8)', 'call.w SEQCREATESEQ 0000 0000',
              'call.w SEQINSMETER 0001 0003 0000 0203', 'call.w SEQINSMETER 0001 0005 0000 0306', 'call.w SEQRECALCMETERMAP 0001',
              'call.d SEQMBT2TICK 0001 0005 0001 0000', f'call.w SEQSETMETER 0001 d:{t:08x} 0001 0307'] + metermap() + \
             ['call.d SEQMBT2TICK 0001 0005 0001 0000', 'call.w SEQRECALCMETERMAP 0001', 'call.d SEQMBT2TICK 0001 0005 0001 0000',
              'call.w SEQDESTROYSEQ 0001']
    for title, c in [('delete the entry in effect at measure 4 (the one at 3)', 'call.w SEQDELMETER 0001 0004'),
                     ('delete at measure 9 (the one at 5)', 'call.w SEQDELMETER 0001 0009')]:
        L += [f'echo == {title}', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQINSMETER 0001 0003 0000 0203',
              'call.w SEQINSMETER 0001 0005 0000 0306', c] + metermap() + ['call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


FRAME_OFFSETS = {'SYN_MAPS': 0x16b93}  # the frame offsets the songs' TIMEFMT chunks set


@suite('timecode')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64']
    for song in ['ENG_TMPO', 'ENG_ORDR', 'SYN_MAPS']:
        L += [f'echo -- {song}', f'@load {song}.WRK']
        ofs = FRAME_OFFSETS.get(song, 0)
        for fps in (0x18, 0x19, 0x1d, 0x1e, 0x1f):
            L += [f'echo fps {fps:04x}', f'call.w SEQSETFPS 0001 {fps:04x}', 'call.w SEQGETFPS 0001', 'call.d SEQGETFRAMEOFS 0001']
            for t in TICKS:
                L += [f'call.d SEQTICK2FRAME 0001 d:{t:08x}', 'fill 1 0 ' + 'ee ' * 16,
                      f'call.w SEQTICK2HMSFSTR 0001 d:{t:08x} @1', 'dump 1 0 16']
            # Frames from the song's frame offset on: 1.01 answers frames before it with ticks that
            # come from an overflow (docs/reverse-engineering/ttsseq-api.md, "Time, tempo, meter").
            for f in (ofs + f for f in (0, 1, 29, 30, 59, 60, 1799, 1800, 107892, 0x12345)):
                L += [f'call.d SEQFRAME2TICKRNDOPT 0001 d:{f:08x} 0000', f'call.d SEQFRAME2TICKRNDOPT 0001 d:{f:08x} 0001',
                      'fill 0 0 ' + 'ee ' * 8, f'call.d SEQFRAME2HMSF 0001 d:{f:08x} @0 @0+2 @0+4 @0+6', 'dump 0 0 8']
            for h, m, s_, fr in [(0, 0, 0, 0), (0, 0, 1, 0), (1, 2, 3, 4), (0, 59, 59, 29), (0, 0, 0, 29), (23, 0, 0, 0)]:
                L += [f'call.d SEQHMSF2FRAME 0001 {h:04x} {m:04x} {s_:04x} {fr:04x} 0000']
            for st in ['00:00:00:00', '00:00:02:00', '01:02:03:04', '1:2:3:4', '00:00:00:29', '00:01', 'x']:
                L += ['fill 0 0 ' + 'ee ' * 8, f'call.w SEQHMSFSTR2HMSF "{st}" @0 @0+2 @0+4 @0+6', 'dump 0 0 8',
                      'fill 0 0 ' + 'ee ' * 8, f'call.w SEQHMSFSTR2MBT 0001 "{st}" @0 @0+2 @0+4', 'dump 0 0 6']
        L += ['echo offsets'] + sum([[f'call.w SEQSETFRAMEOFS 0001 d:{o:08x}', 'call.d SEQGETFRAMEOFS 0001',
                                      'call.d SEQTICK2FRAME 0001 d:000001e0', 'fill 1 0 ' + 'ee ' * 16,
                                      'call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1', 'dump 1 0 16'] for o in (0, 100, 108000)], [])
        L += ['call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


@suite('markers')
def _():
    L = ['@open', 'buf 0 64', '@load SILENT.WRK']

    def dumpm(n=8):
        R = ['call.d SEQNUMMARKERS 0001']
        for i in range(n):
            R += ['fill 0 0 ' + 'ee ' * 32, f'call.w SEQGETMARKERNAME 0001 d:{i:08x} @0 0020', 'dump 0 0 32',
                  'fill 0 0 ee ee ee ee ee ee ee ee', f'call.w SEQGETMARKERTIME 0001 d:{i:08x} @0 @0+4', 'dump 0 0 8',
                  'fill 0 0 ee ee ee ee', f'call.w SEQGETMARKERTIME 0001 d:{i:08x} @0 null', 'dump 0 0 4']
        for t in TICKS:
            R += [f'call.d SEQMARKERFORTIME 0001 d:{t:08x}']
        return R
    L += dumpm()
    for title, c in [('rename 0', 'call.w SEQSETMARKERNAME 0001 d:00000000 "Renamed"'),
                     ('retime 1', 'call.w SEQSETMARKERTIME 0001 d:00000001 d:00000123 0000'),
                     ('retime 0 with flag 1', 'call.w SEQSETMARKERTIME 0001 d:00000000 d:00000010 0001'),
                     ('insert at 0x300', 'call.w SEQINSMARKER 0001 "New" d:00000300 0000'),
                     ('insert at 0 with flag 1', 'call.w SEQINSMARKER 0001 "First" d:00000000 0001'),
                     ('insert at 0x300 again', 'call.w SEQINSMARKER 0001 "Same time" d:00000300 0000'),
                     ('sort', 'call.w SEQSORTMARKERS 0001 d:00000000'),
                     ('delete 1', 'call.w SEQDELMARKER 0001 d:00000001')]:
        L += [f'echo == {title}', c] + dumpm()
    L += ['echo == other fixtures']
    for song in STREAMS:
        L += [f'@load {song}.WRK', 'call.d SEQNUMMARKERS 0002', 'call.w SEQDESTROYSEQ 0002']
    # A locked marker keeps a frame: its tick follows a new tempo and a new frame rate
    L += ['call.w SEQDESTROYSEQ 0001', 'echo == locked markers follow the tempo and the frame rate', '@load SILENT.WRK']
    for c in ['call.w SEQSETTEMPO 0001 d:00000000 1068', 'call.w SEQSETFPS 0001 0018',
              'call.w SEQSETMARKERTIME 0001 d:00000000 d:00000b40 0001', 'call.w SEQSETTEMPO 0001 d:00000000 20d0',
              'call.w SEQSETFPS 0001 001e', 'call.w SEQSETMARKERTIME 0001 d:00000001 d:00000b40 0000',
              'call.w SEQSETTEMPO 0001 d:00000000 1068']:
        L += [c] + sum([['fill 0 0 ee ee ee ee ee ee', f'call.w SEQGETMARKERTIME 0001 d:{i:08x} @0 @0+4', 'dump 0 0 6']
                        for i in range(3)], []) + [f'call.d SEQMARKERFORTIME 0001 d:{t:08x}' for t in (0x5a0, 0xb3f, 0x1700)]
    L += ['@close']
    return L


@suite('sysex')
def _():
    L = ['@open', 'buf 0 512', 'buf 1 128', 'put 1 0 "F0 7E 7F 09 01 F7"', 'put 1 64 "f0 43 10 4c 00 00 7e 00 f7"']

    def banks(seq='0001'):
        R = [f'call.w SEQENUMSYSXBANKS {seq} cb4 d:00000000']
        return R

    def bank(b):
        R = [f'call.d SEQGETSYSXBANKLEN {b}', 'fill 0 0 ' + 'ee ' * 32, f'call.w SEQGETSYSXBANKNAME {b} @0 0020', 'dump 0 0 32']
        R += [f'call.d SEQGETSYSXBANKPARAM {b} {p:04x}' for p in range(6)]
        # the last argument is the number of bytes per line (0 divides by zero in GetSysxAscLen)
        R += [f'call.w SEQGETSYSXASCLEN {b} {n:04x}' for n in (1, 2, 8, 16)]
        for n in (1, 2, 8, 16):
            R += ['fill 0 0 ' + 'ee ' * 64, f'call.w SEQSYSXBIN2ASC {b} @0 {n:04x}', 'dump 0 0 64']
        return R
    for song, n in [('ENG_SYSX', 2), ('SYN_KIND', 1), ('ENG_ORDR', 0)]:
        L += [f'echo -- {song}', f'@load {song}.WRK'] + banks()
        L += sum([bank(f'{0x100 + i:04x}') for i in range(n)], [])
        L += ['call.w SEQDESTROYSEQ 0001']
    L += ['echo == edits', '@load ENG_SYSX.WRK', 'call.w SEQCREATESYSXBANK 0001', 'let 1'] + banks() + bank('%1')
    L += ['call.w SEQSETSYSXBANKNAME %1 "Probe bank"'] + bank('%1')
    for p in range(6):
        L += [f'call.w SEQSETSYSXBANKPARAM %1 {p:04x} d:00000001', f'call.d SEQGETSYSXBANKPARAM %1 {p:04x}']
    L += ['call.w SEQSAVESYSXBANK 0100 "C:\\PROBE\\ORC\\OUT\\B0.SYX"', 'call.w SEQLOADSYSXBANK %1 "C:\\PROBE\\ORC\\OUT\\B0.SYX"'] + bank('%1')
    L += ['call.w SEQEMPTYSYSXBANK %1'] + bank('%1')
    # the widths of the parameters, and the name into short buffers
    for p in (1, 2, 3):
        for v in ('000001ff', 'ffffffff', '00012345', '00000080'):
            L += [f'call.w SEQSETSYSXBANKPARAM %1 {p:04x} d:{v}', f'call.d SEQGETSYSXBANKPARAM %1 {p:04x}']
    for n in (1, 2, 5, 10, 11):
        L += ['fill 0 0 ' + 'ee ' * 16, f'call.w SEQGETSYSXBANKNAME %1 @0 {n:04x}', 'dump 0 0 16']
    L += ['call.w SEQDESTROYSYSXBANK %1'] + banks()
    L += ['@close']
    return L


FILE_SONGS = sorted(STREAMS)


@suite('files')
def _():
    L = ['@open']
    for song in FILE_SONGS:
        L += [f'echo -- {song}', f'@load {song}.WRK', 'call.w SEQGETFILEERROR', f'@save 0001 {song[:8]}.WRK',
              'call.w SEQGETFILEERROR', f'@savemid 0001 {song[:8]}.MID', 'call.w SEQGETFILEERROR', 'call.w SEQDESTROYSEQ 0001',
              f'call.w SEQLOADMIDIFILE "C:\\PROBE\\ORC\\OUT\\{song[:8]}.MID" 0000', 'let 1', 'call.w SEQGETFILEERROR',
              f'@save %1 {song[:6]}_M.WRK', 'call.w SEQDESTROYSEQ %1']
    L += ['echo == errors', '@load NOFILE.WRK', 'call.w SEQGETFILEERROR', 'call.w SEQLOADMIDIFILE "C:\\PROBE\\ORC\\NOFILE.MID" 0000',
          'call.w SEQGETFILEERROR', 'call.w SEQCREATESEQ 0000 0000', '@save 0001 EMPTY.WRK', '@savemid 0001 EMPTY.MID', '@close']
    return L



# MIDI files beyond the corpus round trips of the files suite: what found the rules of
# src/smf_write.cpp and src/smf_read.cpp, and the metronome note the sequences share.
def midi_stream(keyplus, notes):
    """a new stream of sequence 1 (%1) with key+ and the given (status, key, velocity) events 10 ticks apart"""
    L = ['call.w SEQCREATESTREAM 0001 0000 0000', 'let 1', f'call.w SEQSETSTREAMPARAM %1 0000 d:{keyplus & 0xffffffff:08x}']
    for i, (st, k, v) in enumerate(notes):
        L += [f'fill 1 0 {i * 10 & 0xff:02x} {i * 10 >> 8:02x} 00 {st:02x} {k:02x} {v:02x} {5 if st >> 4 == 9 else 0:02x} 00',
              f'call.d SEQSEEK %1 0000 d:{i:08x}', 'call.w SEQINSEVT %1 @1']
    return L


@suite('midifiles')
def _():
    L = ['@open', 'buf 1 64']
    L += ['echo == the metronome note (parameters 17 to 21) is one for all sequences',
          'call.w SEQCREATESEQ 0000 0000'] + [f'call.d SEQGETSEQPARAM 0001 {p:04x}' for p in range(0x11, 0x16)]
    L += ['@save 0001 MET0.WRK', '@load SYN_MAPS.WRK', 'call.d SEQGETSEQPARAM 0002 0013', '@save 0002 MET1.WRK',
          'call.d SEQSETSEQPARAM 0001 0013 d:00000033', 'call.d SEQSETSEQPARAM 0001 0015 d:00000007',
          'call.d SEQGETSEQPARAM 0002 0013', 'call.d SEQGETSEQPARAM 0002 0015', '@save 0002 MET2.WRK',
          'call.w SEQCREATESEQ 0000 0000', 'call.d SEQGETSEQPARAM 0003 0013', '@save 0003 MET3.WRK']
    L += ['echo == a .WRK file sets it when no other sequence is current (after any destroy, or none created)',
          'call.w SEQDESTROYSEQ 0003', 'call.w SEQDESTROYSEQ 0001', '@load SYN_MAPS.WRK',
          'call.d SEQGETSEQPARAM 0001 0013', 'call.d SEQGETSEQPARAM 0002 0013', 'call.w SEQDESTROYSEQ 0002',
          '@load E3_MB.WRK', 'call.d SEQGETSEQPARAM 0001 0013', 'call.d SEQGETSEQPARAM 0001 0015',
          'call.w SEQDESTROYSEQ 0001', 'call.w SEQDESTROYSEQ 0002', 'call.w SEQCREATESEQ 0000 0000',
          '@load SYN_MAPS.WRK', 'call.d SEQGETSEQPARAM 0002 0013', 'call.d SEQGETSEQPARAM 0002 0015',
          'call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    keys = [0, 1, 5, 7, 11, 12, 13, 20, 60, 100, 110, 115, 116, 117, 118, 120, 121, 125, 126, 127]
    L += ['echo == key+ past the MIDI range: KEYFOLD.MID', 'call.w SEQCREATESEQ 0000 0000']
    for kp in (12, -12, 30, -30, 100, -100):
        L += midi_stream(kp, [(0x90, k, 0x40) for k in keys])
    # below 0 the key is the next event's second data byte modulo 12, 0 after the last event
    L += midi_stream(-12, [(0x90, 0, 0x64), (0x90, 5, 0x14), (0x90, 11, 0x64), (0x90, 2, 0x40), (0xb0, 7, 0x33),
                           (0x90, 3, 0x40), (0xc0, 9, 0x2d), (0x90, 4, 0x40), (0x90, 2, 0x7f)])
    L += ['@savemid 0001 KEYFOLD.MID', 'call.w SEQDESTROYSEQ 0001']
    L += ['echo == the event the writer and the reader share: what a program change read keeps',
          '@load SYN_KIND.WRK', '@savemid 0001 SCR0.MID', 'call.w SEQDESTROYSEQ 0001',
          '# the last event the writer took: Forced ch6 from 9 (99 4c 61, duration 120)', '@load ENG_PARM.WRK', '@savemid 0001 SCR1.MID',
          'call.w SEQLOADMIDIFILE "C:\\PROBE\\ORC\\OUT\\SCR0.MID" 0000', '@save 0002 SCRA.WRK', 'call.w SEQDESTROYSEQ 0002',
          '# the last message the reader took: a note off', 'call.w SEQLOADMIDIFILE "C:\\PROBE\\ORC\\OUT\\SCR0.MID" 0000',
          '@save 0002 SCRB.WRK', 'call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001',
          '# Stream 6 none (bc 0a 32)', '@load ENG_MULT.WRK', '@savemid 0001 SCR2.MID',
          'call.w SEQLOADMIDIFILE "C:\\PROBE\\ORC\\OUT\\SCR0.MID" 0000', '@save 0002 SCRC.WRK', '@close']
    return L


PLAY_SONGS = ['E2_CHAS', 'E2_KORD', 'E2_METR', 'E2_OVLP', 'E2_SPP', 'E2_STOP', 'E3_DENS', 'E3_MA', 'E3_MB', 'E3_MC',
              'E3_MD', 'E3_ME', 'E3_MF', 'E3_SHTL', 'E3_SPPD', 'ENG_CHAS', 'ENG_LOOP', 'ENG_METR', 'ENG_MULT', 'ENG_ORDR',
              'ENG_PARM', 'ENG_SYNC', 'ENG_SYSX', 'ENG_TMPO', 'FURELISE']
# E3_SHTL loops for ever; its playback is cut off between two notes, so that the timer's few
# milliseconds of drift under Wine cannot decide whether one more note goes out before the close.
PLAY_MS = {'E3_DENS': 20000, 'FURELISE': 30000, 'E3_SHTL': 11870}


# Songs whose playback needs the timer to run while the start call waits in a loop (the locate
# delay after the SPP): with interrupt-time callbacks, as on Windows 3.1 ("#! irq").
PLAY_IRQ = {'E3_SPPD'}


def make_play(song):
    def fn():
        ms = PLAY_MS.get(song, 12000)
        return [f'#! secs {ms // 1000 + 20}'] + (['#! irq'] if song in PLAY_IRQ else []) + ['@open', f'@load {song}.WRK', 'call.w SEQSETPLAYINGSEQ 0001',
                'call.w SEQSETSTANDBYSEQ 0000', 'call.d SEQGETNOW 0001', 'sleep 2500', '@play', f'idle {ms}', '@close']
    return fn


for _s in PLAY_SONGS:
    SUITES['play_' + _s] = make_play(_s)


@suite('transport')
def _():
    return ['#! secs 60', '@open', '@load ENG_ORDR.WRK', 'call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000',
            'call.d SEQGETNOW 0001', 'call.d SEQGETEND 0001', 'call.d SEQGETDUR 0001',
            'echo == set now', 'call.d SEQSETNOW 0001 d:000001e0', 'call.d SEQGETNOW 0001',
            'call.d SEQSETDUR 0001 d:00000100', 'call.d SEQGETDUR 0001', 'call.d SEQSETDUR 0001 d:00000000',
            'echo == play from 0x1e0 for 1.5 s, stop', 'sleep 1000', '@play', 'idle 1500', '@stop', 'idle 1000',
            'echo == continue to the end', '@play', 'idle 6000',
            'echo == rewind, play from 0 for 1.75 s (between two notes), stop', 'call.d SEQREWIND 0001', 'call.d SEQGETNOW 0001', 'sleep 1000', '@play', 'idle 1750',
            '@stop', 'idle 1000', '@close']


# Tempo and meter copy and paste. The clipboard is a sequence of its own, as in Cakewalk
# Apprentice 1.04. Layouts, found through the probe:
#   SeqTempoCopy(src, clip, from32, thru32, d ?, &flag)  SeqTempoPaste(clip, dst, at32, length32, reps, slide)
#   SeqMeterCopy(src, clip, from, thru, cut, ?, &flag)    SeqMeterPaste(clip, dst, measure, length, reps, slide)
# with the meter's measures counted from 1. The maps are read by index (up to 14 entries) and enumerated.
def tempomap_all(seq, enum=True):
    L = ['call.w SEQGETTEMPOMAPLEN ' + seq]
    for i in range(14):
        L += ['fill 0 0 ee ee ee ee', f'call.w SEQGETTEMPOATIX {seq} {i:04x} @0', 'dump 0 0 4']
    return L + ([f'call.w SEQENUMTEMPOS {seq} d:00000000 d:ffffffff cb4 d:00000000'] if enum else [])


def metermap_all(seq):
    L = ['call.w SEQGETMETERMAPLEN ' + seq]
    for i in range(14):
        L += ['fill 0 0 ee ee ee ee', f'call.d SEQGETMETERATIX {seq} {i:04x} @0', 'dump 0 0 4']
    return L + [f'call.w SEQENUMMETERS {seq} 0000 ffff cb4 d:00000000']


TEMPO_CLIPS = {'c1': [(0, 5000)], 'c2': [(0, 5000), (0x40, 6000)], 'c3': [(0x10, 7000), (0x80, 8000)]}
TEMPO_DSTS = {'d1': [], 'd3': [(0x100, 11000), (0x200, 12000)], 'd4': [(0x80, 11000), (0x180, 12000), (0x400, 13000)]}
METER_CLIPS = {'c1': [(1, 3)], 'c2': [(1, 3), (2, 5)], 'c3': [(1, 3), (3, 5), (4, 6)], 'c3b': [(2, 3), (3, 5)]}
METER_DSTS = {'d1': [], 'd3': [(3, 7), (6, 2)], 'd4': [(2, 7), (4, 2), (9, 3)]}


@suite('copypaste')
def _():
    L = ['#! secs 300', '@open', 'buf 0 64', 'buf 1 64']
    # tempo copy: ENG_TMPO (0 120.00, 0x1e0 60.00, 0x3c0 240.00, 0x5dc 90.50) into a new sequence
    for f, t, v in [(0, 0x780, 0), (0x100, 0x400, 0), (0x1e0, 0x3c0, 0), (0x200, 0x300, 0), (0x1e0, 0x1e0, 0), (0, 0x780, 1),
                    (0x5dc, 0x5dc, 0)]:
        L += [f'echo == tempo copy {f:#x}..{t:#x}, word {v}', '@load ENG_TMPO.WRK', 'call.w SEQCREATESEQ 0000 0000',
              'fill 1 0 ' + 'ee ' * 8, f'call.w SEQTEMPOCOPY 0001 0002 d:{f:08x} d:{t:08x} d:{v:08x} @1', 'dump 1 0 8'] + \
            tempomap_all('0001') + tempomap_all('0002') + ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    # tempo paste: built clips and destinations. The enumeration of a map that ends up with two
    # entries at tick 0 (a paste at 0) is not understood; those maps are read by index only.
    for cn, clip in TEMPO_CLIPS.items():
        for dn, dst in TEMPO_DSTS.items():
            for at, ln, reps, slide in [(0, 1, 1, 0), (0x80, 1, 1, 0), (0x100, 0x100, 1, 0), (0x100, 0x100, 1, 1), (0x100, 0x80, 2, 0),
                                        (0x100, 0x80, 2, 1), (0x180, 0, 1, 0), (0x100, 0x40, 3, 0), (0, 0x100, 1, 1), (0x100, 0x100, 0, 0)]:
                L += [f'echo == tempo paste {cn} into {dn} at {at:#x}, length {ln:#x}, {reps} times, slide {slide}',
                      'call.w SEQCREATESEQ 0000 0000', 'call.w SEQCREATESEQ 0000 0000'] + \
                    [f'call.w SEQINSTEMPO 0001 d:{t:08x} {p:04x}' for t, p in dst] + ['call.w SEQRECALCTEMPOMAP 0001'] + \
                    [f'call.w SEQINSTEMPO 0002 d:{t:08x} {p:04x}' for t, p in clip] + ['call.w SEQRECALCTEMPOMAP 0002'] + \
                    [f'call.w SEQTEMPOPASTE 0002 0001 d:{at:08x} d:{ln:08x} {reps:04x} {slide:04x}'] + \
                    tempomap_all('0001', enum=at != 0) + ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    # meter copy: SYN_MAPS (4/4, 3/4 2 sharps, 6/8 2 sharps, 2/2 3 flats, 5/4 3 flats, at measures 1 to 5,
    # and 4/4 7 sharps at 7) into a new sequence; the fifth word cuts
    for f, t, cut in [(2, 4, 0), (1, 7, 0), (3, 3, 0), (2, 4, 1), (3, 6, 0), (6, 6, 0), (6, 7, 0), (5, 9, 0), (8, 9, 0), (0, 2, 0),
                      (2, 0, 0), (4, 2, 0)]:
        L += [f'echo == meter copy measures {f}..{t}, cut {cut}', '@load SYN_MAPS.WRK', 'call.w SEQCREATESEQ 0000 0000',
              'fill 1 0 ' + 'ee ' * 8, f'call.w SEQMETERCOPY 0001 0002 {f:04x} {t:04x} {cut:04x} 0000 @1', 'dump 1 0 8'] + \
            metermap_all('0001') + metermap_all('0002') + ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    # meter paste: built clips (key = measure, 2/n with n the given numerator) and destinations
    for cn, clip in METER_CLIPS.items():
        for dn, dst in METER_DSTS.items():
            for m, ln, reps, slide in [(1, 0, 1, 0), (2, 0, 1, 0), (5, 0, 1, 0), (5, 2, 1, 1), (5, 2, 2, 0), (5, 2, 2, 1), (3, 4, 1, 0),
                                       (3, 4, 1, 1), (5, 0, 0, 0)]:
                L += [f'echo == meter paste {cn} into {dn} at measure {m}, length {ln}, {reps} times, slide {slide}',
                      'call.w SEQCREATESEQ 0000 0000', 'call.w SEQCREATESEQ 0000 0000'] + \
                    [f'call.w SEQINSMETER 0001 {x:04x} {x:04x} 02{n:02x}' for x, n in dst] + ['call.w SEQRECALCMETERMAP 0001'] + \
                    [f'call.w SEQINSMETER 0002 {x:04x} {x:04x} 02{n:02x}' for x, n in clip] + ['call.w SEQRECALCMETERMAP 0002'] + \
                    [f'call.w SEQMETERPASTE 0002 0001 {m:04x} {ln:04x} {reps:04x} {slide:04x}'] + \
                    metermap_all('0001') + ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    # what Cakewalk Apprentice 1.04 does: copy 1:1:000..4:4:119 with tempo and meter, paste at 5:1:000
    L += ['echo == Apprentice: copy measures 1..4 of SYN_MAPS, paste at measure 5 (0x780)', '@load SYN_MAPS.WRK',
          'call.w SEQCREATESEQ 0000 0000', 'call.w SEQTEMPOCOPY 0001 0002 d:00000000 d:0000077f d:00000000 @1',
          'call.w SEQMETERCOPY 0001 0002 0001 0004 0000 0000 @1', 'call.w SEQTEMPOPASTE 0002 0001 d:00000780 d:00000780 0001 0000',
          'call.w SEQMETERPASTE 0002 0001 0005 0004 0001 0000'] + tempomap_all('0001') + metermap_all('0001') + \
         ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    # The enumerations start at the first entry whatever `from` says; how many entries they give
    # follows from the entries in effect at `from` and `thru` (ttsseq-api.md). Two ranges that do
    # not fit that rule (ticks 0x300..0x300, measures 6..6) are left out.
    L += ['echo == enumeration ranges on SYN_MAPS', '@load SYN_MAPS.WRK']
    for f, t in [(0, 1), (0, 2), (0, 3), (0, 5), (0, 6), (0, 7), (0, 8), (5, 0xffff), (7, 0xffff), (8, 0xffff)]:
        L += [f'call.w SEQENUMMETERS 0001 {f:04x} {t:04x} cb4 d:00050007']
    for f, t in [(0, 0), (0, 0x1df), (0, 0x1e0), (0x1e1, 0xffffffff), (0x5a1, 0xffffffff), (0x5a2, 0xffffffff), (0x1000, 0xffffffff)]:
        L += [f'call.w SEQENUMTEMPOS 0001 d:{f:08x} d:{t:08x} cb4 d:00050007']
    L += ['call.w SEQENUMSTREAMS 0001 cb4 d:00050007', 'call.w SEQDESTROYSEQ 0001', '@close']
    return L


@suite('record')
def _():
    # input script IN3.TXT: times from the moment the engine connects to the input (SeqOpen). The
    # ticks of what comes in after the count-in depend on when Wine delivers it: the script runner
    # lets them differ by 8 ticks (40 ms at 100 bpm and 120 ppq).
    L = ['#! input IN3.TXT', '#! secs 40', '#! slack 8', '@open', 'call.w SEQCREATESEQ 0000 0000',
         'call.w SEQSETSEQPARAM 0001 0010 d:00000001', 'sleep 800',
         'call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000', 'cbret 0', 'cbyield 1']
    L += ['?101,210 call.w SEQRECORDSTART 0000 0000 0000 0000 0000 cb3',
          '?300 call.w SEQRECORD 0001', '?300 call.w SEQSTART cb3 0000 0000', '?300 call.w SEQGETRECORDBUFFER',
          '?300 call.w SEQSETRECORDBUFFERPARAMS 0000 0000 0000',
          'idle 2500',
          '?101,210 call.w SEQRECORDSTOP', '?101,210 call.d SEQRECCOUNT', 'buf 1 16',
          '?101,210 call.w SEQRECORDCLEANUP 0001 0001 0000 @1', '?101,210 dump 1 0 2',
          '?300 call.w SEQGETRECORDBUFFER', '?300 call.w SEQRECORD 0000', '?300 call.w SEQSTOP',
          'call.d SEQSTREAMLEN 0100', 'buf 0 16'] + walk('0100', 10) + ['call.d SEQGETSEQPARAM 0001 0003', '@close']
    return L



@suite('misc')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64',
         'echo == ports', 'fill 0 0 ' + 'ee ' * 32, 'call.w SEQGETPORTNAME 0000 @0', 'dump 0 0 32',
         'echo == memory', 'fill 1 0 01 02 03 04 05 06 07 08 09', 'call.d SEQHUGEMEMCPY @0 @1 d:00000009', 'dump 0 0 12',
         'echo == clock source', '?101 call.w SEQGETSRCCLOCK', '?!101 call.w SEQGETSRCCLOCK 0001', '?101 call.w SEQSETSRCCLOCK 0000', '?!101 call.w SEQSETSRCCLOCK 0001 0000',
         '?101 call.w SEQGETSRCCLOCK', '?!101 call.w SEQGETSRCCLOCK 0001', 'call.w SEQEXPIRED',
         'echo == meter index', '@load SYN_MAPS.WRK'] + [f'call.d SEQMETERIXBYTICK 0001 d:{t:08x}' for t in TICKS]
    L += ['call.w SEQDESTROYSEQ 0001',
          'echo == event kinds', 'cbptr 1 3',
          'call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000011', 'call.w SEQREGISTEREVENTKIND "MCICmd" cb4 d:00000022',
          'call.w SEQREGISTEREVENTKIND "Wave" cb4 d:00000033', 'call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000044',
          '@load SYN_KIND.WRK', '@load ENG_ORDR.WRK',
          'call.w SEQUNREGISTEREVENTKIND "MCICmd"', 'call.w SEQREGISTEREVENTKIND "Other" cb4 d:00000055',
          'fill 0 0 48 65 6c 6c 6f 00', 'call.w SEQCALLEVENTPROC 0001 @0 0005',
          'call.w SEQUNREGISTEREVENTKIND "Text"', 'call.w SEQUNREGISTEREVENTKIND "Wave"', 'call.w SEQUNREGISTEREVENTKIND "Other"',
          'call.w SEQUNREGISTEREVENTKIND "Text"', '@load SYN_KIND.WRK', 'cbptr -1', 'call.w SEQDESTROYSEQ 0001', 'call.w SEQDESTROYSEQ 0002',
          'echo == step record (calls as far as known; the events come from MIDI input)', '@load ENG_ORDR.WRK',
          'call.d SEQRECCOUNT', 'call.w SEQSTEPRECSETUP 0001 0100', 'call.d SEQRECCOUNT', 'call.w SEQSTEPRECPARAMS 0001 0002 0003 0004',
          'fill 0 0 00 00 00 90 3c 64 78 00', 'call.w SEQRECORDEVENT @0', 'call.d SEQRECCOUNT',
          'call.w SEQSETRECCOUNT d:00000005', 'call.d SEQRECCOUNT', 'call.w SEQSTEPRECTERM', 'call.d SEQRECCOUNT',
          'call.w SEQENUMSTREAMS 0001 cb4 d:00000000', 'call.w SEQDESTROYSEQ 0001',
          'echo == send a sysex bank', '@load ENG_SYSX.WRK', 'sleep 300', 'call.w SEQSENDSYSXBANK 0100 0000 0000', 'sleep 500',
          'call.w SEQSENDSYSXBANK 0101 0000 0000', 'sleep 500', '@close']
    return L


def merge(seq, a, seqb, b, flag, offset, count='ffffffff'):
    """SeqMergeStreams in the version's layout: 3.00 adds the number of events of b to take"""
    return [f'?101,210 call.w SEQMERGESTREAMS {seq} {a} {seqb} {b} {flag} d:{offset}',
            f'?300 call.w SEQMERGESTREAMS {seq} {a} {seqb} {b} {flag} d:{count} d:{offset}']


@suite('merge')
def _():
    # stream a: the first two events of ENG_ORDR's stream (a clone in sequence 2, trimmed); stream b:
    # ENG_ORDR's stream itself. The merged stream is new; a is gone, b too when the flag is set.
    L = ['@open', 'buf 0 64']

    def setup(trim=True):
        R = ['@load ENG_ORDR.WRK', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQCLONESTREAM 0002 0100 0000']
        if trim:
            R += ['call.w SEQDELETERANGE 0200 d:00000002 d:00000013']
        return R

    def after(seqs=('0001', '0002')):
        R = ['let 1', 'call.d SEQSTREAMLEN %1'] + walk('%1', 23)
        R += [f'call.w SEQENUMSTREAMS {s} cb4 d:00000000' for s in seqs]
        return R + ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']

    for title, args in [
        ('paste at 0x780 (Cakewalk Apprentice 1.04)', ('0002', '0200', '0001', '0100', '0000', '00000780')),
        ('offset 0', ('0002', '0200', '0001', '0100', '0000', '00000000')),
        ('offset 0x78: equal times, a first', ('0002', '0200', '0001', '0100', '0000', '00000078')),
        ('offset 0x10000', ('0002', '0200', '0001', '0100', '0000', '00010000')),
        ('flag 3 does nothing visible', ('0002', '0200', '0001', '0100', '0003', '00000000')),
        ('third word 5 does nothing visible', ('0002', '0200', '0005', '0100', '0000', '00000780')),
        ('new stream in sequence 1', ('0001', '0200', '0001', '0100', '0000', '00000780')),
        ('swapped: a is the song stream, b the clone', ('0002', '0100', '0002', '0200', '0000', '00000078')),
    ]:
        L += [f'echo == {title}'] + setup() + merge(*args) + after()
    L += ['echo == flag 1 destroys b too'] + setup() + merge('0002', '0200', '0001', '0100', '0001', '00000780') + \
        ['let 1', 'call.d SEQSTREAMLEN %1'] + walk('%1', 4) + \
        ['call.w SEQENUMSTREAMS 0001 cb4 d:00000000', 'call.w SEQENUMSTREAMS 0002 cb4 d:00000000',
         'call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    L += ['echo == the stream parameters and name of a are kept'] + setup() + \
        ['call.w SEQSETSTREAMPARAM 0200 0001 d:00000005', 'call.w SEQSETSTREAMNAME 0200 "Stream a"',
         'call.w SEQSETSTREAMPARAM 0100 0002 d:00000003', 'call.w SEQSETSTREAMNAME 0100 "Stream b"'] + \
        merge('0002', '0200', '0001', '0100', '0000', '00000780') + ['let 1'] + \
        [f'call.d SEQGETSTREAMPARAM %1 {p:04x}' for p in range(16)] + \
        ['fill 0 0 ' + 'ee ' * 32, 'call.w SEQGETSTREAMNAME %1 @0 0081', 'dump 0 0 32',
         'call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    L += ['echo == no time-base conversion: sequence 2 at 240 ppq'] + setup() + \
        ['call.w SEQSETPPQ 0002 00f0'] + merge('0002', '0200', '0001', '0100', '0000', '00000780') + after()
    L += ['echo == 3.00: the number of events taken from b']
    for n in ('00000000', '00000001', '00000007', '0000000a', '00000013', '00000030'):
        L += [f'?300 echo -- {n}'] + [f'?300 {c}' for c in setup()] + \
            [f'?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:{n} d:00000780', '?300 let 1',
             '?300 call.d SEQSTREAMLEN %1', '?300 call.w SEQDESTROYSEQ 0002', '?300 call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


@suite('midiout')
def _():
    # SeqTxMidiMsg(port, channel, status, data1, data2) sends one channel message at once; port
    # 0xffff is every port, channel 0xff every channel. The fake port records what goes out.
    L = ['@open', 'sleep 300']
    for title, msgs in [
        ('note on, note off', ['0000 0000 0090 003c 0064', '0000 0000 0080 003c 0000']),
        ('note off for a note that is not on', ['0000 0000 0080 003d 0000']),
        ('note on, note on with velocity 0, note off', ['0000 0000 0090 003d 0040', '0000 0000 0090 003d 0000', '0000 0000 0080 003d 0000']),
        ('the same note twice, then two note offs', ['0000 0000 0090 0041 0042', '0000 0000 0090 0041 0043', '0000 0000 0080 0041 0000', '0000 0000 0080 0041 0000']),
        ('the channel replaces the status nibble', ['0000 0005 0093 0040 0040', '0000 0005 0083 0040 0000']),
        ('controller, program, channel and key pressure', ['0000 0003 00b0 0007 0050', '0000 0000 00c0 0005 0000', '0000 0002 00d0 0030 0000', '0000 0002 00a0 0030 0020']),
        ('the same controller twice', ['0000 0000 00b0 0007 0050', '0000 0000 00b0 0007 0050']),
        ('pitch wheel', ['0000 0000 00e0 0000 0040', '0000 0000 00e0 0010 0050', '0000 0000 00e0 2000 0000', '0000 0000 00ef 0010 0020']),
        ('every channel', ['0000 00ff 00c0 0007 0000']),
        ('every port, channel 3', ['ffff 0003 00b0 0001 0002']),
        ('every port and channel: note on, note offs', ['ffff 00ff 0090 0031 0021', 'ffff 00ff 0080 0031 0000', 'ffff 00ff 0080 0032 0000']),
        ('port 1', ['0001 0002 0090 0030 0020', '0001 0002 0080 0030 0000']),
        ('channel 16', ['0000 0010 0090 0020 0020']),
        ('statuses below 0x80 and above 0xef', ['0000 0000 0040 0020 0020', '0000 0000 00f0 0020 0020', '0000 0000 00f8 0000 0000', '0000 0000 00ff 0020 0020']),
        ('data bytes above 0x7f', ['0000 0000 0090 0080 0040', '0000 0000 0090 0040 0080', '0000 0000 00b0 0007 00ff']),
        ('the panic of Cakewalk Apprentice 1.04 (excerpt)', ['ffff 00ff 0080 003c 0000', 'ffff 00ff 00b0 0040 0000', 'ffff 00ff 00e0 0000 0040', 'ffff 00ff 00b0 0079 0000']),
    ]:
        L += [f'echo == {title}'] + [f'call.w SEQTXMIDIMSG {m}' for m in msgs] + ['sleep 100']
    L += ['echo == a note left on is turned off at close', 'call.w SEQTXMIDIMSG 0000 0000 0090 0045 0045', 'sleep 100', '@close']
    return L


@suite('sysextext')
def _():
    # SeqSysxAsc2Bin(bank, text, &position): text to bytes, the inverse of SeqSysxBin2Asc. Returns 0,
    # or an error (the bank keeps its bytes); for a character that is not hex it writes the offset
    # of that character to the dword at position.
    L = ['@open', 'buf 0 128', 'buf 1 128', 'buf 2 8', '@load ENG_SYSX.WRK', 'call.w SEQCREATESYSXBANK 0001', 'let 1']

    def show():
        return ['call.d SEQGETSYSXBANKLEN %1', 'fill 0 0 ' + 'ee ' * 48, 'call.w SEQSYSXBIN2ASC %1 @0 0010', 'dump 0 0 48']
    for text in ['F0 7E 7F 09 01 F7', 'f0 43 10 4c 00 00 7e 00 f7', 'F0 01 F7 F0 02 03 F7', 'F07E7FF7', 'F0 1 2 F7',
                 'F0 123 F7', 'F0 7E 80 F7', 'F0 F0 F7', '  F0 01 F7  ', 'F0,01,F7', 'F0 GG F7', 'F0 7E XY 01 F7',
                 'F0 7E 7F F7 GG', 'F0 7E 7F', '7E 7F F7', '', 'F0 F7']:
        L += [f'echo == "{text}"', 'fill 2 0 ee ee ee ee ee ee ee ee', f'put 1 0 "{text}"',
              'call.w SEQSYSXASC2BIN %1 @1 @2', 'dump 2 0 8'] + show()
    L += ['echo == round trip of the banks of ENG_SYSX, 4 bytes per line']
    for b in ('0100', '0101'):
        L += ['fill 1 0 ' + 'ee ' * 64, f'call.w SEQSYSXBIN2ASC {b} @1 0004', 'dump 1 0 32', 'call.w SEQSYSXASC2BIN %1 @1 @2'] + show()
    L += ['@close']
    return L


@suite('sysexin')
def _():
    # SeqReceiveSysxBank(bank, callback, 0, 0, 0): collects incoming sysex into the bank until the
    # callback, called over and over with (bytes received, free memory) as two dwords, returns
    # non-zero. The input script sends the messages; cbretafter makes the callback say stop.
    L = ['#! input SYSX.TXT', '#! secs 45', '@open', 'buf 0 192', 'cbmask c', 'cbyield 1',
         '@load ENG_SYSX.WRK', 'call.w SEQCREATESYSXBANK 0001', 'let 1']

    def show():
        return ['call.d SEQGETSYSXBANKLEN %1', 'fill 0 0 ' + 'ee ' * 128, 'call.w SEQSYSXBIN2ASC %1 @0 0010', 'dump 0 0 128']
    L += ['echo == receive until about 8 s', 'cbret 0', 'cbretafter 7000 1',
          'call.w SEQRECEIVESYSXBANK %1 cb4 0000 0000 0000'] + show()
    L += ['echo == receive again into the same bank, until about 13 s', 'cbret 0', 'cbretafter 4500 1',
          'call.w SEQRECEIVESYSXBANK %1 cb4 0000 0000 0000'] + show()
    L += ['echo == 3.00: SeqReceiveSysxBankEx into a new bank, until about 18 s', '?300 call.w SEQCREATESYSXBANK 0001', '?300 let 1',
          '?300 cbret 0', '?300 cbretafter 4500 1', '?300 call.w SEQRECEIVESYSXBANKEX %1 cb4 0000 0000 0000 0000 0000']
    L += ['?300 ' + c for c in show()]
    L += ['echo == the callback says stop at once', 'cbret 1', 'call.w SEQRECEIVESYSXBANK %1 cb4 0000 0000 0000'] + show()
    L += ['cbmask 0', 'call.w SEQENUMSYSXBANKS 0001 cb4 d:00000000', '@close']
    return L


@suite('fileio')
def _():
    # SeqIOSeek(file, offset32, origin), SeqIORead(file, buffer, count) and SeqIOWrite(file, buffer,
    # count) (1.01 and 2.10) work on the file the engine has open while it loads or saves. The
    # program gets the file in its load or save callback, callback(code, file, value32): 0xFE at the
    # start of a load, then once per chunk with the chunk's type (the file stands at the chunk's
    # length; the callback returns 1 when it read the chunk itself, 0 to leave it to the engine), and
    # 0xFF at the end of a save, for the program's own chunks. An event procedure reads the text of a
    # string event with SeqIORead. 3.00's file functions have no callback, so this suite is 1.01 and
    # 2.10 only. Callback words 1 and 2 (file handle, pointers) differ from run to run: masked.
    text = 'Comments written by the probe, through SeqIOWrite in the save callback.'
    n = len(text)
    V = '?101,210 '
    L = ['@open', 'buf 1 128', 'buf 2 16', 'buf 3 4', 'cbmask 6', 'cbret 0']
    body = [
        'echo == load SYN_MAPS: seek at the start, read the comments chunk (8) and take it',
        'cbdo cb4 fe call.d SEQIOSEEK %w1 d:00000000 0001', 'cbdo cb4 fe call.d SEQIOSEEK %w1 d:00000000 0002',
        'cbdo cb4 fe call.d SEQIOSEEK %w1 d:0000000b 0000',
        'cbdo cb4 08 call.d SEQIOSEEK %w1 d:00000000 0001', 'cbdo cb4 08 fill 1 0 ' + 'ee ' * 96,
        'cbdo cb4 08 call.w SEQIOREAD %w1 @1 0004', 'cbdo cb4 08 call.w SEQIOREAD %w1 @1+4 0002',
        'cbdo cb4 08 call.w SEQIOREAD %w1 @1+6 %b1+4', 'cbdo cb4 08 dump 1 0 96', 'cbdo cb4 08 ret 1',
        'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\SYN_MAPS.WRK" 0000 cb4 d:00001234', 'cbdo -',
        'call.w SEQGETMETERMAPLEN 0001',
        f'echo == save it with a comments chunk of its own, written at the end (0xFF)',
        'fill 2 0 08', f'fill 2 1 {n + 2:02x} 00 00 00', f'fill 3 0 {n:02x} 00', f'put 1 0 "{text}"',
        'cbret 1', 'cbdo cb4 ff call.d SEQIOSEEK %w1 d:00000000 0001',
        'cbdo cb4 ff call.w SEQIOWRITE %w1 @2 0001', 'cbdo cb4 ff call.w SEQIOWRITE %w1 @2+1 0004',
        'cbdo cb4 ff call.w SEQIOWRITE %w1 @3 0002', f'cbdo cb4 ff call.w SEQIOWRITE %w1 @1 {n:04x}',
        'cbdo cb4 ff call.d SEQIOSEEK %w1 d:00000000 0001',
        'call.w SEQSAVECAKEWALKFILE 0001 "C:\\PROBE\\ORC\\OUT\\IOSAVE.WRK" d:00000000 cb4 d:00005678', 'cbdo -',
        'cbret 0', 'call.w SEQDESTROYSEQ 0001',
        'echo == load the saved file and read its comments',
        'cbdo cb4 08 fill 1 0 ' + 'ee ' * 96, 'cbdo cb4 08 call.w SEQIOREAD %w1 @1 0004',
        'cbdo cb4 08 call.w SEQIOREAD %w1 @1+4 0002', 'cbdo cb4 08 call.w SEQIOREAD %w1 @1+6 %b1+4',
        'cbdo cb4 08 dump 1 0 96', 'cbdo cb4 08 ret 1',
        'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\OUT\\IOSAVE.WRK" 0000 cb4 d:00001234', 'cbdo -',
        'call.w SEQDESTROYSEQ 0001',
        'echo == load SYN_MAPS leaving every chunk to the engine',
        'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\SYN_MAPS.WRK" 0000 cb4 d:00001234',
        'call.w SEQGETMETERMAPLEN 0001', 'call.w SEQDESTROYSEQ 0001',
        'echo == string events: the event procedure reads each text with the file of the load callback',
        'call.w SEQREGISTEREVENTKIND "MCICmd" cb4 d:00000011', 'call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000022',
        'cbdo cb4 fe letw 1 1',
        'cbdo cb4 04 fill 1 0 ' + 'ee ' * 32, 'cbdo cb4 04 call.w SEQIOREAD %1 @1 0004',
        'cbdo cb4 04 call.w SEQIOREAD %1 @1+4 %b1+0', 'cbdo cb4 04 dump 1 0 32', 'cbdo cb4 04 ret 1',
        'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\SYN_KIND.WRK" 0000 cb4 d:00001234', 'cbdo -',
        'call.w SEQENUMSTREAMS 0001 cb4 d:00000000', 'call.d SEQSTREAMLEN 0100', 'call.d SEQSTREAMLEN 0101',
        'call.w SEQDESTROYSEQ 0001',
        'call.w SEQUNREGISTEREVENTKIND "MCICmd"', 'call.w SEQUNREGISTEREVENTKIND "Text"',
    ]
    L += [V + c for c in body]
    L += ['@close']
    return L


# String events and their kinds (1.01 and 2.10; the parts with a file callback or SeqIO are not
# run on 3.00, whose file functions have none). An event procedure gets (message, far pointer to
# the event in the stream, length); the pointer is masked and its bytes dumped instead (dumpcb).
# Messages: 1 play, 3 free, 4 load, 5 save, 7 MIDI file (length 0: write?, then 7: write).
def kind_stream(seq, events):
    """a new stream of `seq` (%1) with (time, status, data) events in that order"""
    L = [f'call.w SEQCREATESTREAM {seq} 0000 0000', 'let 1']
    for i, (t, st, d) in enumerate(events):
        L += [f'fill 2 0 {t & 0xff:02x} {t >> 8 & 0xff:02x} 00 {st:02x} ' + ' '.join(f'{b:02x}' for b in d),
              f'call.d SEQSEEK %1 0000 d:{i:08x}', 'call.w SEQINSEVT %1 @2']
    return L


@suite('eventkinds')
def _():
    V = '?101,210 '
    L = ['@open', 'buf 1 128', 'buf 2 16', 'buf 3 16', 'buf 4 4', 'cbmask 6', 'cbret 0']
    L += ['echo == kinds: the lowest free number, a name may come twice',
          'call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000011', 'call.w SEQREGISTEREVENTKIND "MCICmd" cb4 d:00000022',
          'call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000033', 'call.w SEQUNREGISTEREVENTKIND "MCICmd"',
          'call.w SEQREGISTEREVENTKIND "Other" cb4 d:00000044', 'call.w SEQUNREGISTEREVENTKIND "Nothing"',
          'call.w SEQUNREGISTEREVENTKIND "Text"', 'call.w SEQUNREGISTEREVENTKIND "Text"',
          'call.w SEQUNREGISTEREVENTKIND "Other"', 'call.w SEQUNREGISTEREVENTKIND "Text"']
    L += ['echo == SeqCallEventProc(message, event, length): the procedure of the event\'s kind',
          'call.w SEQREGISTEREVENTKIND "A" cb4 d:00000011', 'call.w SEQREGISTEREVENTKIND "B" cb4 d:00000022',
          'cbdo cb4 * dumpcb 1 0 8', 'cbdo cb4 * fillcb 1 6 5a', 'cbdo cb4 * ret 77',
          'fill 1 0 05 00 00 02 aa bb cc 00', 'call.w SEQCALLEVENTPROC 0001 @1 0005', 'dump 1 0 8',
          'call.w SEQCALLEVENTPROC 0009 @1 0000', 'fill 1 0 05 00 00 01 aa bb cc 00', 'call.w SEQCALLEVENTPROC 0004 @1 0007',
          'fill 1 0 05 00 00 03 aa bb cc 00', 'call.w SEQCALLEVENTPROC 0004 @1 0007',
          'fill 1 0 05 00 00 90 3c 40 10 00', 'call.w SEQCALLEVENTPROC 0004 @1 0007', 'cbdo -']
    # The registration value is a set of flags: bit 1 keeps the free message (3) away, bit 0 the play
    # message (1); the cases below were how that showed (values 0x11, 0x22, 0x33, 0x44, ...).
    L += ['echo == freeing (3) when a stream goes: not for kinds whose value has bit 1 (2)',
          'call.w SEQREGISTEREVENTKIND "C" cb4 d:00000033', 'cbdo cb4 * dumpcb 1 0 8', 'call.w SEQCREATESEQ 0000 0000']
    L += kind_stream('0001', [(0, 1, [0x11] * 4), (0x10, 2, [0x22] * 4), (0x20, 3, [0x33] * 4), (0x30, 1, [0x44] * 4),
                              (0x40, 0x90, [0x3c, 0x40, 0x10, 0])])
    L += ['echo delete, put over and delete range free nothing', 'call.d SEQSEEK %1 0000 d:00000001', 'call.w SEQDELEVT %1',
          'fill 2 0 00 00 00 90 3c 40 10 00', 'call.d SEQSEEK %1 0000 d:00000000', 'call.w SEQPUTEVT %1 @2',
          'call.w SEQDELETERANGE %1 d:00000000 d:00000001', 'call.d SEQSTREAMLEN %1', 'call.w SEQDESTROYSTREAM %1']
    L += ['echo with A (11) gone, the others (22, 33) are not freed', 'call.w SEQUNREGISTEREVENTKIND "A"']
    L += kind_stream('0001', [(0, 1, [0x55] * 4), (0x10, 2, [0x66] * 4), (0x20, 3, [0x77] * 4)])
    L += ['call.w SEQDESTROYSTREAM %1', 'echo a new kind 1 (44), named as kind 3', 'call.w SEQREGISTEREVENTKIND "C" cb4 d:00000044']
    L += kind_stream('0001', [(0, 1, [0x55] * 4), (0x10, 2, [0x66] * 4), (0x20, 3, [0x77] * 4)])
    L += ['call.w SEQDESTROYSEQ 0001', 'call.w SEQUNREGISTEREVENTKIND "B"', 'call.w SEQUNREGISTEREVENTKIND "C"',
          'call.w SEQUNREGISTEREVENTKIND "C"', 'echo kinds 1 and 2 with 55, 3 with 66',
          'call.w SEQREGISTEREVENTKIND "A" cb4 d:00000055', 'call.w SEQREGISTEREVENTKIND "B" cb4 d:00000055',
          'call.w SEQREGISTEREVENTKIND "A" cb4 d:00000066', 'call.w SEQCREATESEQ 0000 0000']
    L += kind_stream('0001', [(0, 1, [0x55] * 4), (0x10, 2, [0x66] * 4), (0x20, 3, [0x77] * 4)])
    L += ['call.w SEQDESTROYSEQ 0001', 'call.w SEQUNREGISTEREVENTKIND "A"', 'call.w SEQUNREGISTEREVENTKIND "B"',
          'call.w SEQUNREGISTEREVENTKIND "A"']
    L += ['echo == playing (1, a copy of the event, length 0) and freeing by the value\'s bits 0 and 1']
    L += [f'call.w SEQREGISTEREVENTKIND "V{v}" cb4 d:{v:08x}' for v in (0, 1, 2, 3, 4)]
    L += ['call.w SEQCREATESEQ 0000 0000']
    L += kind_stream('0001', [(0x10 * i, i + 1, [0xa0 + i, 0, 0, 0]) for i in range(5)] + [(0x60, 0x90, [0x3c, 0x40, 8, 0])])
    L += ['call.w SEQSETSTREAMPARAM %1 0007 d:00000000', 'call.w SEQSETPLAYINGSEQ 0001', '@play',
          'idle 1500', 'call.w SEQDESTROYSEQ 0001', 'cbdo -'] + [f'call.w SEQUNREGISTEREVENTKIND "V{v}"' for v in (0, 1, 2, 3, 4)]
    L += ['echo == a MIDI file: the procedure says whether it writes (length 0) and then writes (7)',
          '@load ENG_ORDR.WRK', 'call.w SEQDESTROYSEQ 0001',
          'call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000011', 'put 3 3 "hell"', 'fill 3 0 ff 01 04',
          'call.w SEQCREATESEQ 0000 0000']
    L += kind_stream('0001', [(0, 0x90, [0x3c, 0x40, 5, 0]), (0x10, 1, [1, 0, 0, 0]), (0x18, 1, [0, 0, 0, 0]),
                              (0x20, 0x90, [0x3e, 0x40, 5, 0])])
    L += [V + 'cbdo cb4 07 dumpcb 1 0 8', V + 'cbdo cb4 07 call.w SEQIOWRITE 0000 @3 %w3', V + 'cbdo cb4 07 ret 1',
          V + '@savemid 0001 KINDS.MID', V + 'cbdo -', 'call.w SEQDESTROYSEQ 0001']
    L += ['echo == load SYN_KIND: each procedure reads its text with the file of the load callback (4)',
          'call.w SEQREGISTEREVENTKIND "MCICmd" cb4 d:00000022']
    L += [V + c for c in [
        'cbdo cb4 fe letw 1 1', 'cbdo cb4 04 dumpcb 1 0 8', 'cbdo cb4 04 fill 1 0 ' + 'ee ' * 32,
        'cbdo cb4 04 call.w SEQIOREAD %1 @1 0004', 'cbdo cb4 04 call.w SEQIOREAD %1 @1+4 %b1+0', 'cbdo cb4 04 dump 1 0 32',
        'cbdo cb4 04 fillcb 1 4 a1 a2 a3', 'cbdo cb4 04 ret 1',
        'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\SYN_KIND.WRK" 0000 cb4 d:00001234', 'cbdo -',
        'call.d SEQSTREAMLEN 0100'] +
        sum([[f'call.d SEQSEEK 0100 0000 d:{i:08x}', 'call.w SEQGETEVT 0100 @2', 'dump 2 0 8'] for i in (0, 1, 2, 15)], []) + [
        'echo == save it: each procedure writes its text (5); with kinds every stream is a chunk 18',
        'fill 4 0 03 00 00 00', 'put 3 0 "abc"', 'cbdo cb4 05 dumpcb 1 0 8', 'cbdo cb4 05 call.w SEQIOWRITE %1 @4 0004',
        'cbdo cb4 05 call.w SEQIOWRITE %1 @3 0003', 'cbdo cb4 05 ret 1', '@save 0001 KINDS1.WRK',
        'echo and with the callback: 0xFE 0, 3 after VARS, the percentage after each stream and bank, 0xFF',
        'cbret 1', 'cbdo cb4 fe letw 2 1', 'cbdo cb4 fe putw 4 0 %2', 'cbdo cb4 fe dump 4 0 2',
        'call.w SEQSAVECAKEWALKFILE 0001 "C:\\PROBE\\ORC\\OUT\\KINDS2.WRK" d:00000000 cb4 d:00005678',
        'echo a procedure returning 0 fails the save, and no file stays', 'cbdo -', 'cbret 0',
        'cbdo cb4 05 ret 0', '@save 0001 KINDS3.WRK', 'cbdo -', 'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\OUT\\KINDS3.WRK" 0000 null 0000 0000',
        'call.w SEQGETFILEERROR', 'call.w SEQDESTROYSEQ 0001',
        'echo == the callback cancels with 0 at 0xFE 0 or at 3',
        '@load ENG_MULT.WRK', 'cbdo cb4 fe letw 2 1', 'cbdo cb4 fe putw 4 0 %2', 'cbdo cb4 fe dump 4 0 2',
        'cbdo cb4 fe ret 1', 'cbret 1', 'call.w SEQSAVECAKEWALKFILE 0001 "C:\\PROBE\\ORC\\OUT\\KINDS4.WRK" d:00000000 cb4 d:00005678',
        'cbret 0', 'call.w SEQSAVECAKEWALKFILE 0001 "C:\\PROBE\\ORC\\OUT\\KINDS5.WRK" d:00000000 cb4 d:00005678',
        'cbdo -', 'call.w SEQSAVECAKEWALKFILE 0001 "C:\\PROBE\\ORC\\OUT\\KINDS6.WRK" d:00000000 cb4 d:00005678',
        'call.w SEQLOADCAKEWALKFILE "C:\\PROBE\\ORC\\OUT\\KINDS5.WRK" 0000 null 0000 0000', 'call.w SEQGETFILEERROR',
        'call.w SEQDESTROYSEQ 0001']]
    L += ['call.w SEQUNREGISTEREVENTKIND "Text"', 'call.w SEQUNREGISTEREVENTKIND "MCICmd"']
    L += ['echo == which kind a file\'s id goes to: a procedure that reads nothing leaves the engine in the text',
          'cbret 1']
    # what a failed load frees: the events of kinds without bit 1 in their value, as always
    for kinds in ([('Text', 0x11), ('Zed', 0x22), ('Yo', 0x33)], [('Zed', 0x11), ('Text', 0x11)],
                  [('Zed', 0x22), ('Text', 0x33)], [('Zed', 0x33), ('Yo', 0x22)], [('MCICmd', 0x22), ('Text', 0x11), ('Wave', 0x11)],
                  [('Wave', 0x11)], [('Text', 0x11), ('MCICmd', 0x22), ('Wave', 0x33), ('Text', 0x44)]):
        L += ['echo ' + ' '.join(f'{k} {v:x}' for k, v in kinds)]
        L += [f'call.w SEQREGISTEREVENTKIND "{k}" cb4 d:{v:08x}' for k, v in kinds]
        L += ['cbptr 1 3', '@load SYN_KIND.WRK', 'cbptr -1', 'cbdo cb4 03 dumpcb 1 0 8', 'call.w SEQDESTROYSEQ 0001', 'cbdo -']
        L += [f'call.w SEQUNREGISTEREVENTKIND "{k}"' for k, v in kinds]
    L += ['@close']
    return L


@suite('ports')
def _():
    # The port assignment that SeqConfigMidi's dialog writes to TTSSEQ.INI, given here directly:
    # two output devices in reverse order (port 1 is the fake's second port, port 2 its first) and
    # sync output to port 2. The MIDI lines name the fake port that got each message (@0, @2).
    L = ['#! outputs 2', '#! ini PORTS.INI', '#! secs 40', '@open', 'buf 0 32']
    for p in range(4):
        L += ['fill 0 0 ' + 'ee ' * 24, f'call.w SEQGETPORTNAME {p:04x} @0', 'dump 0 0 24']
    L += ['echo == a note to port 1, port 2, port 3 (none) and every port', 'sleep 300']
    for i, p in enumerate(('0000', '0001', '0002', 'ffff')):
        L += [f'call.w SEQTXMIDIMSG {p} 0000 0090 {0x3c + i:04x} 0064', f'call.w SEQTXMIDIMSG {p} 0000 0080 {0x3c + i:04x} 0000', 'sleep 100']
    L += ['echo == play ENG_SYNC (MIDI sync on in 2.10 and 3.00) for 0.6 s', '@load ENG_SYNC.WRK', 'call.w SEQSETPLAYINGSEQ 0001',
          'call.w SEQSETSTANDBYSEQ 0000', 'sleep 500', '@play', 'idle 600', '@stop', 'idle 300', '@close']
    return L


@suite('steprec')
def _():
    # Step recording as Cakewalk Apprentice 1.04's dialog does it [trace]: SeqStepRecSetup(seq, hwnd)
    # with a window for the engine's messages, SeqStepRecParams(position32, duration, velocity)
    # (velocity 0: as played). After each step the engine posts message 0x464 (wParam 0x100,
    # lParam the number of events so far); the program answers by moving to the next position.
    # SeqSetRecCount(n) keeps the first n events (Delete); SeqStepRecTerm() ends it and returns the
    # count. The events are in the stream SeqStepRecSetup made and returned; SeqMergeStreams with
    # stream 0 as the first stream makes the track from it, as Apprentice's Keep does.
    L = ['#! input STEP.TXT', '#! secs 40', '@open', 'buf 0 16', 'buf 2 16', 'win',
         'call.w SEQCREATESEQ 0000 0000', 'sleep 800', 'set 3 0',
         'call.v SEQSTEPRECPARAMS d:00000000 0077 0000', 'call.w SEQSTEPRECSETUP 0001 hwnd', 'let 7',
         'call.v SEQSTEPRECPARAMS d:00000000 0077 0000', 'set 5 77', 'set 6 0',
         'cbdo msg 0464 add 3 78', 'cbdo msg 0464 call.d SEQSETNOW 0001 %d3',
         'cbdo msg 0464 call.v SEQSTEPRECPARAMS %d3 %5 %6', 'cbdo msg 0464 call.d SEQRECCOUNT',
         'sleep 5000',
         'echo == delete the last step, then duration 0x1e0 and velocity 0x40',
         'call.d SEQRECCOUNT', 'call.w SEQSETRECCOUNT d:00000004', 'call.d SEQRECCOUNT',
         'fill 2 0 00 00 00 91 30 31 32 00', 'call.w SEQRECORDEVENT @2', 'call.d SEQRECCOUNT',
         'add 3 ffffff88', 'set 5 1e0', 'set 6 40', 'call.d SEQSETNOW 0001 %d3', 'call.v SEQSTEPRECPARAMS %d3 %5 %6',
         'sleep 4500', 'cbdo -',
         'echo == end', 'call.d SEQRECCOUNT', 'call.w SEQSTEPRECTERM', 'call.d SEQRECCOUNT', 'call.d SEQGETNOW 0001',
         '?300 call.w SEQGETRECORDBUFFER', 'call.d SEQSTREAMLEN %7'] + merge('0001', '0000', '0001', '%7', '0001', '00000000') + \
        ['let 1', 'call.d SEQSTREAMLEN %1'] + walk('%1', 12) + \
        ['call.w SEQENUMSTREAMS 0001 cb4 d:00000000', '@close']
    return L


PUNCH_STARTS = [920, 4830, 8740, 12640, 16540, 20440, 24340, 28140, 32140, 36040, 39940, 44210]  # count-in starts, measured on 2.10
PUNCH_EVENTS = [0, 1, 98, 99, 100, 101, 102, 103, 400]  # the target stream: one note per tick listed
PUNCH_TAKES = [  # (title, Now, target stream?, punch in, punch out, keep, cleanup pointer holds the target?)
    ('no target stream: the range is ignored', 100, False, 100, 102, 1, False),
    ('punch 100..102', 100, True, 100, 102, 1, False),
    ('punch 100..101', 100, True, 100, 101, 1, False),
    ('punch 101..103', 100, True, 101, 103, 1, False),
    ('punch 104..110: no event of the stream in the range', 100, True, 104, 110, 1, False),
    ('punch 100..100: an empty range', 100, True, 100, 100, 1, False),
    ('punch 98..ffffffff: also before the start', 100, True, 98, 0xffffffff, 1, False),
    ('punch 0..0 from Now 0: punch out 0 counts as 1', 0, True, 0, 0, 1, False),
    ('punch 100..102, kept into the target: one merged stream', 100, True, 100, 102, 1, True),
    ('punch 100..102, discarded with the target: the cut events come back', 100, True, 100, 102, 0, True),
    ('no punch, kept into the target: sound on sound', 100, False, 0, 0, 1, True),
    ('Song Position on: the start rounds down to a MIDI beat', 45, False, 0, 0, 1, False),
]


@suite('punch')
def _():
    # SeqRecordStart(stream, punch in, punch out, callback) (1.01, 2.10). With a stream, the start
    # call moves the stream's events with in <= time < out out of it (they do not play), and
    # SeqRecordCleanup keeps only the take's events with in <= time and time + 1 < out. When the
    # stream has no event in the range, nothing is cut and the take is kept whole. Stream 0 means
    # no punch. SeqRecordCleanup(seq, keep, 0, &stream): the word at the pointer is read as well as
    # written. 0 there: the take goes into a new stream. A stream there: keeping merges the take
    # and that stream into a new stream (the old one is gone); discarding puts the cut events back
    # into it (with 0 there, discarding a punch take crashes). Each take gets one pair of
    # controllers during its count-in, recorded at the start tick. Parameter 4 (send Song
    # Position) is off, so starts are exact; the last take turns it on to show the rounding.
    L = ['#! input PUNCH.TXT', '#! secs 120', '#! irq', '@open', 'buf 0 16', 'buf 1 16', 'buf 3 8',
         'call.w SEQCREATESEQ 0000 0000', 'call.w SEQSETSEQPARAM 0001 0010 d:00000001',
         'call.w SEQSETSEQPARAM 0001 0004 d:00000000', 'sleep 800',
         'call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000', 'cbret 0', 'cbyield 1',
         'cbmask 2 merge']  # the count-in's beats: one line per count-in (the record suite shows them)
    n = len(PUNCH_EVENTS)
    R = '?101,210 '
    for title, now, target, a, b, keep, into in PUNCH_TAKES:
        L += [f'echo == {title}', 'call.w SEQCREATESTREAM 0001 0000 0000', 'let 1',
              'call.w SEQSETSTREAMPARAM %1 0007 d:00000000']
        for i, t in enumerate(PUNCH_EVENTS):
            L += [f'fill 3 0 {t & 255:02x} {t >> 8 & 255:02x} 00 92 {0x50 + i:02x} 40 01 00',
                  f'call.d SEQSEEK %1 0000 d:{i:08x}', 'call.w SEQINSEVT %1 @3']
        if 'Song Position' in title:
            L += ['call.w SEQSETSEQPARAM 0001 0004 d:00000001']
        L += [f'call.d SEQSETNOW 0001 d:{now:08x}', 'putw 1 0 %1' if into else 'fill 1 0 00 00',
              R + f'call.w SEQRECORDSTART {"%1" if target else "0000"} d:{a:08x} d:{b:08x} cb3',
              R + 'call.d SEQGETNOW 0001', R + 'call.d SEQSTREAMLEN %1',
              R + 'call.w SEQRECORDSTOP', R + 'call.d SEQRECCOUNT',
              R + f'call.w SEQRECORDCLEANUP 0001 {keep:04x} 0000 @1', 'dump 1 0 2',
              R + 'call.w SEQENUMSTREAMS 0001 cb4 d:00000000']
        if into and keep:  # the target is gone: only the result is listed
            L += ['?300 echo stream', '?300 call.d SEQSTREAMLEN %1'] + ['?300 ' + x for x in walk('%1', n)]
            L += [R + 'echo result', R + 'call.d SEQSTREAMLEN %b1'] + [R + x for x in walk('%b1', n + 4)]
            L += [R + 'call.w SEQDESTROYSTREAM %b1', '?300 call.w SEQDESTROYSTREAM %1']
        elif into:  # discarded: the result is the target
            L += ['echo stream', 'call.d SEQSTREAMLEN %1'] + walk('%1', n)
            L += ['call.w SEQDESTROYSTREAM %1']
        else:
            L += ['echo stream', 'call.d SEQSTREAMLEN %1'] + walk('%1', n)
            L += [R + 'echo take', R + 'call.d SEQSTREAMLEN %b1'] + [R + x for x in walk('%b1', 4)]
            L += [R + 'call.w SEQDESTROYSTREAM %b1', 'call.w SEQDESTROYSTREAM %1']
        L += ['sleep 1500']
    L += ['@close']
    return L


def punch_input(starts):
    """The MIDI input of the punch suite: a pair of controllers between two clicks of each take's count-in.
    starts: when each take's count-in begins, in ms from the input connection."""
    L = ['# MIDI input for the punch suite (written by tools/ttsprobe/oracles/gen.py): times in ms from',
         '# the moment the engine connects to the input (SeqOpen), between two clicks of each count-in.']
    for k, t in enumerate(starts):
        L += [f'{t + 900} b0 07 {0x40 + k:02x}', f'{t + 910} c0 {k:02x}']
    return L


@suite('param3')
def _():
    # Sequence parameter 3: a flag (any value other than 0 reads back as 1) that the engine sets
    # whenever Now is set or jumps: creating, loading, SeqSetNow (even to the same value),
    # SeqRewind, every start (playback or recording) and the auto-shuttle's wrap. Edits, saves,
    # stopping, the end of a song and Now moving during playback leave it as it is; only the
    # program clears it. No effect of it was found. Each step below clears it first.
    P = 'call.d SEQGETSEQPARAM 0001 0003'
    Z = 'call.w SEQSETSEQPARAM 0001 0003 d:00000000'
    L = ['#! secs 40', '@open', 'buf 1 16', 'call.w SEQCREATESEQ 0000 0000', 'echo == created', P,
         'call.w SEQDESTROYSEQ 0001', '@load ENG_ORDR.WRK', 'echo == loaded', P]
    steps = [
        ('cleared', []),
        ('set to 5', ['call.w SEQSETSEQPARAM 0001 0003 d:00000005']),
        ('another parameter', ['call.w SEQSETSEQPARAM 0001 0002 d:00000001']),
        ('insert, put, delete and move events', ['call.d SEQSEEK 0100 0000 d:00000000', 'fill 1 0 78 00 00 b0 07 64 00 00',
                                                 'call.w SEQINSEVT 0100 @1', 'call.d SEQSEEK 0100 0000 d:00000002',
                                                 'call.w SEQPUTEVT 0100 @1', 'call.w SEQDELEVT 0100',
                                                 'call.d SEQSEEK 0100 0000 d:00000001', 'call.d SEQMOVEEVT 0100 d:00000300']),
        ('insert a tempo', ['call.w SEQINSTEMPO 0001 d:000001e0 2ee0']),
        ('clone and destroy a stream', ['call.w SEQCLONESTREAM 0001 0100 0000', 'let 2', 'call.w SEQDESTROYSTREAM %2']),
        ('save', ['@save 0001 P3.WRK']),
        ('set Now', ['call.d SEQSETNOW 0001 d:000000f0']),
        ('set Now to the same value', ['call.d SEQSETNOW 0001 d:000000f0']),
        ('rewind', ['call.d SEQREWIND 0001']),
        ('make it the playing sequence', ['call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000']),
        ('start', ['@play']),
        ('play on, then stop', ['idle 600', P, '@stop', 'idle 200']),
        ('record start (no count-in)', ['call.w SEQSETSEQPARAM 0001 0010 d:00000000', 'cbret 0',
                                        '?101,210 call.w SEQRECORDSTART 0000 0000 0000 0000 0000 cb3',
                                        '?300 call.w SEQRECORD 0001', '?300 call.w SEQSTART cb3 0000 0000']),
        ('record stop', ['idle 300', '?101,210 call.w SEQRECORDSTOP', '?300 call.w SEQRECORD 0000', '?300 call.w SEQSTOP']),
    ]
    for t, c in steps:
        L += [Z, f'echo == {t}'] + c + [P]
    L += ['echo == E3_SHTL: play, clear after the start, read before and after the wrap (960 to 480)',
          'call.w SEQDESTROYSEQ 0001', '@load E3_SHTL.WRK', 'call.w SEQSETPLAYINGSEQ 0001', '@play', 'idle 200', Z,
          'idle 3000', P, 'idle 1500', P, '@stop', '@close']
    return L


THRU_SLOTS = [
    # (what the slot shows, the calls at its start, its input: (ms into the slot, message))
    ('no sequence yet', [], 'notes'),
    ('the first sequence, no playing sequence set', ['call.w SEQCREATESEQ 0000 0000', 'call.w SEQCREATESEQ 0000 0000',
     'call.w SEQSETSEQPARAM 0001 0009 d:00000003', 'call.w SEQSETSEQPARAM 0002 0009 d:00000007'], 'all'),
    ('playing sequence 2 (thru channel 7)', ['call.w SEQSETPLAYINGSEQ 0002'], 'all'),
    ('playing sequence 1 (thru channel 3)', ['call.w SEQSETPLAYINGSEQ 0001'], 'all'),
    ('standby sequence 2: no change', ['call.w SEQSETSTANDBYSEQ 0002'], 'notes'),
    ('mode 1 (manual)', ['call.w SEQSETSEQPARAM 0001 0007 d:00000001'], 'notes'),
    ('mode 0: nothing', ['call.w SEQSETSEQPARAM 0001 0007 d:00000000'], 'notes'),
    ('mode 2, Key+ 100 and Vel+ 100: the top octave, velocity 127', ['call.w SEQSETSEQPARAM 0001 0007 d:00000002',
     'call.w SEQSETSEQPARAM 0001 000a d:00000064', 'call.w SEQSETSEQPARAM 0001 000b d:00000064'], 'all'),
    ('Key+ -100 and Vel+ -120: folded up by octaves, velocity 1', ['call.w SEQSETSEQPARAM 0001 000a d:ffffff9c',
     'call.w SEQSETSEQPARAM 0001 000b d:ffffff88'], 'notes'),
    ('channel 16: ORed into the status (a program change becomes channel pressure, the wheel goes)',
     ['call.w SEQSETSEQPARAM 0001 000a d:00000000', 'call.w SEQSETSEQPARAM 0001 000b d:00000000',
      'call.w SEQSETSEQPARAM 0001 0009 d:00000010'], 'all'),
    ('channel -1, port 1: no such port', ['call.w SEQSETSEQPARAM 0001 0009 d:ffffffff',
     'call.w SEQSETSEQPARAM 0001 0008 d:00000001'], 'notes'),
    ('port 0, record filter 0, Local On port 0: no change', ['call.w SEQSETSEQPARAM 0001 0008 d:00000000',
     'call.w SEQSETSEQPARAM 0001 001c d:00000000', 'call.w SEQSETSEQPARAM 0001 000c d:00000000'], 'all'),
    ('a note held into the next slot', [], 'held'),
    ('channel 5: the held note\'s note off goes to channel 5, where it is not on, and is dropped',
     ['call.w SEQSETSEQPARAM 0001 0009 d:00000005'], 'release'),
    ('Local On port 0 and input on channel 5, the thru channel', [], 'ch5'),
    ('Local On port 0, channel -1', ['call.w SEQSETSEQPARAM 0001 0009 d:ffffffff'], 'notes'),
    ('Local On port 1, channel -1', ['call.w SEQSETSEQPARAM 0001 000c d:00000001'], 'notes'),
    ('Local On port -1, channel 5', ['call.w SEQSETSEQPARAM 0001 000c d:ffffffff',
     'call.w SEQSETSEQPARAM 0001 0009 d:00000005'], 'notes'),
    ('SeqTxMidiMsg and the thru share the notes: the input ends a note SeqTxMidiMsg started',
     ['call.w SEQTXMIDIMSG 0000 0005 0090 0030 0050'], 'tx'),
    ('and SeqTxMidiMsg ends a note the input started; the input\'s note off is dropped',
     ['call.w SEQTXMIDIMSG 0000 0005 0080 0031 0000'], 'txoff'),
    ('no playing sequence: nothing', ['call.w SEQSETPLAYINGSEQ 0000'], 'notes'),
    ('the playing sequence destroyed: its settings stay', ['call.w SEQSETPLAYINGSEQ 0001',
     'call.w SEQDESTROYSEQ 0001'], 'notes'),
    ('a new sequence becomes the current one, but the thru keeps the settings', ['call.w SEQCREATESEQ 0000 0000'],
     'notes'),
    ('until one of them is set on it', ['call.w SEQSETSEQPARAM 0001 000a d:00000001'], 'notes'),
]
THRU_START, THRU_SLOT = 700, 500


def thru_messages(kind, k):
    """The input of one thru slot: (ms into the slot, message); the channel is 2."""
    notes = [(200, f'92 {k:02x} 64'), (230, f'82 {k:02x} 40'), (260, f'92 {k:02x} 65'), (290, f'92 {k:02x} 00')]
    if kind == 'notes':
        return notes
    if kind == 'all':
        return notes + [(320, f'a2 {k:02x} 20'), (340, 'b2 07 50'), (360, 'c2 05'), (380, 'd2 30'), (400, 'e2 00 40'),
                        (420, 'f0 01 02 f7'), (440, 'f8')]
    if kind == 'held':
        return [(200, '92 2f 64'), (300, '92 2e 64'), (350, '82 2e 00')]
    if kind == 'release':
        return [(150, '82 2f 00')] + notes
    if kind == 'tx':
        return [(200, '82 30 00'), (250, '92 31 64')]
    if kind == 'txoff':
        return [(200, '82 31 00')]
    if kind == 'ch5':
        return [(ms, '9' + m[1:].replace('2', '5', 1) if m.startswith('9') else '8' + m[1:].replace('2', '5', 1))
                for ms, m in notes]
    raise ValueError(kind)


@suite('thru')
def _():
    # MIDI thru (sequence parameters 7 to 12) with scripted input (THRU.TXT): each slot sets
    # something, then input comes in on channel 2, a few hundred milliseconds later.
    L = ['#! input THRU.TXT', '#! secs 30', '@open', f'sleep {THRU_START}']
    for i, (what, calls, _) in enumerate(THRU_SLOTS):
        L += [f'echo == {what}'] + calls + [f'sleep {THRU_SLOT}']
    # the playback keeps its notes in the same table: stopping ends the notes SeqTxMidiMsg started
    L += ['sleep 300', 'echo == stopping the playback ends the notes SeqTxMidiMsg started too',
          'call.w SEQTXMIDIMSG 0000 000f 0090 003c 0050', 'call.w SEQTXMIDIMSG 0000 0000 0090 007c 0050',
          '@load ENG_ORDR.WRK', 'let 1', 'call.w SEQSETPLAYINGSEQ %1', 'sleep 300', '@play', 'idle 300', '@stop', 'idle 300',
          '@close']
    return L


def thru_input():
    L = ['# MIDI input for the thru suite (written by tools/ttsprobe/oracles/gen.py): times in ms from',
         '# the moment the engine connects to the input (SeqOpen); one slot of 500 ms per setting.']
    for i, (what, _, kind) in enumerate(THRU_SLOTS):
        t = THRU_START + THRU_SLOT * i
        L.append(f'# {i}: {what}')
        L += [f'{t + ms} {m}' for ms, m in thru_messages(kind, 0x3c + i)]
    return L


def sync_input():
    """The scripted MIDI input of the sync suite (tests/data/ttsseq/input/SYNC.TXT)."""
    L = ['# MIDI sync for the sync suite (written by tools/ttsprobe/oracles/gen.py): times in ms from the',
         '# moment the engine connects to the input (SeqOpen). Clocks (F8) every 50 ms are 5 ticks each at',
         '# 120 ticks per quarter. The MIDI Time Code at the end is quarter frames (F1) for 30 fps from',
         '# 00:00:10:00.',
         '# 1: clocks while the engine waits do not start it']

    def clk(a, b, p=50):
        t = a
        while t < b + 0.01:
            L.append(f'{t:g} f8'); t += p
    clk(1500, 1600)
    L.append('# Start; two clocks (ticks 0 to 9), six (10 to 39), then 28 at twice the rate')
    L.append('2000 fa'); clk(2200, 2250); clk(2500, 2750); clk(3000, 3675, 25); L.append('4000 fc')
    L.append('# 2: Continue without a Song Position: from where it stopped; four clocks')
    L.append('4500 fb'); clk(4700, 4850); L.append('5000 fc')
    L.append('# Song Position while stopped (16 sixteenths: tick 480), Continue, two clocks')
    L += ['5100 f2 10 00', '6200 fb']; clk(6400, 6450)
    L.append('# Song Position 0 while playing: kept for the next Continue; two clocks, Stop')
    L.append('6500 f2 00 00'); clk(6550, 6600); L.append('6800 fc')
    L.append('7800 fb'); clk(7900, 8050); L.append('8200 fc')
    L.append('# 3: Start while the start callback polls')
    L.append('8600 fa'); clk(8800, 8950); L.append('9100 fc')
    L.append('# A Stop while the engine waits is kept: the Start that follows stops at once')
    L += ['9400 fc', '9600 fa']
    L.append('# 4: MIDI Time Code, 00:00:10:00 at 30 fps, from 10300 to 13800 ms')
    fps, k, frame = 30, 0, 10 * 30
    qf = 1000.0 / (fps * 4)
    while 10300 + k * qf <= 13800:
        piece, f = k % 8, frame + (k // 8) * 2
        ss, ff = f // fps, f % fps
        vals = [ff & 15, ff >> 4, ss & 15, ss >> 4, 0, 0, 0, 3 << 1]
        L.append(f'{10300 + k * qf:.1f} f1 {(piece << 4) | vals[piece]:02x}'); k += 1
    return L


@suite('sync')
def _():
    # External sync with the scripted input SYNC.TXT; the times below are those of its messages
    # (ms after SeqOpen). The clock source is 0 internal, 1 MIDI clock, 3 MIDI Time Code; other
    # values are refused and leave it unchanged. 2.10 and 3.00 keep it per sequence and take the
    # playing sequence's source when it is made the playing sequence, so it is set before
    # SeqSetPlayingSeq. With source 1 or 3 the start call waits for Start or Continue (or for the
    # time code) and calls its callback meanwhile: (2, 0, 0) for MIDI sync or (3, 0, 0) for time code
    # once, then (5, 0, 0) while it waits and (4, time, 0) while it locks to the time code; a
    # non-zero return gives up (the call returns 0). 3.00's SeqStart writes the start tick through
    # its second argument. A Stop received while waiting is kept and stops the playback as soon as it
    # starts. 1.01 waits without a callback in a loop that only a MIDI input callback can end, hence
    # "#! irq".
    def src(v, seq='0001'):
        return [f'?101 call.w SEQSETSRCCLOCK {v}', f'?!101 call.w SEQSETSRCCLOCK {seq} {v}',
                '?101 call.w SEQGETSRCCLOCK', f'?!101 call.w SEQGETSRCCLOCK {seq}']
    now = ['call.d SEQGETNOW 0001']
    play_cb = ['fill 0 0 ee ee ee ee', '?!300 call.w SEQPLAYSTART cb3', '?300 call.w SEQSTART cb3 @0']
    L = ['#! input SYNC.TXT', '#! secs 40', '#! irq', '@open', 'buf 0 16', '@load QUANT.WRK',
         'echo == clock source values']
    for v in ('0001', '0002', '0003', '0004', '0103', 'ffff', '0000'):
        L += src(v)
    L += ['echo == MIDI sync: wait for Start (clocks at 1500 to 1600 do not end it)'] + src('0001') + \
         ['call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000', 'call.w SEQEXPIRED', '@play'] + now + \
         ['echo == Start at 2000, two clocks: ticks 0 to 9 are played; Now is the time of the last event',
          'echo == played, else 5 ticks more than before', 'idle 400'] + now + \
         ['echo == six more clocks', 'idle 500'] + now + \
         ['echo == 28 clocks at twice the rate', 'idle 1000'] + now + ['call.w SEQEXPIRED',
          'echo == Stop at 4000', 'idle 300'] + now + ['call.w SEQEXPIRED',
          'echo == Continue at 4500 without a Song Position: from where it stopped; four clocks; Stop',
          '@play'] + now + ['idle 450'] + now + ['idle 300'] + now + \
         ['echo == Song Position 480 at 5100 while stopped; Continue at 6200; two clocks', '@play'] + now + \
         ['idle 280'] + now + ['echo == Song Position 0 at 6500 while playing; two clocks; Stop at 6800',
          'idle 220'] + now + ['idle 300'] + now + \
         ['echo == Continue at 7800: from the Song Position kept; four clocks; Stop at 8200', '@play'] + now + \
         ['idle 350'] + now + ['idle 200'] + now + \
         ['echo == Start at 8600 while the start callback polls; four clocks; Stop', 'cbret 0'] + play_cb + now + \
         ['idle 450'] + now + ['idle 250', '?300 dump 0 0 4',
          'echo == Stop at 9400 while waiting is kept: the Start at 9600 stops at once', '@play'] + now + \
         ['idle 300'] + now + ['call.w SEQEXPIRED',
          'echo == MIDI Time Code from 00:00:10:00 at 10300 (ENG_ORDR, which ends before 00:00:10:00):',
          'echo == the start returns 2 s after the code locks, at the time of the code',
          '@load ENG_ORDR.WRK'] + src('0003', '0002') + \
         ['call.w SEQSETPLAYINGSEQ 0002', 'fill 0 0 ee ee ee ee', '?!300 call.w SEQPLAYSTART null',
          '?300 call.w SEQSTART null @0', 'call.d SEQGETNOW 0002', '?300 dump 0 0 4',
          'echo == the code stops at 13800; the engine runs on for 0.5 s, then stops', 'idle 2500',
          'call.d SEQGETNOW 0002', 'call.w SEQEXPIRED', 'echo == the callback gives up at once', 'cbret 1'] + play_cb + \
         ['call.w SEQEXPIRED', '@close']
    return L
def qframes(t0, dur, rate, h, m, s, f, speed=1.0):
    """MIDI Time Code quarter frames (F1) from t0 for dur ms: code h:m:s:f at rate 24, 25, 29 (30 drop
    frame, running at 29.97 frames a second) or 30, as fake input lines; speed > 1 sends it faster
    than real time"""
    nominal = 30 if rate == 29 else rate
    rcode = {24: 0, 25: 1, 29: 2, 30: 3}[rate]
    start = ((h * 60 + m) * 60 + s) * nominal + f
    if rate == 29:
        mins = h * 60 + m
        start -= 2 * (mins - mins // 10)

    def hmsf(n):
        if rate == 29:
            d, mm = divmod(n, 17982)
            n += 18 * d + (2 * ((mm - 2) // 1798) if mm >= 2 else 0)
        t = n // nominal
        return t // 3600, t // 60 % 60, t % 60, n % nominal
    qf = 1000.0 / (nominal * (1000 / 1001 if rate == 29 else 1) * 4) / speed
    L, k = [], 0
    while k * qf <= dur:
        hh, mi, ss, ff = hmsf(start + (k // 8) * 2)
        piece = k % 8
        vals = [ff & 15, ff >> 4, ss & 15, ss >> 4, mi & 15, mi >> 4, hh & 15, (hh >> 4) | (rcode << 1)]
        L.append(f'{t0 + k * qf:.1f} f1 {(piece << 4) | vals[piece]:02x}'); k += 1
    return L


MTC_CASES = [  # (title, song, code start ms, code ms, code rate, code h:m:s:f, SeqSetFPS, offset h:m:s:f, give up after ms, sleep after ms)
    ('25 fps code, the sequence at 25 fps', 'ENG_ORDR', 2000, 2600, 25, (0, 0, 10, 0), 25, None, 0, 600),
    ('offset 01:00:00:00, code 01:02:03:04: the callback\'s third word is hours * 256 + minutes', 'ENG_ORDR',
     6000, 2600, 30, (1, 2, 3, 4), 30, (1, 0, 0, 0), 0, 600),
    ('30 fps drop frame, code 00:10:00:00: the code counts as its label, 600 s', 'ENG_ORDR',
     10000, 2600, 29, (0, 10, 0, 0), 29, None, 0, 600),
    ('a tempo map (ENG_TMPO), code 00:00:10:00: Now as SeqFrame2TickRndOpt gives it', 'ENG_TMPO',
     14000, 2600, 30, (0, 0, 10, 0), 30, None, 0, 600),
    ('offset 00:00:20:00, code from 00:00:17:00: the chase goes on until the code reaches the offset; Now 0',
     'ENG_ORDR', 18000, 5500, 30, (0, 0, 17, 0), 30, (0, 0, 20, 0), 0, 2700),
    ('25 fps code, the sequence at 30 fps: ignored; the callback gives up', 'ENG_ORDR',
     25000, 3000, 25, (0, 0, 10, 0), 30, None, 5500, 300),
    ('code 00:34:06:00, no offset: too far after the offset; ignored; the callback gives up', 'ENG_ORDR',
     31000, 3000, 30, (0, 34, 6, 0), 30, None, 5500, 300),
    ('the limit is 4096 quarter notes: at 60 bpm, code 00:34:09:00 locks', 'ENG_ORDR',
     37000, 2600, 30, (0, 34, 9, 0), 30, None, 0, 600, {'pre': ['call.w SEQSETTEMPO 0001 d:00000000 1770']}),
    ('at 240 bpm, code 00:17:30:00 is ignored; the callback gives up', 'ENG_ORDR',
     41000, 3000, 30, (0, 17, 30, 0), 30, None, 5500, 300, {'pre': ['call.w SEQSETTEMPO 0001 d:00000000 5dc0']}),
    ('code 1.5 times too fast: the chase counts code time; Now follows the code', 'ENG_ORDR',
     47000, 3500, 30, (0, 0, 10, 0), 30, None, 0, 1300, {'speed': 1.5}),
    ('a jump of 2 frames while following: 1.01 and 2.10 stop within about 0.3 s; 3.00 follows it', 'ENG_ORDR',
     52000, 3000, 30, (0, 0, 10, 0), 30, None, 0, 2700, {'then': (55000, 2500, (0, 0, 13, 2)), 'hold': 1500, 'now': False}),
    ('a jump of 1 s: all three stop', 'ENG_ORDR',
     59000, 3000, 30, (0, 0, 10, 0), 30, None, 0, 2700, {'then': (62000, 2500, (0, 0, 14, 0)), 'hold': 1500}),
]


@suite('mtc')
def _():
    # MIDI Time Code (clock source 3) with frame rates, SMPTE offsets and a tempo map; input MTC.TXT.
    # The engine locks only to code of the sequence's own frame rate (SeqSetFPS; 29 is 30 drop frame).
    # The code's time counts as its label (h:m:s plus frames at the nominal rate), minus the
    # sequence's SMPTE offset (SeqSetFrameOfs, in frames of the sequence's rate). The chase lasts
    # ChaseDelay and goes on while the code lies before the offset. Code of another rate, and code
    # whose time after the chase would lie 4096 quarter notes or more after the offset (34:08 at
    # 120 bpm, whatever the PPQ), is ignored: no chase at all. While chasing, the callback gets
    # (4, seconds * 256 + frames, hours * 256 + minutes) of the incoming code. Now at the lock is
    # the code's time, through the tempo map; the chase lasts ChaseDelay of code time, and the
    # playback follows the code's speed. A jump in the code stops the playback within about 0.3 s,
    # whatever FlywheelAmount says: in 1.01 and 2.10 any jump, in 3.00 one of more than 2 frames.
    def src(v):
        return [f'?101 call.w SEQSETSRCCLOCK {v}', f'?!101 call.w SEQSETSRCCLOCK 0001 {v}']
    L = ['#! input MTC.TXT', '#! secs 100', '#! irq', '@open', 'buf 0 16', 'cbyield 1', 'cbret 0',
         'cbmask 2 merge']  # the callback's code seconds and frames: one line per lock
    for title, song, t0, dur, rate, code, fps, ofs, giveup, after, *more in MTC_CASES:
        x = more[0] if more else {}
        L += [f'echo == {title}', f'@load {song}.WRK'] + src('0003') + x.get('pre', [])
        L += [f'call.w SEQSETFPS 0001 {fps:04x}', 'call.w SEQGETFPS 0001']
        if ofs:
            L += ['call.d SEQHMSF2FRAME 0001 ' + ' '.join(f'{x:04x}' for x in ofs) + ' 0000', 'let 3',
                  'call.w SEQSETFRAMEOFS 0001 %d3', 'call.d SEQGETFRAMEOFS 0001']
        L += ['call.w SEQSETPLAYINGSEQ 0001', 'call.w SEQSETSTANDBYSEQ 0000']
        if giveup:
            L += [f'cbretafter {giveup} 1']
        L += ['fill 0 0 ee ee ee ee', '?!300 call.w SEQPLAYSTART cb3', '?300 call.w SEQSTART cb3 @0',
              'call.d SEQGETNOW 0001', '?300 dump 0 0 4']
        if 'ENG_TMPO' in song:
            L += ['call.d SEQFRAME2TICKRNDOPT 0001 d:0000016f 0000', 'call.d SEQFRAME2TICKRNDOPT 0001 d:00000170 0000']
        if 'hold' in x:
            L += [f'idle {x["hold"]}'] + (['call.d SEQGETNOW 0001'] if x.get('now', True) else []) + ['call.w SEQEXPIRED']
        L += ['cbret 0'] if giveup else ['idle 200', '@stop']
        L += [f'sleep {after}', 'call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


def mtc_input():
    L = ['# MIDI Time Code for the mtc suite (written by tools/ttsprobe/oracles/gen.py): quarter frames',
         '# (F1), times in ms from the moment the engine connects to the input (SeqOpen)']
    for title, song, t0, dur, rate, code, fps, ofs, giveup, after, *more in MTC_CASES:
        x = more[0] if more else {}
        L.append(f'# {title}')
        L += qframes(t0, dur, rate, *code, speed=x.get('speed', 1.0))
        if 'then' in x:
            L += qframes(x['then'][0], x['then'][1], rate, *x['then'][2])
    return L


OPTS_USUAL = {'SyncOutputPort': 1}
OPTS = {  # the [Options] of the two options suites (tests/data/ttsseq/input/OPTS1.INI, OPTS2.INI)
    1: {'SendLocalOff': 0, 'SendLocalOn': 1, 'ForceNoteOff': 1, 'Timebase': 96, 'SysxSendPacketSize': 100,
        'SysxReceiveBuffersSize': 32, 'RecordLengthDesired': 1, 'ChaseDelay': 5, 'FlywheelAmount': 1},
    2: {'SendLocalOff': 2, 'SendLocalOn': 2, 'Timebase': 7, 'SysxSendPacketSize': 0, 'SysxReceiveBuffers': 1,
        'RecordLengthDesired': 0},
}


def opts_ini(n):
    o = dict(OPTS_USUAL, **OPTS[n])
    return ['[Options]'] + [f'{k}={v}' for k, v in o.items()] + \
        ['[MIDI Output Devices]', '0=Fake - MIDI Out', '[MIDI Input Devices]', '0=Fake - MIDI In']


def opts_input():
    """The scripted MIDI input of the options1 suite (tests/data/ttsseq/input/OPTS1.TXT)."""
    a = [0xf0, 0x7d] + [0x10] * 98 + [0xf7]
    a[33] = 0x7f  # the second byte of the second 32-byte input buffer
    b = [0xf0, 0x7d] + [i % 100 for i in range(98)] + [0xf7]
    L = ['# MIDI input for the options1 suite (written by tools/ttsprobe/oracles/gen.py): times in ms from',
         '# the moment the engine connects to the input (SeqOpen).',
         '# 1: two sysex messages of 101 bytes; in the first, 7F is the second byte of the second',
         '# 32-byte input buffer',
         '2000 ' + ' '.join(f'{x:02x}' for x in a), '2400 ' + ' '.join(f'{x:02x}' for x in b),
         '# 2: 1100 notes on, 2 ms apart, to record (a 1 x 8 KB record buffer holds 1024 events)']
    L += [f'{4000 + 2 * i} 90 {0x24 + i % 40:02x} 40' for i in range(1100)]
    L.append('# 3: MIDI Time Code, 00:00:10:00 at 30 fps, from 11500 to 15000 ms')
    fps, k, frame = 30, 0, 10 * 30
    qf = 1000.0 / (fps * 4)
    while 11500 + k * qf <= 15000:
        piece, f = k % 8, frame + (k // 8) * 2
        ss, ff = f // fps, f % fps
        vals = [ff & 15, ff >> 4, ss & 15, ss >> 4, 0, 0, 0, 3 << 1]
        L.append(f'{11500 + k * qf:.1f} f1 {(piece << 4) | vals[piece]:02x}'); k += 1
    return L


def fill_text(buf, text):
    """fill commands that put text (zero-terminated) into a probe buffer, 120 bytes a line"""
    data = text.encode('latin1') + b'\0'
    return [f'fill {buf} {i} ' + ' '.join(f'{x:02x}' for x in data[i:i + 120]) for i in range(0, len(data), 120)]


@suite('options1')
def _():
    # TTSSEQ.INI [Options] other than the usual ones (OPTS1.INI; read by SeqOpen): no Local Control
    # Off when the port opens, Local On when it closes (2.10, 3.00); note-offs as 8n kk 40 (3.00);
    # new sequences at 96 ppq (2.10, 3.00); sysex sent in packets of 100 bytes; sysex received into
    # 32-byte buffers, where 1.01 and 2.10 drop a buffer whose second byte is 7E or 7F (they look for
    # universal sysex in every buffer, 3.00 in every message); a record buffer of 1 x 8 KB, which
    # holds 1024 events (2.10, 3.00); time code locked after 0.5 s instead of 2 s, and a run-on of
    # 0.1 s instead of 0.5 s when it stops. Sequence 1 has no thru, count-in or recording metronome.
    big = 'F0 7D ' + ' '.join(f'{i % 100:02X}' for i in range(297)) + ' F7'
    L = ['#! input OPTS1.TXT', '#! ini OPTS1.INI', '#! secs 40', '#! irq', '@open', 'buf 0 16', 'buf 1 1024',
         'echo == a new sequence', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQGETPPQ 0001',
         'call.w SEQSETSEQPARAM 0001 0007 d:00000000', 'call.w SEQSETSEQPARAM 0001 0010 d:00000000',
         'call.w SEQSETSEQPARAM 0001 000e d:00000000',
         'echo == send a bank with one sysex message of 300 bytes', 'call.w SEQCREATESYSXBANK 0001', 'let 1'] + \
        fill_text(1, big) + \
        ['call.w SEQSYSXASC2BIN %1 @1 @0', 'call.d SEQGETSYSXBANKLEN %1', 'call.w SEQSENDSYSXBANK %1 0000 0000',
         'echo == receive two messages of 101 bytes (2000 and 2400 ms)', 'call.w SEQCREATESYSXBANK 0001', 'let 2',
         'cbmask f merge', 'cbyield 1', 'cbret 0', 'sleep 200', 'cbretafter 2600 1',
         'call.w SEQRECEIVESYSXBANK %2 cb4 0000 0000 0000', 'cbmask 0', 'call.d SEQGETSYSXBANKLEN %2',
         'fill 1 0 00', 'call.w SEQSYSXBIN2ASC %2 @1 0020', 'dump 1 0 330',
         'echo == record 1100 notes (4000 to 6200 ms)', 'call.w SEQSETPLAYINGSEQ 0001',
         'call.w SEQSETSTANDBYSEQ 0000', 'cbret 0',
         '?101,210 call.w SEQRECORDSTART 0000 0000 0000 0000 0000 cb3',
         '?300 call.w SEQRECORD 0001', '?300 call.w SEQSTART cb3 0000 0000', '?300 call.w SEQGETRECORDBUFFER',
         '?300 call.w SEQSETRECORDBUFFERPARAMS 0000 0000 0000', 'idle 3800',
         '?101,210 call.w SEQRECORDSTOP', '?101,210 call.d SEQRECCOUNT',
         '?300 call.w SEQGETRECORDBUFFER', '?300 call.w SEQRECORD 0000', '?300 call.w SEQSTOP',
         '?300 call.d SEQSTREAMLEN 0100',
         'echo == play E2_OVLP (overlapping notes)', '@load E2_OVLP.WRK', 'call.w SEQSETPLAYINGSEQ 0002',
         'call.w SEQSETSTANDBYSEQ 0000', '@play', 'idle 2600', '@stop', 'idle 200',
         'echo == MIDI Time Code from 00:00:10:00 (11500 to 15000 ms)',
         '?101 call.w SEQSETSRCCLOCK 0003', '?!101 call.w SEQSETSRCCLOCK 0002 0003', 'call.w SEQSETPLAYINGSEQ 0002',
         'fill 0 0 ee ee ee ee', '?!300 call.w SEQPLAYSTART null', '?300 call.w SEQSTART null @0',
         'call.d SEQGETNOW 0002', '?300 dump 0 0 4', 'idle 3500', 'call.d SEQGETNOW 0002', 'call.w SEQEXPIRED',
         '@close']
    return L


@suite('options2')
def _():
    # TTSSEQ.INI [Options] at odd values (OPTS2.INI): SendLocalOff and SendLocalOn 2 count as on;
    # Timebase 7 gives 24 ppq (2.10, 3.00: below 24 counts as 24, above 480 as 480, other values not
    # in the list as 120); SysxSendPacketSize 0 sends one byte at a time; SysxReceiveBuffers 1 makes
    # the receive fail (-4); RecordLengthDesired 0 counts as 16.
    L = ['#! input OPTS2.TXT', '#! ini OPTS2.INI', '#! secs 30', '@open', 'buf 0 16', 'buf 1 64',
         'call.w SEQCREATESEQ 0000 0000', 'call.w SEQGETPPQ 0001',
         'echo == send a bank of 6 bytes', 'call.w SEQCREATESYSXBANK 0001', 'let 1'] + \
        fill_text(1, 'F0 7D 01 02 03 F7') + \
        ['call.w SEQSYSXASC2BIN %1 @1 @0', 'call.w SEQSENDSYSXBANK %1 0000 0000', 'sleep 800',
         'echo == receive with one buffer (a message at 2000 ms)', 'call.w SEQCREATESYSXBANK 0001', 'let 2',
         'cbmask c', 'cbyield 1', 'cbret 0', 'cbretafter 1500 1',
         'call.w SEQRECEIVESYSXBANK %2 cb4 0000 0000 0000', 'call.d SEQGETSYSXBANKLEN %2',
         '@close']
    return L


def main():
    os.makedirs(os.path.join(HERE, 'suites'), exist_ok=True)
    for name, fn in SUITES.items():
        with open(os.path.join(HERE, 'suites', name + '.tpl'), 'w') as f:
            f.write('\n'.join(fn()) + '\n')
        print('wrote', name)
    inputs = os.path.join(HERE, '..', '..', '..', 'tests', 'data', 'ttsseq', 'input')
    for name, lines, nl in [('SYNC.TXT', sync_input(), '\n'), ('OPTS1.TXT', opts_input(), '\n'),
                            ('OPTS2.TXT', ['# MIDI input for the options2 suite (written by tools/ttsprobe/oracles/gen.py)',
                                          '2000 f0 7d 01 02 03 f7'], '\n'),
                            ('PUNCH.TXT', punch_input(PUNCH_STARTS), '\n'), ('THRU.TXT', thru_input(), '\n'), ('MTC.TXT', mtc_input(), '\n'),
                            ('OPTS1.INI', opts_ini(1), '\r\n'), ('OPTS2.INI', opts_ini(2), '\r\n')]:
        with open(os.path.join(inputs, name), 'w', newline='') as f:
            f.write(nl.join(lines) + nl)
        print('wrote input/' + name)


if __name__ == '__main__':
    main()
