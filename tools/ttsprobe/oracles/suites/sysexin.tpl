#! input SYSX.TXT
#! secs 45
@open
buf 0 192
cbmask c
cbyield 1
@load ENG_SYSX.WRK
call.w SEQCREATESYSXBANK 0001
let 1
echo == receive until about 8 s
cbret 0
cbretafter 7000 1
call.w SEQRECEIVESYSXBANK %1 cb4 0000 0000 0000
call.d SEQGETSYSXBANKLEN %1
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQSYSXBIN2ASC %1 @0 0010
dump 0 0 128
echo == receive again into the same bank, until about 13 s
cbret 0
cbretafter 4500 1
call.w SEQRECEIVESYSXBANK %1 cb4 0000 0000 0000
call.d SEQGETSYSXBANKLEN %1
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQSYSXBIN2ASC %1 @0 0010
dump 0 0 128
echo == 3.00: SeqReceiveSysxBankEx into a new bank, until about 18 s
?300 call.w SEQCREATESYSXBANK 0001
?300 let 1
?300 cbret 0
?300 cbretafter 4500 1
?300 call.w SEQRECEIVESYSXBANKEX %1 cb4 0000 0000 0000 0000 0000
?300 call.d SEQGETSYSXBANKLEN %1
?300 fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
?300 call.w SEQSYSXBIN2ASC %1 @0 0010
?300 dump 0 0 128
echo == the callback says stop at once
cbret 1
call.w SEQRECEIVESYSXBANK %1 cb4 0000 0000 0000
call.d SEQGETSYSXBANKLEN %1
fill 0 0 ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee ee 
call.w SEQSYSXBIN2ASC %1 @0 0010
dump 0 0 128
cbmask 0
call.w SEQENUMSYSXBANKS 0001 cb4 d:00000000
@close
