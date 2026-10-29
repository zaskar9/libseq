#! secs 50
@open
@load FURELISE.WRK
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
call.d SEQGETNOW 0001
sleep 2500
@play
idle 30000
@close
