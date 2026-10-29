@open
buf 0 64
echo == paste at 0x780 (Cakewalk Apprentice 1.04)
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000780
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:ffffffff d:00000780
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == offset 0
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000000
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:ffffffff d:00000000
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == offset 0x78: equal times, a first
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000078
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:ffffffff d:00000078
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == offset 0x10000
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00010000
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:ffffffff d:00010000
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == flag 3 does nothing visible
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0003 d:00000000
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0003 d:ffffffff d:00000000
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == third word 5 does nothing visible
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0005 0100 0000 d:00000780
?300 call.w SEQMERGESTREAMS 0002 0200 0005 0100 0000 d:ffffffff d:00000780
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == new stream in sequence 1
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0001 0200 0001 0100 0000 d:00000780
?300 call.w SEQMERGESTREAMS 0001 0200 0001 0100 0000 d:ffffffff d:00000780
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == swapped: a is the song stream, b the clone
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0100 0002 0200 0000 d:00000078
?300 call.w SEQMERGESTREAMS 0002 0100 0002 0200 0000 d:ffffffff d:00000078
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == flag 1 destroys b too
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0001 d:00000780
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0001 d:ffffffff d:00000780
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == the stream parameters and name of a are kept
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
call.w SEQSETSTREAMPARAM 0200 0001 d:00000005
call.w SEQSETSTREAMNAME 0200 "Stream a"
call.w SEQSETSTREAMPARAM 0100 0002 d:00000003
call.w SEQSETSTREAMNAME 0100 "Stream b"
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000780
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:ffffffff d:00000780
let 1
call.d SEQGETSTREAMPARAM %1 0000
call.d SEQGETSTREAMPARAM %1 0001
call.d SEQGETSTREAMPARAM %1 0002
call.d SEQGETSTREAMPARAM %1 0003
call.d SEQGETSTREAMPARAM %1 0004
call.d SEQGETSTREAMPARAM %1 0005
call.d SEQGETSTREAMPARAM %1 0006
call.d SEQGETSTREAMPARAM %1 0007
call.d SEQGETSTREAMPARAM %1 0008
call.d SEQGETSTREAMPARAM %1 0009
call.d SEQGETSTREAMPARAM %1 000a
call.d SEQGETSTREAMPARAM %1 000b
call.d SEQGETSTREAMPARAM %1 000c
call.d SEQGETSTREAMPARAM %1 000d
call.d SEQGETSTREAMPARAM %1 000e
call.d SEQGETSTREAMPARAM %1 000f
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETSTREAMNAME %1 @0 0081
dump 0 0 32
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == no time-base conversion: sequence 2 at 240 ppq
@load ENG_ORDR.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQCLONESTREAM 0002 0100 0000
call.w SEQDELETERANGE 0200 d:00000002 d:00000013
call.w SEQSETPPQ 0002 00f0
?101,210 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000780
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:ffffffff d:00000780
let 1
call.d SEQSTREAMLEN %1
call.d SEQSEEK %1 0000 d:00000000
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETEVT %1 @0
dump 0 0 8
call.d SEQTELL %1
call.w SEQEOS %1
call.w SEQNXTEVT %1
call.w SEQENUMSTREAMS 0001 cb4 d:00000000
call.w SEQENUMSTREAMS 0002 cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == 3.00: the number of events taken from b
?300 echo -- 00000000
?300 @load ENG_ORDR.WRK
?300 call.w SEQCREATESEQ 0000 0000
?300 call.w SEQCLONESTREAM 0002 0100 0000
?300 call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000000 d:00000780
?300 let 1
?300 call.d SEQSTREAMLEN %1
?300 call.w SEQDESTROYSEQ 0002
?300 call.w SEQDESTROYSEQ 0001
?300 echo -- 00000001
?300 @load ENG_ORDR.WRK
?300 call.w SEQCREATESEQ 0000 0000
?300 call.w SEQCLONESTREAM 0002 0100 0000
?300 call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000001 d:00000780
?300 let 1
?300 call.d SEQSTREAMLEN %1
?300 call.w SEQDESTROYSEQ 0002
?300 call.w SEQDESTROYSEQ 0001
?300 echo -- 00000007
?300 @load ENG_ORDR.WRK
?300 call.w SEQCREATESEQ 0000 0000
?300 call.w SEQCLONESTREAM 0002 0100 0000
?300 call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000007 d:00000780
?300 let 1
?300 call.d SEQSTREAMLEN %1
?300 call.w SEQDESTROYSEQ 0002
?300 call.w SEQDESTROYSEQ 0001
?300 echo -- 0000000a
?300 @load ENG_ORDR.WRK
?300 call.w SEQCREATESEQ 0000 0000
?300 call.w SEQCLONESTREAM 0002 0100 0000
?300 call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:0000000a d:00000780
?300 let 1
?300 call.d SEQSTREAMLEN %1
?300 call.w SEQDESTROYSEQ 0002
?300 call.w SEQDESTROYSEQ 0001
?300 echo -- 00000013
?300 @load ENG_ORDR.WRK
?300 call.w SEQCREATESEQ 0000 0000
?300 call.w SEQCLONESTREAM 0002 0100 0000
?300 call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000013 d:00000780
?300 let 1
?300 call.d SEQSTREAMLEN %1
?300 call.w SEQDESTROYSEQ 0002
?300 call.w SEQDESTROYSEQ 0001
?300 echo -- 00000030
?300 @load ENG_ORDR.WRK
?300 call.w SEQCREATESEQ 0000 0000
?300 call.w SEQCLONESTREAM 0002 0100 0000
?300 call.w SEQDELETERANGE 0200 d:00000002 d:00000013
?300 call.w SEQMERGESTREAMS 0002 0200 0001 0100 0000 d:00000030 d:00000780
?300 let 1
?300 call.d SEQSTREAMLEN %1
?300 call.w SEQDESTROYSEQ 0002
?300 call.w SEQDESTROYSEQ 0001
@close
