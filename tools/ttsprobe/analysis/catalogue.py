#!/usr/bin/env python3
"""Merges the export tables, argument sizes and trace samples of the three TTSSEQ versions into one
markdown table.  catalogue.py > api-catalogue.md

Inputs, in docs/reverse-engineering of this repository: exports/exports{101,210,300}.txt,
exports/argsizes-{101,210,300}.txt (argcount.py output of the sweeps) and traces/trace-*.txt
(snoopsum.py output of the program traces)."""
import os
import re
from collections import OrderedDict

BASE = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', 'docs', 'reverse-engineering')) + '/'
VERS = ['101', '210', '300']
GROUPS = [
    ('Lifecycle', r'^(SEQGETVERSION|SEQOPEN|SEQCLOSE|SEQIDLE|SEQSLEEP|SEQWAKE|SEQWIRE|WEP|SEQSETBAGGAGE.*|SEQGETBAGGAGE.*|SEQCACHEPATCHES)$'),
    ('Sequences', r'^SEQ(CREATE|DESTROY)SEQ$|^SEQ(SET|GET)SEQPARAM$|^SEQSWITCHTOSTANDBYSEQ$'),
    ('Streams', r'STREAM|^SEQMEASHASEVTS$'),
    ('Event cursor and edits', r'EVT|^SEQ(TELL|EOS|SEEK)$|RANGE$|^SEQDISPLACE|^SEQTEMPORALSORT$|EVENTKIND|EVENTPROC'),
    ('Time base and tempo map', r'PPQ|TEMPO|^SEQRESCALETIME$'),
    ('Timecode', r'FPS|FRAME|HMSF'),
    ('Meter map and positions', r'METER|MBT|MEAS|BEAT|^SEQTICK2'),
    ('Transport', r'PLAY|STANDBY|SRCCLOCK|^SEQ(START|STOP|REWIND|EXPIRED)$|NOW$|^SEQ(GET|SET)(END|DUR)$'),
    ('Recording and input', r'REC|^SEQTXMIDIMSG$|MACRO|STEPREC'),
    ('Sysex banks', r'SYSX'),
    ('Files', r'FILE|^SEQIO'),
    ('Devices', r'MIDI$|PORTNAME|^SEQCONFIGMIDI$'),
    ('Markers', r'MARKER'),
    ('Internal', r'.'),
]


def load_exports():
    ex = {}
    for v in VERS:
        ex[v] = OrderedDict()
        for line in open(f'{BASE}exports/exports{v}.txt'):
            o, n = line.split()
            ex[v][n] = int(o)
    return ex


def load_counts():
    counts = {}
    for v in VERS:
        counts[v] = {}
        try:
            for line in open(f'{BASE}exports/argsizes-{v}.txt'):
                p = line.split()
                if len(p) >= 3:
                    counts[v][p[1]] = p[2]
        except FileNotFoundError:
            pass
    return counts


def load_samples():
    samples = {}
    for path, v in [('traces/trace-101-demo.txt', '101'), ('traces/trace-210-apprentice.txt', '210'),
                    ('traces/trace-210-views.txt', '210'), ('traces/trace-300-pro200.txt', '300')]:
        try:
            for line in open(BASE + path):
                m = re.match(r'\s*\d+ (\S+)\s+\d+\s+\d+ \| (.*?) \| (.*)$', line)
                if m and m.group(2):
                    samples.setdefault((v, m.group(1)), m.group(2).split(' ; ')[0])
        except FileNotFoundError:
            pass
    return samples


def main():
    ex, counts, samples = load_exports(), load_counts(), load_samples()
    names = []
    for v in VERS:
        for n in ex[v]:
            if n not in names:
                names.append(n)
    grouped = OrderedDict((g, []) for g, _ in GROUPS)
    for n in names:
        for g, pat in GROUPS:
            if re.search(pat, n):
                grouped[g].append(n)
                break
    print('| Function | 1.01 | 2.10 | 3.00 | Sample arguments (from a program trace) |')
    print('|---|---|---|---|---|')
    for g, ns in grouped.items():
        if not ns:
            continue
        print(f'| **{g}** | | | | |')
        for n in sorted(ns, key=lambda n: min(ex[v].get(n, 999) for v in VERS)):
            cells = []
            for v in VERS:
                if n not in ex[v]:
                    cells.append('')
                else:
                    cells.append(f'#{ex[v][n]}: {counts[v].get(n, "?")}')
            sample = next((f'{samples[(v, n)]} ({v[0]}.{v[1:]})' for v in reversed(VERS) if (v, n) in samples), '')
            print(f'| `{n}` | ' + ' | '.join(cells) + f' | {sample} |')


main()
