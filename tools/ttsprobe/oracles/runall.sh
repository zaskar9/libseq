#!/bin/bash
# runall.sh: regenerates the oracle sets of all three versions in $TTSSEQ_DATA, runs 1.01 a second
# time into $TTSPROBE_WORK/repeat to check that the results repeat, and writes the reports next to
# the sets: repeat-101.txt, coverage.txt, diff-101-210.txt, diff-210-300.txt. Takes about 90 minutes.
here=$(cd "$(dirname "$0")" && pwd)
. "$here/../env.sh"
python3 "$here/gen.py" > /dev/null
for v in 101 210 300; do "$here/mkoracles.sh" $v; done
TTSSEQ_DATA=$TTSPROBE_WORK/repeat "$here/mkoracles.sh" 101 > /dev/null
python3 "$here/cmporacles.py" "$TTSSEQ_DATA/101" "$TTSPROBE_WORK/repeat/101" > "$TTSSEQ_DATA/repeat-101.txt"
for v in 101 210 300; do python3 "$here/coverage.py" $v; done > "$TTSSEQ_DATA/coverage.txt"
python3 "$here/diffversions.py" "$TTSSEQ_DATA/101" "$TTSSEQ_DATA/210" > "$TTSSEQ_DATA/diff-101-210.txt"
python3 "$here/diffversions.py" "$TTSSEQ_DATA/210" "$TTSSEQ_DATA/300" > "$TTSSEQ_DATA/diff-210-300.txt"
tail -1 "$TTSSEQ_DATA/repeat-101.txt"
