#!/bin/bash
# spptest.sh: which system messages each version sends when playback starts, from Wine's raw MIDI trace.
. "$(dirname "$0")/../env.sh"
for ver in 101 210 300; do
  P=$PROBE_C/V$ver
  dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
  for case in "E2_SPP 1" "E2_SPP 0" "ENG_ORDR 1" "ENG_SYNC 1"; do
    set -- $case; song=$1; spp=$2
    {
      echo "load $dll"; [ $ver != 101 ] && echo "call SEQWIRE 0001"
      echo "call SEQOPEN 0000 0000"
      if [ $ver = 300 ]; then echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song.WRK\" 0000"
      else echo "call SEQLOADCAKEWALKFILE \"C:\\PROBE\\$song.WRK\" 0000 null 0000 0000"; fi
      echo "call SEQSETSEQPARAM 0001 0003 d:0000000$spp"
      echo "call SEQSETPLAYINGSEQ 0001"; echo "call SEQSETSTANDBYSEQ 0000"
      echo "sleep 500"
      if [ $ver = 300 ]; then echo "call SEQSTART null 0000 0000"; else echo "call SEQPLAYSTART null"; fi
      echo "idle 700"
      if [ $ver = 300 ]; then echo "call SEQSTOP"; else echo "call SEQPLAYSTOP"; fi
      echo "idle 300"; echo "call SEQCLOSE"
    } > $P/spp.txt
    FAKESEQ_LOG=/dev/null TRACE=+midi $RUNCW $P ttsprobe.exe $TTSPROBE_WORK/spp-$ver-$song-$spp.log 30 spp.txt spp.out
    msgs=$(python3 -c "import re,sys
for m in re.findall(r\"midi_out_data \\(0000, ([0-9A-F]+)\\)\", open(sys.argv[1]).read()):
  v=int(m,16); s=v&255
  if s>=0xf0 and s!=0xf8: print(f\"{s:02x} {(v>>8)&255:02x} {(v>>16)&255:02x}\", end=\" | \")" $TTSPROBE_WORK/spp-$ver-$song-$spp.log)
    echo "$ver $song spp=$spp: $msgs"
  done
done
