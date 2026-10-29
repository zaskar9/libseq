#!/bin/bash
# exact.sh VER SONG SPEC...: loads SONG (C:\PROBE\SONG), then tries each SPEC = "NAME|layout|layout..."
# one layout at a time (one process per try) until the call returns; prints what the tracer saw.
# A layout uses ttsprobe's argument syntax; "seek" before it positions stream 0x0100 at event 0.
ver=$1; song=$2; shift 2
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
mod=TTSSEQ; [ $ver = 300 ] && mod=TTSSEQ30
out=$TTSPROBE_WORK/sweep/$ver-exact; mkdir -p $out
for spec in "$@"; do
  name=${spec%%|*}; rest=${spec#*|}
  IFS='|' read -ra layouts <<< "$rest"
  ok=""
  for layout in "${layouts[@]}"; do
    {
      echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
      echo "call SEQOPEN 0000 0000"
      if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song\" 0000"
      else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song\" 0000 null 0000 0000"; fi
      echo "buf 0 4096"; echo "put 0 0 \"Probe name\""
      echo "call SEQSEEK 0100 0000 d:00000000"
      echo "call $name $layout"
      echo "dump 0 0 32"; echo "strr"
    } > $P/exact.txt
    FAKESEQ_LOG=/dev/null TRACE=+snoop $RUNCW $P ttsprobe.exe $out/try.log 20 exact.txt exact.out
    if grep -q "RET  $mod\.[0-9]*: $name(" $out/try.log; then
      ok=$(grep -m1 "RET  $mod\.[0-9]*: $name(" $out/try.log | sed -E 's/.*\((.*)\) retval = (\S+).*/(\1) -> \2/')
      words=$(grep -m1 "RET  $mod\.[0-9]*: $name(" $out/try.log | sed -E 's/.*\((.*)\) retval.*/\1/' | awk -F, '{print ($0=="")?0:NF}')
      echo "$ver $name $words words, layout [$layout]: $ok"
      cp $out/try.log $out/$name.log; tr -d '\r' < $P/exact.out > $out/$name.out
      break
    fi
  done
  [ -z "$ok" ] && echo "$ver $name: no layout returned"
done
