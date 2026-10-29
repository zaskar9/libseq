#! input IN3.TXT
#! secs 40
@open
call.w SEQCREATESEQ 0000 0000
call.w SEQSETSEQPARAM 0001 0010 d:00000001
sleep 800
call.w SEQSETPLAYINGSEQ 0001
call.w SEQSETSTANDBYSEQ 0000
cbret 0
cbyield 1
?101,210 call.w SEQRECORDSTART 0000 0000 0000 0000 0000 cb3
?300 call.w SEQRECORD 0001
?300 call.w SEQSTART cb3 0000 0000
?300 call.w SEQGETRECORDBUFFER
?300 call.w SEQSETRECORDBUFFERPARAMS 0000 0000 0000
idle 2500
?101,210 call.w SEQRECORDSTOP
?101,210 call.d SEQRECCOUNT
buf 1 16
?101,210 call.w SEQRECORDCLEANUP 0001 0001 0000 @1
?101,210 dump 1 0 2
?300 call.w SEQGETRECORDBUFFER
?300 call.w SEQRECORD 0000
?300 call.w SEQSTOP
call.d SEQSTREAMLEN 0100
buf 0 16
call.d SEQSEEK 0100 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT 0100 @0
dump 0 0 8
call.d SEQTELL 0100
call.w SEQEOS 0100
call.w SEQNXTEVT 0100
call.d SEQGETSEQPARAM 0001 0003
@close
