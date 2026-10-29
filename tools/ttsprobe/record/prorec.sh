#!/bin/bash
# prorec.sh TAG INPUT [GO_DELAY_S] [RECORD_S]: records with Cakewalk Professional 2.00 itself.
# Starts WINCAKE.EXE (installed from your own disks in C:\WINCAKE of the Wine prefix, with the
# TTSSEQ.INI of setup.sh and TTSNOTE.TTF in the prefix's Fonts folder) with the scripted MIDI
# INPUT, presses R (record into track 1), starts the input GO_DELAY_S seconds later, stops after
# RECORD_S more seconds, saves C:\PROBE\TAG.WRK and quits. Logs: $TTSPROBE_WORK/record/TAG.*.
# The screen positions assume a 1024x768 display without a window manager.
. "$(dirname "$0")/../env.sh"
tag=$1; in=$(cd "$(dirname "$2")" && pwd)/$(basename "$2"); godelay=${3:-2.6}; recs=${4:-4.5}
out=$TTSPROBE_WORK/record; mkdir -p "$out"; rm -f "$out/go"; : > "$out/$tag.fake"
export DISPLAY=${DISPLAY:-:99}
FAKESEQ_LOG=$out/$tag.fake FAKESEQ_INPUT=$in FAKESEQ_INPUT_GO=$out/go TRACE=${TRACE:-+snoop} \
    "$RUNCW" "$WINEPREFIX/drive_c/WINCAKE" WINCAKE.EXE "$out/$tag.log" 300 &
sleep 12; xdotool mousemove 510 419 click 1; sleep 3   # the font warning, if any
w=$(xdotool search --name "Cakewalk Professional" | head -1); xdotool windowmap "$w"; sleep 2
xdotool mousemove 500 600 click 1; sleep 0.5; xdotool key r; sleep "$godelay"; touch "$out/go"; sleep "$recs"
xdotool key space; sleep 3
xdotool mousemove 18 40 click 1; sleep 1.5; xdotool mousemove 40 107 click 1; sleep 2.5   # File > Save As
xdotool key ctrl+a; xdotool type --delay 40 "C:\\PROBE\\$tag.WRK"; xdotool key Return; sleep 3
for p in $(pgrep -f '[w]inevdm'); do kill "$p"; done; sleep 2; wineserver -k
