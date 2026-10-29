#!/bin/bash
# trkmap.sh VER: for T_BASE and each track-record variant in C:\PROBE\TRK (analysis/mkvariants.py;
# one process each), dumps SeqGetStreamParam(0x0100, 0..31) after loading.
ver=$1
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
mkdir -p $TTSPROBE_WORK/params; : > $TTSPROBE_WORK/params/trkmap-$ver.out
for f in T_BASE $(cd $PROBE_C/TRK && ls T[0-9]*.WRK | sed "s/\.WRK$//"); do
  {
    echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
    echo "call SEQOPEN 0000 0000"
    echo "echo === $f"
    if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\TRK\\$f.WRK\" 0000"
    else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\TRK\\$f.WRK\" 0000 null 0000 0000"; fi
    for ((p = 0; p < 32; p++)); do printf 'call SEQGETSTREAMPARAM 0100 %04x\n' $p; done
    echo "call SEQCLOSE"
  } > $P/trkmap.txt
  FAKESEQ_LOG=/dev/null TRACE=-all $RUNCW $P ttsprobe.exe $TTSPROBE_WORK/trkmap-$ver.log 20 trkmap.txt trkmap.out
  tr -d '\r' < $P/trkmap.out >> $TTSPROBE_WORK/params/trkmap-$ver.out
done
