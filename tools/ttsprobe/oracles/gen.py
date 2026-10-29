#!/usr/bin/env python3
"""gen.py: writes the oracle suites (suites/*.tpl). Each suite is a version-independent template
for render.py. Run it after changing a suite; mkoracles.sh then renders and runs them."""
import json
import os

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
    L = ['@open', 'buf 0 64']
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
    L += ['echo == edits', '@load ENG_ORDR.WRK']
    for title, c in [('insert 3/4 at measure 3', 'call.w SEQINSMETER 0001 0003 0003 0002'),
                     ('insert 6/8 at measure 5', 'call.w SEQINSMETER 0001 0005 0006 0003'),
                     ('insert 5/4 at measure 3 again', 'call.w SEQINSMETER 0001 0003 0005 0002'),
                     ('set (layout unsure)', 'call.w SEQSETMETER 0001 0002 0002 0003 0002'),
                     ('delete 2', 'call.w SEQDELMETER 0001 0002'),
                     ('recalc', 'call.w SEQRECALCMETERMAP 0001'),
                     ('delete 1', 'call.w SEQDELMETER 0001 0001'),
                     ('clear to 3/2', 'call.w SEQCLEARMETERMAP 0001 0003 0001')]:
        L += [f'echo == {title}', c] + metermap() + positions()
    L += ['echo == swap', 'call.w SEQCREATESEQ 0000 0000', 'call.w SEQSWAPMETERMAPS 0001 0002'] + metermap('0001') + metermap('0002')
    L += ['@close']
    return L


@suite('timecode')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64']
    for song in ['ENG_TMPO', 'ENG_ORDR', 'SYN_MAPS']:
        L += [f'echo -- {song}', f'@load {song}.WRK']
        for fps in (0x18, 0x19, 0x1d, 0x1e, 0x1f):
            L += [f'echo fps {fps:04x}', f'call.w SEQSETFPS 0001 {fps:04x}', 'call.w SEQGETFPS 0001', 'call.d SEQGETFRAMEOFS 0001']
            for t in TICKS:
                L += [f'call.d SEQTICK2FRAME 0001 d:{t:08x}', 'fill 1 0 ' + 'ee ' * 16,
                      f'call.w SEQTICK2HMSFSTR 0001 d:{t:08x} @1', 'dump 1 0 16']
            for f in (0, 1, 29, 30, 59, 60, 1799, 1800, 107892, 0x12345):
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
    L += ['call.w SEQEMPTYSYSXBANK %1'] + bank('%1') + ['call.w SEQDESTROYSYSXBANK %1'] + banks()
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



PLAY_SONGS = ['E2_CHAS', 'E2_KORD', 'E2_METR', 'E2_OVLP', 'E2_SPP', 'E2_STOP', 'E3_DENS', 'E3_MA', 'E3_MB', 'E3_MC',
              'E3_MD', 'E3_ME', 'E3_MF', 'E3_SHTL', 'E3_SPPD', 'ENG_CHAS', 'ENG_LOOP', 'ENG_METR', 'ENG_MULT', 'ENG_ORDR',
              'ENG_PARM', 'ENG_SYNC', 'ENG_SYSX', 'ENG_TMPO', 'FURELISE']
PLAY_MS = {'E3_DENS': 20000, 'FURELISE': 30000}


def make_play(song):
    def fn():
        ms = PLAY_MS.get(song, 12000)
        return [f'#! secs {ms // 1000 + 20}', '@open', f'@load {song}.WRK', 'call.w SEQSETPLAYINGSEQ 0001',
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


@suite('copypaste')
def _():
    L = ['@open', 'buf 0 64', 'buf 1 64']
    for title, cmds in [
        ('tempo copy 0..0x780, paste at 0x780 (Apprentice)',
         ['call.w SEQTEMPOCOPY 0002 0001 d:00000000 d:00000780 d:00000000 @1', 'dump 1 0 8',
          'call.w SEQTEMPOPASTE 0001 0002 d:00000780 d:00000781 0001 0000']),
        ('tempo copy 0..0x3c0, paste at 0x1e0 with slide (Professional)',
         ['call.w SEQTEMPOCOPY 0002 0001 d:00000000 d:000003c0 d:00000000 @1', 'dump 1 0 8',
          'call.w SEQTEMPOPASTE 0001 0002 d:000001e0 d:000003c1 0001 0001']),
        ('meter copy 0..0x780, paste at 0x780',
         ['call.w SEQMETERCOPY 0002 0001 d:00000000 d:00000780 @1', 'dump 1 0 8',
          'call.w SEQMETERPASTE 0001 0002 d:00000780 0001 0000']),
    ]:
        for song in ['ENG_TMPO', 'SYN_MAPS']:
            L += [f'echo == {title}: {song}', f'@load {song}.WRK', 'call.w SEQCREATESEQ 0000 0000'] + cmds
            L += tempomap('0001') + metermap('0001') + tempomap('0002') + metermap('0002')
            L += ['call.w SEQDESTROYSEQ 0002', 'call.w SEQDESTROYSEQ 0001']
    L += ['@close']
    return L


@suite('record')
def _():
    # input script IN3.TXT: times from the moment the engine connects to the input (SeqOpen)
    L = ['#! input IN3.TXT', '#! secs 40', '@open', 'call.w SEQCREATESEQ 0000 0000',
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


def main():
    os.makedirs(os.path.join(HERE, 'suites'), exist_ok=True)
    for name, fn in SUITES.items():
        with open(os.path.join(HERE, 'suites', name + '.tpl'), 'w') as f:
            f.write('\n'.join(fn()) + '\n')
        print('wrote', name)


if __name__ == '__main__':
    main()
