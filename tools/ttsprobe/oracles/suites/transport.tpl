#! secs 60
@open
@load ENG_ORDR.WRK
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
call.d SEQGETNOW 0001
call.d SEQGETEND 0001
call.d SEQGETDUR 0001
echo == set now
call.d SEQSETNOW 0001 d:000001e0
call.d SEQGETNOW 0001
call.d SEQSETDUR 0001 d:00000100
call.d SEQGETDUR 0001
call.d SEQSETDUR 0001 d:00000000
echo == play from 0x1e0 for 1.5 s, stop
sleep 1000
@play
idle 1500
@stop
idle 1000
echo == continue to the end
@play
idle 6000
echo == rewind, play from 0 for 1.75 s (between two notes), stop
call.d SEQREWIND 0001
call.d SEQGETNOW 0001
sleep 1000
@play
idle 1750
@stop
idle 1000
@close
