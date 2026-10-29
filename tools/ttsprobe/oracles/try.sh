#!/bin/bash
# try.sh VER [SECS] < TEMPLATE: renders a template (render.py syntax) in raw mode, runs it on one
# version and prints the probe's output. For exploring; FAKELOG=file keeps the MIDI output,
# TRACE=+snoop the Wine call trace (in $TTSPROBE_WORK/try.log).
here=$(cd "$(dirname "$0")" && pwd)
. "$here/../env.sh"
ver=$1; secs=${2:-25}
V=$PROBE_C/V$ver
mkdir -p "$PROBE_C/ORC/OUT"; cp -n "$SONGS"/*/*.WRK "$PROBE_C/ORC/" 2>/dev/null
t=$(mktemp); cat > "$t"; RAW=1 python3 "$here/render.py" $ver "$t" > "$V/try.txt"; rm -f "$t" "$V/try.out"
FAKESEQ_LOG=${FAKELOG:-/dev/null} TRACE=${TRACE:--all} "$RUNCW" "$V" ttsprobe.exe "$TTSPROBE_WORK/try.log" $secs try.txt try.out
tr -d '\r' < "$V/try.out" 2>/dev/null
