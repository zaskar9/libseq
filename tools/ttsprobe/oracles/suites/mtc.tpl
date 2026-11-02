#! input MTC.TXT
#! secs 60
#! irq
@open
buf 0 16
cbyield 1
cbret 0
cbmask 2 merge
echo == 25 fps code, the sequence at 25 fps
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 0019
call.w SEQGETFPS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
idle 200
@stop
sleep 600
call.w SEQDESTROYSEQ 0001
echo == offset 01:00:00:00, code 01:02:03:04: the callback's third word is hours * 256 + minutes
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.d SEQHMSF2FRAME 0001 0001 0000 0000 0000 0000
let 3
call.w SEQSETFRAMEOFS 0001 %d3
call.d SEQGETFRAMEOFS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
idle 200
@stop
sleep 600
call.w SEQDESTROYSEQ 0001
echo == 30 fps drop frame, code 00:10:00:00: the code counts as its label, 600 s
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001d
call.w SEQGETFPS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
idle 200
@stop
sleep 600
call.w SEQDESTROYSEQ 0001
echo == a tempo map (ENG_TMPO), code 00:00:10:00: Now as SeqFrame2TickRndOpt gives it
@load ENG_TMPO.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
call.d SEQFRAME2TICKRNDOPT 0001 d:0000016f 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000170 0000
idle 200
@stop
sleep 600
call.w SEQDESTROYSEQ 0001
echo == offset 00:00:20:00, code from 00:00:17:00: the chase goes on until the code reaches the offset; Now 0
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.d SEQHMSF2FRAME 0001 0000 0000 0014 0000 0000
let 3
call.w SEQSETFRAMEOFS 0001 %d3
call.d SEQGETFRAMEOFS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
idle 200
@stop
sleep 2700
call.w SEQDESTROYSEQ 0001
echo == 25 fps code, the sequence at 30 fps: ignored; the callback gives up
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
cbretafter 5500 1
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
cbret 0
sleep 300
call.w SEQDESTROYSEQ 0001
echo == code 00:34:06:00, no offset: too far after the offset; ignored; the callback gives up
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
cbretafter 5500 1
fill 0 0 ee ee ee ee
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
?300 dump 0 0 4
cbret 0
sleep 300
call.w SEQDESTROYSEQ 0001
@close
