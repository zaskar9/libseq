#! secs 60
# SeqOpen and SeqClose count: the port opens when the count becomes 1 and closes when it becomes 0. 2.10 and 3.00: SeqWire
# again, and SeqSleep and SeqWake, which close and reopen the port.
@open
echo == open twice: the second SeqClose closes the port
call.w SEQCREATESEQ 0000 0000
sleep 10
call.w SEQOPEN 0000 0000
sleep 10
call.w SEQCLOSE
sleep 10
call.w SEQTXMIDIMSG 0000 0000 0090 003c 0040
call.w SEQCREATESEQ 0000 0000
call.w SEQCLOSE
sleep 10
call.w SEQTXMIDIMSG 0000 0000 0090 003d 0040
call.w SEQCREATESEQ 0000 0000
echo == a third SeqClose counts too: the next SeqOpen opens nothing, the one after does
call.w SEQCLOSE
sleep 10
call.w SEQOPEN 0000 0000
sleep 10
call.w SEQTXMIDIMSG 0000 0000 0090 003e 0040
call.w SEQOPEN 0000 0000
sleep 10
call.w SEQTXMIDIMSG 0000 0000 0090 0046 0040
?210,300 echo == SeqWire while open
?210,300 call.w SEQWIRE 0000
?210,300 call.w SEQTXMIDIMSG 0000 0000 0090 003f 0040
?210,300 call.w SEQWIRE 0001
?210,300 call.w SEQWIRE 0002
?210,300 call.w SEQWIRE ffff
?210,300 call.w SEQTXMIDIMSG 0000 0000 0090 0040 0040
?210,300 echo == sleep: the port closes; wake: it opens again
?210,300 sleep 10
?210,300 call.w SEQSLEEP
?210,300 sleep 10
?210,300 call.w SEQTXMIDIMSG 0000 0000 0090 0041 0040
?210,300 call.w SEQSLEEP
?210,300 sleep 10
?210,300 call.w SEQWAKE
?210,300 sleep 10
?210,300 call.w SEQTXMIDIMSG 0000 0000 0090 0042 0040
?210,300 call.w SEQWAKE
?210,300 echo == not while playing
?210,300 @load E2_OVLP.WRK
?210,300 let 1
?210,300 call.w SEQSETPLAYINGSEQ %1
?210,300 @play
?210,300 sleep 700
?210,300 call.w SEQSLEEP
?210,300 sleep 300
?210,300 call.d SEQGETNOW %1
?210,300 @stop
?210,300 echo == a start while asleep waits for the wake
?210,300 call.d SEQSETNOW %1 d:00000000
?210,300 call.w SEQSLEEP
?210,300 sleep 10
?210,300 @play
?210,300 call.w SEQEXPIRED
?210,300 sleep 300
?210,300 call.d SEQGETNOW %1
?210,300 call.w SEQWAKE
?210,300 sleep 300
?210,300 call.d SEQGETNOW %1
?210,300 @stop
?210,300 echo == after such a wake the clock runs at 100 bpm until the next tempo change (at tick 480)
?210,300 @load ENG_TMPO.WRK
?210,300 let 2
?210,300 call.w SEQSETPLAYINGSEQ %2
?210,300 call.d SEQSETNOW %2 d:00000168
?210,300 call.w SEQSLEEP
?210,300 @play
?210,300 call.w SEQWAKE
?210,300 sleep 300
?210,300 call.d SEQGETNOW %2
?210,300 sleep 300
?210,300 call.d SEQGETNOW %2
?210,300 sleep 400
?210,300 call.d SEQGETNOW %2
?210,300 @stop
?210,300 call.w SEQSETPLAYINGSEQ %1
?210,300 echo == not when opened twice
?210,300 call.w SEQOPEN 0000 0000
?210,300 call.w SEQSLEEP
?210,300 call.w SEQWAKE
?210,300 @play
?210,300 sleep 100
?210,300 call.w SEQSLEEP
?210,300 @stop
?210,300 call.w SEQCLOSE
?210,300 echo == SeqClose while asleep, then a wake
?210,300 call.w SEQSLEEP
?210,300 sleep 10
?210,300 call.w SEQCLOSE
?210,300 sleep 10
?210,300 call.w SEQSLEEP
?210,300 call.w SEQWAKE
?210,300 sleep 10
?210,300 call.w SEQTXMIDIMSG 0000 0000 0090 0043 0040
?210,300 call.w SEQWAKE
?210,300 call.w SEQSLEEP
?210,300 sleep 10
?210,300 call.w SEQOPEN 0000 0000
?210,300 sleep 10
?210,300 call.w SEQTXMIDIMSG 0000 0000 0090 0044 0040
?210,300 call.w SEQSLEEP
?210,300 call.w SEQWAKE
?210,300 sleep 10
@close
sleep 10
call.w SEQTXMIDIMSG 0000 0000 0090 0045 0040
call.w SEQCLOSE
