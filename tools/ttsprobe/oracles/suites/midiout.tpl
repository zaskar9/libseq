@open
sleep 300
echo == note on, note off
call.w SEQTXMIDIMSG 0000 0000 0090 003c 0064
call.w SEQTXMIDIMSG 0000 0000 0080 003c 0000
sleep 100
echo == note off for a note that is not on
call.w SEQTXMIDIMSG 0000 0000 0080 003d 0000
sleep 100
echo == note on, note on with velocity 0, note off
call.w SEQTXMIDIMSG 0000 0000 0090 003d 0040
call.w SEQTXMIDIMSG 0000 0000 0090 003d 0000
call.w SEQTXMIDIMSG 0000 0000 0080 003d 0000
sleep 100
echo == the same note twice, then two note offs
call.w SEQTXMIDIMSG 0000 0000 0090 0041 0042
call.w SEQTXMIDIMSG 0000 0000 0090 0041 0043
call.w SEQTXMIDIMSG 0000 0000 0080 0041 0000
call.w SEQTXMIDIMSG 0000 0000 0080 0041 0000
sleep 100
echo == the channel replaces the status nibble
call.w SEQTXMIDIMSG 0000 0005 0093 0040 0040
call.w SEQTXMIDIMSG 0000 0005 0083 0040 0000
sleep 100
echo == controller, program, channel and key pressure
call.w SEQTXMIDIMSG 0000 0003 00b0 0007 0050
call.w SEQTXMIDIMSG 0000 0000 00c0 0005 0000
call.w SEQTXMIDIMSG 0000 0002 00d0 0030 0000
call.w SEQTXMIDIMSG 0000 0002 00a0 0030 0020
sleep 100
echo == the same controller twice
call.w SEQTXMIDIMSG 0000 0000 00b0 0007 0050
call.w SEQTXMIDIMSG 0000 0000 00b0 0007 0050
sleep 100
echo == pitch wheel
call.w SEQTXMIDIMSG 0000 0000 00e0 0000 0040
call.w SEQTXMIDIMSG 0000 0000 00e0 0010 0050
call.w SEQTXMIDIMSG 0000 0000 00e0 2000 0000
call.w SEQTXMIDIMSG 0000 0000 00ef 0010 0020
sleep 100
echo == every channel
call.w SEQTXMIDIMSG 0000 00ff 00c0 0007 0000
sleep 100
echo == every port, channel 3
call.w SEQTXMIDIMSG ffff 0003 00b0 0001 0002
sleep 100
echo == every port and channel: note on, note offs
call.w SEQTXMIDIMSG ffff 00ff 0090 0031 0021
call.w SEQTXMIDIMSG ffff 00ff 0080 0031 0000
call.w SEQTXMIDIMSG ffff 00ff 0080 0032 0000
sleep 100
echo == port 1
call.w SEQTXMIDIMSG 0001 0002 0090 0030 0020
call.w SEQTXMIDIMSG 0001 0002 0080 0030 0000
sleep 100
echo == channel 16
call.w SEQTXMIDIMSG 0000 0010 0090 0020 0020
sleep 100
echo == statuses below 0x80 and above 0xef
call.w SEQTXMIDIMSG 0000 0000 0040 0020 0020
call.w SEQTXMIDIMSG 0000 0000 00f0 0020 0020
call.w SEQTXMIDIMSG 0000 0000 00f8 0000 0000
call.w SEQTXMIDIMSG 0000 0000 00ff 0020 0020
sleep 100
echo == data bytes above 0x7f
call.w SEQTXMIDIMSG 0000 0000 0090 0080 0040
call.w SEQTXMIDIMSG 0000 0000 0090 0040 0080
call.w SEQTXMIDIMSG 0000 0000 00b0 0007 00ff
sleep 100
echo == the panic of Cakewalk Apprentice 1.04 (excerpt)
call.w SEQTXMIDIMSG ffff 00ff 0080 003c 0000
call.w SEQTXMIDIMSG ffff 00ff 00b0 0040 0000
call.w SEQTXMIDIMSG ffff 00ff 00e0 0000 0040
call.w SEQTXMIDIMSG ffff 00ff 00b0 0079 0000
sleep 100
echo == a note left on is turned off at close
call.w SEQTXMIDIMSG 0000 0000 0090 0045 0045
sleep 100
@close
