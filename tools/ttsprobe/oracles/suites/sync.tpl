#! input SYNC.TXT
#! secs 40
#! irq
@open
buf 0 16
@load QUANT.WRK
echo == clock source values
?101 call.w SEQSETSRCCLOCK 0001
?!101 call.w SEQSETSRCCLOCK 0001 0001
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
?101 call.w SEQSETSRCCLOCK 0002
?!101 call.w SEQSETSRCCLOCK 0001 0002
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
?101 call.w SEQSETSRCCLOCK 0004
?!101 call.w SEQSETSRCCLOCK 0001 0004
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
?101 call.w SEQSETSRCCLOCK 0103
?!101 call.w SEQSETSRCCLOCK 0001 0103
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
?101 call.w SEQSETSRCCLOCK ffff
?!101 call.w SEQSETSRCCLOCK 0001 ffff
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
?101 call.w SEQSETSRCCLOCK 0000
?!101 call.w SEQSETSRCCLOCK 0001 0000
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
echo == MIDI sync: wait for Start (clocks at 1500 to 1600 do not end it)
?101 call.w SEQSETSRCCLOCK 0001
?!101 call.w SEQSETSRCCLOCK 0001 0001
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
call.w SEQEXPIRED
@play
call.d SEQGETNOW 0001
echo == Start at 2000, two clocks: ticks 0 to 9 are played; Now is the time of the last event
echo == played, else 5 ticks more than before
idle 400
call.d SEQGETNOW 0001
echo == six more clocks
idle 500
call.d SEQGETNOW 0001
echo == 28 clocks at twice the rate
idle 1000
call.d SEQGETNOW 0001
call.w SEQEXPIRED
echo == Stop at 4000
idle 300
call.d SEQGETNOW 0001
call.w SEQEXPIRED
echo == Continue at 4500 without a Song Position: from where it stopped; four clocks; Stop
@play
call.d SEQGETNOW 0001
idle 450
call.d SEQGETNOW 0001
idle 300
call.d SEQGETNOW 0001
echo == Song Position 480 at 5100 while stopped; Continue at 6200; two clocks
@play
call.d SEQGETNOW 0001
idle 280
call.d SEQGETNOW 0001
echo == Song Position 0 at 6500 while playing; two clocks; Stop at 6800
idle 220
call.d SEQGETNOW 0001
idle 300
call.d SEQGETNOW 0001
echo == Continue at 7800: from the Song Position kept; four clocks; Stop at 8200
@play
call.d SEQGETNOW 0001
idle 350
call.d SEQGETNOW 0001
idle 200
call.d SEQGETNOW 0001
echo == Start at 8600 while the start callback polls; four clocks; Stop
cbret 0
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 450
call.d SEQGETNOW 0001
idle 250
?300 dump 0 0 4
echo == Stop at 9400 while waiting is kept: the Start at 9600 stops at once
@play
call.d SEQGETNOW 0001
idle 300
call.d SEQGETNOW 0001
call.w SEQEXPIRED
echo == MIDI Time Code from 00:00:10:00 at 10300 (ENG_ORDR, which ends before 00:00:10:00):
echo == the start returns 2 s after the code locks, at the time of the code
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0002 0003
?101 call.w SEQGETSRCCLOCK
?!101 call.w SEQGETSRCCLOCK 0002
call.w SEQSETPLAYINGSEQ 0002
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART null
?300 call.w SEQSTART null @0
call.d SEQGETNOW 0002
?300 dump 0 0 4
echo == the code stops at 13800; the engine runs on for 0.5 s, then stops
idle 2500
call.d SEQGETNOW 0002
call.w SEQEXPIRED
echo == the callback gives up at once
cbret 1
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.w SEQEXPIRED
@close
