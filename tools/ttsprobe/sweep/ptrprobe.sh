#!/bin/bash
# ptrprobe.sh VER NAME...: finds the argument size of functions that take strings or buffers. For
# k = 2..12 words it tries a handle followed by far pointers to a zeroed 8 KB buffer, then pointers
# only, one process per try, until a call returns; the tracer then reports the callee's true size.
# Runs in the second prefix so it does not disturb the sweep.
ver=$1; shift
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
mod=TTSSEQ; [ $ver = 300 ] && mod=TTSSEQ30
out=$TTSPROBE_WORK/sweep/$ver-ptr; mkdir -p $out
tail() { # m words: an odd one becomes a leading 0, the rest far pointers
  local m=$1 s=""; [ $((m % 2)) = 1 ] && s="0"
  for ((i = 0; i < m / 2; i++)); do s="$s @0"; done; echo $s
}
for name in "$@"; do
  h=0002; echo "$name" | grep -qE 'STREAM|EVT|SEEK|TELL|EOS|RANGE|DISPLACE|TEMPORALSORT' && h=0200
  echo "$name" | grep -qE 'SYSX' && h=0000
  found=""
  for k in 2 3 4 5 6 7 8 9 10 11 12; do
    for layout in "$h $(tail $((k - 1)))" "$(tail $k)"; do
      {
        echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
        echo "call SEQOPEN 0000 0000"; echo "call SEQCREATESEQ 0000 0000"
        if [ $ver = 300 ]; then echo 'call SEQLOADCAKEWALKFILE "C:\PROBE\ENG_ORDR.WRK" 0000'
        else echo 'call SEQLOADCAKEWALKFILE "C:\PROBE\ENG_ORDR.WRK" 0000 null 0000 0000'; fi
        echo "buf 0 8192"
        echo "call $name $layout"
        echo "dump 0 0 32"
      } > $P/ptr.txt
      FAKESEQ_LOG=/dev/null TRACE=+snoop $RUNCW $P ttsprobe.exe $out/try.log 15 ptr.txt ptr.out
      if grep -q "RET  $mod\.[0-9]*: $name(" $out/try.log; then
        found=$(grep -m1 "RET  $mod\.[0-9]*: $name(" $out/try.log)
        { echo "# $name: k=$k layout=[$layout]"; grep -E "$mod\.[0-9]+: $name\(" $out/try.log; tr -d '\r' < $P/ptr.out | grep -A3 "^$name\|^dump"; } >> $out/found.txt
        cp $out/try.log $out/$name.log
        break 2
      fi
    done
  done
  [ -z "$found" ] && echo "# $name: no layout returned" >> $out/found.txt
done
