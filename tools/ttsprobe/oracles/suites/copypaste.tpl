@open
buf 0 64
buf 1 64
echo == tempo copy 0..0x780, paste at 0x780 (Apprentice): ENG_TMPO
@load ENG_TMPO.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQTEMPOCOPY 0002 0001 d:00000000 d:00000780 d:00000000 @1
dump 1 0 8
call.w SEQTEMPOPASTE 0001 0002 d:00000780 d:00000781 0001 0000
call.w SEQGETTEMPOMAPLEN 0001
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0001 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0001
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0001 0000 ffff cb4 d:00000000
call.w SEQGETTEMPOMAPLEN 0002
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0002 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0002
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0002 0000 ffff cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == tempo copy 0..0x780, paste at 0x780 (Apprentice): SYN_MAPS
@load SYN_MAPS.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQTEMPOCOPY 0002 0001 d:00000000 d:00000780 d:00000000 @1
dump 1 0 8
call.w SEQTEMPOPASTE 0001 0002 d:00000780 d:00000781 0001 0000
call.w SEQGETTEMPOMAPLEN 0001
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0001 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0001
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0001 0000 ffff cb4 d:00000000
call.w SEQGETTEMPOMAPLEN 0002
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0002 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0002
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0002 0000 ffff cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == tempo copy 0..0x3c0, paste at 0x1e0 with slide (Professional): ENG_TMPO
@load ENG_TMPO.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQTEMPOCOPY 0002 0001 d:00000000 d:000003c0 d:00000000 @1
dump 1 0 8
call.w SEQTEMPOPASTE 0001 0002 d:000001e0 d:000003c1 0001 0001
call.w SEQGETTEMPOMAPLEN 0001
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0001 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0001
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0001 0000 ffff cb4 d:00000000
call.w SEQGETTEMPOMAPLEN 0002
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0002 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0002
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0002 0000 ffff cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == tempo copy 0..0x3c0, paste at 0x1e0 with slide (Professional): SYN_MAPS
@load SYN_MAPS.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQTEMPOCOPY 0002 0001 d:00000000 d:000003c0 d:00000000 @1
dump 1 0 8
call.w SEQTEMPOPASTE 0001 0002 d:000001e0 d:000003c1 0001 0001
call.w SEQGETTEMPOMAPLEN 0001
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0001 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0001
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0001 0000 ffff cb4 d:00000000
call.w SEQGETTEMPOMAPLEN 0002
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0002 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0002
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0002 0000 ffff cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == meter copy 0..0x780, paste at 0x780: ENG_TMPO
@load ENG_TMPO.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQMETERCOPY 0002 0001 d:00000000 d:00000780 @1
dump 1 0 8
call.w SEQMETERPASTE 0001 0002 d:00000780 0001 0000
call.w SEQGETTEMPOMAPLEN 0001
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0001 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0001
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0001 0000 ffff cb4 d:00000000
call.w SEQGETTEMPOMAPLEN 0002
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0002 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0002
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0002 0000 ffff cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
echo == meter copy 0..0x780, paste at 0x780: SYN_MAPS
@load SYN_MAPS.WRK
call.w SEQCREATESEQ 0000 0000
call.w SEQMETERCOPY 0002 0001 d:00000000 d:00000780 @1
dump 1 0 8
call.w SEQMETERPASTE 0001 0002 d:00000780 0001 0000
call.w SEQGETTEMPOMAPLEN 0001
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0001 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0001 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0001
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0001 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0001 0000 ffff cb4 d:00000000
call.w SEQGETTEMPOMAPLEN 0002
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0005 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0006 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.w SEQGETTEMPOATIX 0002 0007 @0
dump 0 0 4
call.w SEQENUMTEMPOS 0002 d:00000000 d:ffffffff cb4 d:00000000
call.w SEQGETMETERMAPLEN 0002
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0000 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0001 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0002 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0003 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0004 @0
dump 0 0 4
fill 0 0 ee ee ee ee
call.d SEQGETMETERATIX 0002 0005 @0
dump 0 0 4
call.w SEQENUMMETERS 0002 0000 ffff cb4 d:00000000
call.w SEQDESTROYSEQ 0002
call.w SEQDESTROYSEQ 0001
@close
