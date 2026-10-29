#!/bin/bash
# varsmap.sh VER: loads V_BASE and the 89 one-byte VARS variants in C:\PROBE\VARS
# (analysis/mkvariants.py) in one process and dumps
# SeqGetSeqParam(seq, 0..63) for each (the sequence is destroyed after each dump).
ver=$1
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
{
  echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
  echo "call SEQOPEN 0000 0000"
  for f in BASE $(seq -w 0 88); do
    echo "echo === $f"
    if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\VARS\\V_$f.WRK\" 0000"
    else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\VARS\\V_$f.WRK\" 0000 null 0000 0000"; fi
    for ((p = 0; p < 64; p++)); do printf 'call SEQGETSEQPARAM 0001 %04x\n' $p; done
    echo "call SEQDESTROYSEQ 0001"
  done
  echo "call SEQCLOSE"
} > $P/varsmap.txt
FAKESEQ_LOG=/dev/null TRACE=-all $RUNCW $P ttsprobe.exe $TTSPROBE_WORK/varsmap-$ver.log 300 varsmap.txt varsmap.out
mkdir -p $TTSPROBE_WORK/params; tr -d '\r' < $P/varsmap.out > $TTSPROBE_WORK/params/varsmap-$ver.out
