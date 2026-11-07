#! input MTCBRK.TXT
#! secs 120
#! irq
@open
cbyield 1
cbret 0
cbmask 2 merge
echo == the code breaks off after 0 pieces of a group; the next label 2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 0 pieces of a group; the next label 4 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 1 pieces of a group; the next label 0 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 1 pieces of a group; the next label 2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 2 pieces of a group; the next label 0 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 2 pieces of a group; the next label 2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 4 pieces of a group; the next label 0 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 4 pieces of a group; the next label 2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 6 pieces of a group; the next label 0 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 6 pieces of a group; the next label 2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 7 pieces of a group; the next label 2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
echo == the code breaks off after 1 pieces of a group; the next label -2 frames on
@load ENG_ORDR.WRK
?101 call.w SEQSETSRCCLOCK 0003
?!101 call.w SEQSETSRCCLOCK 0001 0003
call.w SEQSETFPS 0001 001e
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
?!300 call.w SEQPLAYSTART cb3
?300 call.w SEQSTART cb3 @0
call.d SEQGETNOW 0001
idle 2400
call.d SEQGETNOW 0001
call.w SEQEXPIRED
@stop
sleep 1000
call.w SEQDESTROYSEQ 0001
@close
