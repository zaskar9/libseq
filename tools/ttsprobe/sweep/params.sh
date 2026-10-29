#!/bin/bash
# params.sh VER SONG...: loads each song and prints SeqGetSeqParam(seq, p) for p = 0..63, to match
# the sequence parameters against the settings stored in the file (VARS).
ver=$1; shift
. "$(dirname "$0")/../env.sh"
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
out=$TTSPROBE_WORK/params/$ver; mkdir -p $out
for song in "$@"; do
  {
    echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
    echo "call SEQOPEN 0000 0000"
    if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song.WRK\" 0000"
    else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song.WRK\" 0000 null 0000 0000"; fi
    for ((p = 0; p < 64; p++)); do printf 'call SEQGETSEQPARAM 0001 %04x\n' $p; done
    echo "call SEQCLOSE"
  } > $P/params.txt
  FAKESEQ_LOG=/dev/null TRACE=-all $RUNCW $P ttsprobe.exe $out/$song.log 30 params.txt params.out
  tr -d '\r' < $P/params.out | sed -nE 's/^SEQGETSEQPARAM\(0001,([0-9a-f]+)\) -> (.*)$/\1 \2/p' > $out/$song.txt
done
