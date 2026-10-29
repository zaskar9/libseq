@open
buf 0 64
buf 1 64
echo -- ENG_TMPO
@load ENG_TMPO.WRK
echo fps 0018
call.w SEQSETFPS 0001 0018
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 0019
call.w SEQSETFPS 0001 0019
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001d
call.w SEQSETFPS 0001 001d
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001e
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001f
call.w SEQSETFPS 0001 001f
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo offsets
call.w SEQSETFRAMEOFS 0001 d:00000000
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQSETFRAMEOFS 0001 d:00000064
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQSETFRAMEOFS 0001 d:0001a5e0
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQDESTROYSEQ 0001
echo -- ENG_ORDR
@load ENG_ORDR.WRK
echo fps 0018
call.w SEQSETFPS 0001 0018
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 0019
call.w SEQSETFPS 0001 0019
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001d
call.w SEQSETFPS 0001 001d
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001e
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001f
call.w SEQSETFPS 0001 001f
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo offsets
call.w SEQSETFRAMEOFS 0001 d:00000000
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQSETFRAMEOFS 0001 d:00000064
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQSETFRAMEOFS 0001 d:0001a5e0
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQDESTROYSEQ 0001
echo -- SYN_MAPS
@load SYN_MAPS.WRK
echo fps 0018
call.w SEQSETFPS 0001 0018
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 0019
call.w SEQSETFPS 0001 0019
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001d
call.w SEQSETFPS 0001 001d
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001e
call.w SEQSETFPS 0001 001e
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo fps 001f
call.w SEQSETFPS 0001 001f
call.w SEQGETFPS 0001
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:00000000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000001
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000001 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000077
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000077 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000078
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000078 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000ef
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000ef @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000000f0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000000f0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001df
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001df @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000001e1
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e1 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000003c0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000003c0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:000005a0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000005a0 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00000777
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00000777 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00001000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00001000 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00002345
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00002345 @1
dump 1 0 16
call.d SEQTICK2FRAME 0001 d:00010000
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:00010000 @1
dump 1 0 16
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000000 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000000 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000001 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000001 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001d 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001d @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000001e 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000001e @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003b 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003b @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0000003c 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0000003c @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000707 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000707 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00000708 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00000708 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:0001a574 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:0001a574 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0000
call.d SEQFRAME2TICKRNDOPT 0001 d:00012345 0001
fill 0 0 ee ee ee ee ee ee ee ee 
call.d SEQFRAME2HMSF 0001 d:00012345 @0 @0+2 @0+4 @0+6
dump 0 0 8
call.d SEQHMSF2FRAME 0001 0000 0000 0000 0000 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0001 0000 0000
call.d SEQHMSF2FRAME 0001 0001 0002 0003 0004 0000
call.d SEQHMSF2FRAME 0001 0000 003b 003b 001d 0000
call.d SEQHMSF2FRAME 0001 0000 0000 0000 001d 0000
call.d SEQHMSF2FRAME 0001 0017 0000 0000 0000 0000
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:02:00" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:02:00" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "01:02:03:04" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "01:02:03:04" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "1:2:3:4" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "1:2:3:4" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:00:00:29" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:00:00:29" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "00:01" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "00:01" @0 @0+2 @0+4
dump 0 0 6
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2HMSF "x" @0 @0+2 @0+4 @0+6
dump 0 0 8
fill 0 0 ee ee ee ee ee ee ee ee 
call.w SEQHMSFSTR2MBT 0001 "x" @0 @0+2 @0+4
dump 0 0 6
echo offsets
call.w SEQSETFRAMEOFS 0001 d:00000000
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQSETFRAMEOFS 0001 d:00000064
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQSETFRAMEOFS 0001 d:0001a5e0
call.d SEQGETFRAMEOFS 0001
call.d SEQTICK2FRAME 0001 d:000001e0
fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQTICK2HMSFSTR 0001 d:000001e0 @1
dump 1 0 16
call.w SEQDESTROYSEQ 0001
@close
