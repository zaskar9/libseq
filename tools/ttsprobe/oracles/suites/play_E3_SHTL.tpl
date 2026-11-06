#! secs 31
@open
@load E3_SHTL.WRK
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
call.d SEQGETNOW 0001
sleep 2500
@play
idle 11870
@close
