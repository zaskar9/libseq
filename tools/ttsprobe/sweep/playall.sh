#!/bin/bash
# playall.sh VER SONG...: plays each song from tick 0 to its end on one DLL version through the probe
# (no Cakewalk program involved) and keeps the fake port's log in midicap's format.
# EXTRA: script lines run after loading (\\n between lines); TAG: suffix for the output folder.
ver=$1; shift
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
out=$TTSPROBE_WORK/plays/$ver${TAG:+-$TAG}; mkdir -p $out
for song in "$@"; do
  {
    echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
    echo "call SEQOPEN 0000 0000"
    echo "echo --- load"
    if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song.WRK\" 0000"
    else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song.WRK\" 0000 null 0000 0000"; fi
    echo "call SEQSETPLAYINGSEQ \$ax"; echo "call SEQSETSTANDBYSEQ 0000"
    [ -n "$EXTRA" ] && printf '%b\n' "$EXTRA"
    echo "sleep 2500"
    if [ $ver = 300 ]; then echo "call SEQSTART null 0000 0000"; else echo "call SEQPLAYSTART null"; fi
    echo "idle ${SECS:-12}000"
    echo "call SEQCLOSE"
  } > $P/play.txt
  : > $out/$song.fake
  FAKESEQ_LOG=$out/$song.fake TRACE=-all $RUNCW $P ttsprobe.exe $out/$song.log 40 play.txt play.out
  tr -d '\r' < $P/play.out > $out/$song.out
  python3 $TTSPROBE/analysis/fake2cap.py $out/$song.fake $out/$song.txt
done
