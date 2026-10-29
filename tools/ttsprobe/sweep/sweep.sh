#!/bin/bash
# sweep.sh VER PASS: calls every export of one DLL version under the tracer, in batches: one process
# runs the remaining calls in order; when one crashes or hangs, the next batch starts after it.
# PASS=zero passes 16 zero words; PASS=ptr passes eight far pointers to a zeroed 4 KB buffer.
. "$(dirname "$0")/../env.sh"
ver=$1; pass=$2
P=$PROBE_C/V$ver
dll=TTSSEQ.DLL; [ $ver = 300 ] && dll=TTSSEQ30.DLL
out=$TTSPROBE_WORK/sweep/$ver-$pass; rm -rf $out; mkdir -p $out
skip='^(WEP|DLGPROC|\?DLGPROC.*|TIMERTRAP|RXTRAP|___EXPORTEDSTUB|SEQCONFIGMIDI)$'
if [ -n "$TODO" ]; then cp "$TODO" $out/todo.txt
else awk '{print $2}' $EXPORTS/exports$ver.txt | grep -vE "$skip" > $out/todo.txt; fi
batch=0
while [ -s $out/todo.txt ]; do
  batch=$((batch+1))
  {
    echo "load $dll"; echo "call SEQGETVERSION"
    [ $ver != 101 ] && echo "call SEQWIRE 0001"
    echo "call SEQOPEN 0000 0000"; echo "call SEQCREATESEQ 0000 0000"
    echo "buf 0 4096"
    if [ ${pass:0:1} = h ]; then
      if [ $ver = 300 ]; then echo 'call SEQLOADCAKEWALKFILE "C:\PROBE\CAKEWALK.WRK" 0000'
      else echo 'call SEQLOADCAKEWALKFILE "C:\PROBE\CAKEWALK.WRK" 0000 null 0000 0000'; fi
    fi
    while read name; do
      echo "echo >> $name"
      if [ $pass = zero ]; then echo "call $name 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"
      elif [ ${pass:0:1} = h ]; then
        # the song loaded above is sequence 2; its first stream is 0x0200
        h=0002; echo "$name" | grep -qE 'STREAM|EVT|SEEK|TELL|EOS|RANGE|DISPLACE|TEMPORALSORT' && ! echo "$name" | grep -qE 'ENUMSTREAMS|CREATESTREAM|MERGESTREAMS|ENUMRECORDSTREAMS' && h=0200
        # Pascal: the callee's parameters are the last words pushed, so with surplus words its
        # first parameter could be any of them; filling all 16 with the handle makes it the handle.
        if [ $pass = hh ]; then echo "call $name $h $h $h $h $h $h $h $h $h $h $h $h $h $h $h $h"
        elif [ $pass = hz ]; then echo "call $name $h 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"
        else echo "call $name $h @0 @0 @0 @0 @0 @0 @0"; fi
      else echo "call $name @0 @0 @0 @0 @0 @0 @0 @0"; fi
    done < $out/todo.txt
    echo "echo done"
  } > $P/sweep.txt
  FAKESEQ_LOG=/dev/null TRACE=+snoop $RUNCW $P ttsprobe.exe $out/batch$batch.log 25 sweep.txt sweep.out
  cp $P/sweep.out $out/batch$batch.out
  tr -d '\r' < $out/batch$batch.out | grep -q '^done' && break
  # the last ">> NAME" marker is the call that did not come back: drop it and everything before
  last=$(tr -d '\r' < $out/batch$batch.out | grep '^>> ' | tail -1 | cut -c4-)
  [ -z "$last" ] && { echo "no progress in batch $batch" >> $out/failed.txt; break; }
  echo "$last" >> $out/failed.txt
  awk -v l="$last" 'f{print} $0==l{f=1}' $out/todo.txt > $out/todo.next; mv $out/todo.next $out/todo.txt
  wineserver -k 2>/dev/null; sleep 1
done
