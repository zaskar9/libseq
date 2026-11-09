#! outputs 2
#! ini PORTS2.INI
#! secs 40
@open
echo == ENG_SYNC: its stream plays on port 1
@load ENG_SYNC.WRK
call.w SEQGETSTREAMPARAM 0100 0003
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
sleep 500
@play
idle 600
@stop
idle 300
echo == the stream on port 2
call.w SEQSETSTREAMPARAM 0100 0003 d:00000001
@play
idle 300
@stop
idle 300
echo == an empty sequence
call.w SEQCREATESEQ 0000 0000
call.w SEQSETPLAYINGSEQ 0002
@play
idle 300
@stop
idle 300
@close
