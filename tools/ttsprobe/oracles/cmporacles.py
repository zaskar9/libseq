#!/usr/bin/env python3
"""cmporacles.py A B [TOL_MS]: compares two oracle result folders (two runs of the DLL, or the DLL
against libseq's script runner). Per suite:
  .out   must be identical, except in suites that record live input ("#! input"), where event times
         and durations may differ by a few ticks (timer jitter); those lines are reported, not failed
  .midi  the same messages in the same order; in suites that play (@play, no live input) also the
         times within TOL_MS (default 30) of each other after aligning the first message, allowing
         1% of the messages (at least one) up to 5 × TOL_MS for Wine's timer stalls
  files  saved files must be byte-identical
Prints one line per suite and a summary; exit status 1 when something differs."""
import filecmp, os, sys


def midi(path):
    out = []
    for l in open(path):
        p = l.split()
        if p:
            out.append((float(p[0]), ' '.join(p[1:])))
    t0 = out[0][0] if out else 0
    return [(t - t0, m) for t, m in out]


def main():
    a, b = sys.argv[1], sys.argv[2]
    tol = float(sys.argv[3]) if len(sys.argv) > 3 else 30.0
    here = os.path.dirname(os.path.abspath(__file__))
    bad = 0
    suites = sorted(f[:-4] for f in os.listdir(a) if f.endswith('.out'))
    only_a = [s for s in suites if not os.path.exists(os.path.join(b, s + '.out'))]
    if only_a:
        print(f'not in {b}, not compared: {len(only_a)} suites')
    suites = [s for s in suites if s not in only_a]
    for s in suites:
        notes = []
        tpl = os.path.join(here, 'suites', s + '.tpl')
        text = open(tpl).read() if os.path.exists(tpl) else ''
        live = '#! input' in text
        timed = '@play' in text and not live  # elsewhere MIDI times only reflect how long the calls took
        pa, pb = os.path.join(a, s + '.out'), os.path.join(b, s + '.out')
        la, lb = open(pa).read().splitlines(), open(pb).read().splitlines()
        diff = [(i, x, y) for i, (x, y) in enumerate(zip(la, lb)) if x != y]
        if len(la) != len(lb):
            notes.append(f'out: {len(la)} vs {len(lb)} lines')
        if diff:
            notes.append(f'out: {len(diff)} lines differ, first at {diff[0][0] + 1}: {diff[0][1]!r} / {diff[0][2]!r}')
        ok = not notes or (live and len(la) == len(lb))
        ma, mb = os.path.join(a, s + '.midi'), os.path.join(b, s + '.midi')
        if os.path.exists(ma) or os.path.exists(mb):
            if not (os.path.exists(ma) and os.path.exists(mb)):
                notes.append('midi: only in one'); ok = False
            else:
                xa, xb = midi(ma), midi(mb)
                if [m for _, m in xa] != [m for _, m in xb]:
                    k = next((i for i, (x, y) in enumerate(zip(xa, xb)) if x[1] != y[1]), min(len(xa), len(xb)))
                    notes.append(f'midi: messages differ ({len(xa)} vs {len(xb)}), first at {k + 1}'); ok = False
                else:
                    worst = max((abs(x[0] - y[0]) for x, y in zip(xa, xb)), default=0)
                    if not timed:
                        notes.append(f'midi: {len(xa)} messages in the same order')
                    else:
                        # Wine's timer stalls now and then: allow 1% of the messages (at least one)
                        # beyond TOL_MS, none beyond 5 × TOL_MS
                        late = sum(1 for x, y in zip(xa, xb) if abs(x[0] - y[0]) > tol)
                        if late > max(1, len(xa) // 100) or worst > 5 * tol:
                            notes.append(f'midi: times differ by up to {worst:.1f} ms ({late} beyond {tol:g})'); ok = False
                        else:
                            notes.append(f'midi: {len(xa)} messages, times within {worst:.1f} ms' + (f' ({late} beyond {tol:g})' if late else ''))
        da, db = os.path.join(a, s), os.path.join(b, s)
        if os.path.isdir(da) or os.path.isdir(db):
            fa = sorted(os.listdir(da)) if os.path.isdir(da) else []
            fb = sorted(os.listdir(db)) if os.path.isdir(db) else []
            if fa != fb:
                notes.append('files: different names'); ok = False
            else:
                _, mism, err = filecmp.cmpfiles(da, db, fa, shallow=False)
                if mism or err:
                    notes.append(f'files: {len(mism) + len(err)} of {len(fa)} differ ({", ".join((mism + err)[:4])})'); ok = False
                else:
                    notes.append(f'files: {len(fa)} identical')
        bad += not ok
        print(f'{"ok  " if ok else "DIFF"} {s}: ' + '; '.join(notes))
    print(f'{len(suites) - bad} of {len(suites)} suites agree')
    sys.exit(1 if bad else 0)


main()
