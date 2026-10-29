#!/bin/bash
# multi.sh VER NAME "LAYOUT" [SETUP...]: one try of NAME with LAYOUT after the SETUP script lines;
# prints the size the tracer saw (or that it did not return) and the probe's output.
ver=$1; name=$2; layout=$3; shift 3
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
mod=TTSSEQ; [ $ver = 300 ] && mod=TTSSEQ30
load() { if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$1\" 0000"; else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$1\" 0000 null 0000 0000"; fi; }
{
  echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"; echo "call SEQOPEN 0000 0000"
  for s in "$@"; do case "$s" in LOAD:*) load "${s#LOAD:}";; *) echo "$s";; esac; done
  echo "call $name $layout"; echo "echo end"
} > $P/multi.txt
FAKESEQ_LOG=/dev/null TRACE=+snoop $RUNCW $P ttsprobe.exe $TTSPROBE_WORK/multi.log 20 multi.txt multi.out
r=$(grep -m1 "RET  $mod\.[0-9]*: $name(" $TTSPROBE_WORK/multi.log | sed -E 's/.*\((.*)\) retval = (\S+).*/\1 -> \2/')
if [ -n "$r" ]; then echo "$ver $name $(echo "$r" | sed -E 's/ ->.*//' | awk -F, '{print ($0=="")?0:NF}') words [$layout]: ($r)"; else echo "$ver $name: no return [$layout]"; fi
tr -d '\r' < $P/multi.out | grep -E "^callback" | head -4
