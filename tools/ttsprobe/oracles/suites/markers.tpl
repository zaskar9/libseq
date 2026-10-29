@open
buf 0 64
@load SILENT.WRK
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == rename 0
call.w SEQSETMARKERNAME 0001 d:00000000 "Renamed"
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == retime 1
call.w SEQSETMARKERTIME 0001 d:00000001 d:00000123 0000
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == retime 0 with flag 1
call.w SEQSETMARKERTIME 0001 d:00000000 d:00000010 0001
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == insert at 0x300
call.w SEQINSMARKER 0001 "New" d:00000300 0000
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == insert at 0 with flag 1
call.w SEQINSMARKER 0001 "First" d:00000000 0001
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == insert at 0x300 again
call.w SEQINSMARKER 0001 "Same time" d:00000300 0000
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == sort
call.w SEQSORTMARKERS 0001 d:00000000
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == delete 1
call.w SEQDELMARKER 0001 d:00000001
call.d SEQNUMMARKERS 0001
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000000 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000000 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000001 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000001 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000002 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000002 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000003 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000003 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000004 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000004 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000005 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000005 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000006 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000006 @0 null
dump 0 0 4
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQGETMARKERNAME 0001 d:00000007 @0 0020
dump 0 0 32
fill 0 0 ee ee ee ee ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 @0+4
dump 0 0 8
fill 0 0 ee ee ee ee
call.w SEQGETMARKERTIME 0001 d:00000007 @0 null
dump 0 0 4
call.d SEQMARKERFORTIME 0001 d:00000000
call.d SEQMARKERFORTIME 0001 d:00000001
call.d SEQMARKERFORTIME 0001 d:00000077
call.d SEQMARKERFORTIME 0001 d:00000078
call.d SEQMARKERFORTIME 0001 d:000000ef
call.d SEQMARKERFORTIME 0001 d:000000f0
call.d SEQMARKERFORTIME 0001 d:000001df
call.d SEQMARKERFORTIME 0001 d:000001e0
call.d SEQMARKERFORTIME 0001 d:000001e1
call.d SEQMARKERFORTIME 0001 d:000003c0
call.d SEQMARKERFORTIME 0001 d:000005a0
call.d SEQMARKERFORTIME 0001 d:00000777
call.d SEQMARKERFORTIME 0001 d:00001000
call.d SEQMARKERFORTIME 0001 d:00002345
call.d SEQMARKERFORTIME 0001 d:00010000
echo == other fixtures
@load AMAZING.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load CLIP.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E2_CHAS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E2_KORD.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E2_METR.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E2_OVLP.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E2_SPP.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E2_STOP.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_DENS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_MA.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_MB.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_MC.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_MD.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_ME.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_MF.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_SHTL.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load E3_SPPD.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load EFILT.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_CHAS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_LOOP.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_METR.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_MULT.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_ORDR.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_PARM.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_SYNC.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_SYSX.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ENG_TMPO.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load FRERE.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load FURELISE.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load GREENS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load JINGLE.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ODETOJOY.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load QUANT.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load ROWBOAT.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SILENT.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_DENS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_KIND.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_MAPS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_MIN.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_OVLP.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_PARM.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_STRS.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load SYN_WAVE.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load TWINKLE.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@load XPOSE.WRK
call.d SEQNUMMARKERS 0002
call.w SEQDESTROYSEQ 0002
@close
