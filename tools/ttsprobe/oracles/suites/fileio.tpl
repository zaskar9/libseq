@open
buf 1 128
buf 2 16
buf 3 4
cbmask 6
cbret 0
?101,210 echo == load SYN_MAPS: seek at the start, read the comments chunk (8) and take it
?101,210 cbdo cb4 fe call.d SEQIOSEEK %w1 d:00000000 0001
?101,210 cbdo cb4 fe call.d SEQIOSEEK %w1 d:00000000 0002
?101,210 cbdo cb4 fe call.d SEQIOSEEK %w1 d:0000000b 0000
?101,210 cbdo cb4 08 call.d SEQIOSEEK %w1 d:00000000 0001
?101,210 cbdo cb4 08 fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
?101,210 cbdo cb4 08 call.w SEQIOREAD %w1 @1 0004
?101,210 cbdo cb4 08 call.w SEQIOREAD %w1 @1+4 0002
?101,210 cbdo cb4 08 call.w SEQIOREAD %w1 @1+6 %b1+4
?101,210 cbdo cb4 08 dump 1 0 96
?101,210 cbdo cb4 08 ret 1
?101,210 call.w SEQLOADCAKEWALKFILE "C:\PROBE\ORC\SYN_MAPS.WRK" 0000 cb4 d:00001234
?101,210 cbdo -
?101,210 call.w SEQGETMETERMAPLEN 0001
?101,210 echo == save it with a comments chunk of its own, written at the end (0xFF)
?101,210 fill 2 0 08
?101,210 fill 2 1 49 00 00 00
?101,210 fill 3 0 47 00
?101,210 put 1 0 "Comments written by the probe, through SeqIOWrite in the save callback."
?101,210 cbret 1
?101,210 cbdo cb4 ff call.d SEQIOSEEK %w1 d:00000000 0001
?101,210 cbdo cb4 ff call.w SEQIOWRITE %w1 @2 0001
?101,210 cbdo cb4 ff call.w SEQIOWRITE %w1 @2+1 0004
?101,210 cbdo cb4 ff call.w SEQIOWRITE %w1 @3 0002
?101,210 cbdo cb4 ff call.w SEQIOWRITE %w1 @1 0047
?101,210 cbdo cb4 ff call.d SEQIOSEEK %w1 d:00000000 0001
?101,210 call.w SEQSAVECAKEWALKFILE 0001 "C:\PROBE\ORC\OUT\IOSAVE.WRK" d:00000000 cb4 d:00005678
?101,210 cbdo -
?101,210 cbret 0
?101,210 call.w SEQDESTROYSEQ 0001
?101,210 echo == load the saved file and read its comments
?101,210 cbdo cb4 08 fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
?101,210 cbdo cb4 08 call.w SEQIOREAD %w1 @1 0004
?101,210 cbdo cb4 08 call.w SEQIOREAD %w1 @1+4 0002
?101,210 cbdo cb4 08 call.w SEQIOREAD %w1 @1+6 %b1+4
?101,210 cbdo cb4 08 dump 1 0 96
?101,210 cbdo cb4 08 ret 1
?101,210 call.w SEQLOADCAKEWALKFILE "C:\PROBE\ORC\OUT\IOSAVE.WRK" 0000 cb4 d:00001234
?101,210 cbdo -
?101,210 call.w SEQDESTROYSEQ 0001
?101,210 echo == load SYN_MAPS leaving every chunk to the engine
?101,210 call.w SEQLOADCAKEWALKFILE "C:\PROBE\ORC\SYN_MAPS.WRK" 0000 cb4 d:00001234
?101,210 call.w SEQGETMETERMAPLEN 0001
?101,210 call.w SEQDESTROYSEQ 0001
?101,210 echo == string events: the event procedure reads each text with the file of the load callback
?101,210 call.w SEQREGISTEREVENTKIND "MCICmd" cb4 d:00000011
?101,210 call.w SEQREGISTEREVENTKIND "Text" cb4 d:00000022
?101,210 cbdo cb4 fe letw 1 1
?101,210 cbdo cb4 04 fill 1 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
?101,210 cbdo cb4 04 call.w SEQIOREAD %1 @1 0004
?101,210 cbdo cb4 04 call.w SEQIOREAD %1 @1+4 %b1+0
?101,210 cbdo cb4 04 dump 1 0 32
?101,210 cbdo cb4 04 ret 1
?101,210 call.w SEQLOADCAKEWALKFILE "C:\PROBE\ORC\SYN_KIND.WRK" 0000 cb4 d:00001234
?101,210 cbdo -
?101,210 call.w SEQENUMSTREAMS 0001 cb4 d:00000000
?101,210 call.d SEQSTREAMLEN 0100
?101,210 call.d SEQSTREAMLEN 0101
?101,210 call.w SEQDESTROYSEQ 0001
?101,210 call.w SEQUNREGISTEREVENTKIND "MCICmd"
?101,210 call.w SEQUNREGISTEREVENTKIND "Text"
@close
