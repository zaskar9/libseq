| Function | 1.01 | 2.10 | 3.00 | Sample arguments (from a program trace) |
|---|---|---|---|---|
| **Lifecycle** | | | | |
| `WEP` | #1: 1 | #1: 1 | #1: 1 |  |
| `SEQGETVERSION` | #2: 0 | #2: 0 | #2: 0 |  |
| `SEQOPEN` | #3: 2 | #3: 2 | #3: 2 | 0000,0000 (3.00) |
| `SEQCLOSE` | #4: 0 | #4: 0 | #4: 0 |  |
| `SEQIDLE` | #5: 0 | #5: 0 | #5: 0 |  |
| `SEQSLEEP` |  | #166: 0 | #167: 0 |  |
| `SEQWAKE` |  | #167: 0 | #168: 0 |  |
| `SEQWIRE` |  | #168: 1 | #169: 1 | 0000 (3.00) |
| `SEQSETBAGGAGE` |  |  | #171: 7 | 0002,0477,0d68,0477,aa4c,0000,0010 (3.00) |
| `SEQSETBAGGAGESTRING` |  |  | #172: 5 |  |
| `SEQGETBAGGAGESIZE` |  |  | #173: 3 |  |
| `SEQGETBAGGAGE` |  |  | #174: 7 | 0002,0477,0d1e,0477,aa2a,0000,0026 (3.00) |
| `SEQGETBAGGAGESTRING` |  |  | #175: 9 |  |
| `SEQGETBAGGAGEDATAPTR` |  |  | #176: 3 | 0002,0477,0d68 (3.00) |
| `SEQCACHEPATCHES` |  |  | #180: 3 | 0000,ffff,0001 (3.00) |
| **Sequences** | | | | |
| `SEQCREATESEQ` | #6: 2 | #6: 2 | #6: 2 | 0000,0000 (3.00) |
| `SEQDESTROYSEQ` | #7: 1 | #7: 1 | #7: 1 | 0001 (3.00) |
| `SEQSETSEQPARAM` | #8: 4 | #8: 4 | #8: 4 | 0000,0016,0000,0001 (3.00) |
| `SEQGETSEQPARAM` | #9: 2 | #9: 2 | #9: 2 | 0002,0018 (3.00) |
| `SEQSWITCHTOSTANDBYSEQ` |  |  | #170: 0 |  |
| **Streams** | | | | |
| `SEQCREATESTREAM` | #10: 3 | #10: 3 | #10: 3 |  |
| `SEQDESTROYSTREAM` | #11: 1 | #11: 1 | #11: 1 |  |
| `SEQENUMSTREAMS` | #12: 5 | #12: 5 | #12: 5 | 0002,02df,00ea,0000,0000 (3.00) |
| `SEQSTREAMLEN` | #16: 1 | #16: 1 | #16: 1 | 0200 (3.00) |
| `SEQSTREAMDUR` | #17: 1 | #17: 1 | #17: 1 | 0200 (3.00) |
| `SEQMEASHASEVTS` | #20: 2 | #20: 2 | #20: 2 | 0200,0001 (3.00) |
| `SEQCLONESTREAM` | #32: 3 | #32: 3 | #32: 3 |  |
| `SEQADOPTSTREAM` | #33: 2 | #33: 2 | #33: 2 |  |
| `SEQMERGESTREAMS` | #34: 7 | #34: 7 | #34: 9 |  |
| `SEQSETSTREAMPARAM` | #40: 4 | #40: 4 | #40: 4 |  |
| `SEQGETSTREAMPARAM` | #41: 2 | #41: 2 | #41: 2 | 0200,000b (3.00) |
| `SEQGETSTREAMALLPARAMS` | #42: 3 | #42: 3 | #42: 3 | 0200,0477,a9de (3.00) |
| `SEQSETSTREAMNAME` | #43: 3 | #43: 3 | #43: 3 |  |
| `SEQGETSTREAMNAME` | #44: 4 | #44: 4 | #44: 4 | 0200,0477,aaf2,0081 (3.00) |
| `SEQENUMRECORDSTREAMS` |  |  | #112: 4 | 061f,0028,0000,0000 (3.00) |
| `SEQCLONESTREAMPARAMS` |  | #163: 2 | #164: 2 |  |
| **Event cursor and edits** | | | | |
| `SEQREGISTEREVENTKIND` | #13: 6 | #13: 6 | #13: 6 | 0477,0028,061f,0010,0000,0000 (3.00) |
| `SEQUNREGISTEREVENTKIND` | #14: 2 | #14: 2 | #14: 2 |  |
| `SEQCALLEVENTPROC` | #15: 4 | #15: 4 | #15: 4 |  |
| `SEQEVTATTIMEGE` | #18: 3 | #18: 3 | #18: 3 | 0200,0000,0000 (2.10) |
| `SEQEVTATTIMEGT` | #19: 3 | #19: 3 | #19: 3 |  |
| `SEQTELL` | #22: 1 | #22: 1 | #22: 1 |  |
| `SEQEOS` | #23: 1 | #23: 1 | #23: 1 |  |
| `SEQSEEK` | #24: 4 | #24: 4 | #24: 4 | 0200,0000,0000,0000 (2.10) |
| `SEQNXTEVT` | #25: 1 | #25: 1 | #25: 1 | 0200 (2.10) |
| `SEQGETEVT` | #26: 3 | #26: 3 | #26: 3 | 0200,041f,751c (2.10) |
| `SEQGETEVTCOPY` | #27: 3 | #27: 3 | #27: 3 |  |
| `SEQPUTEVT` | #28: 3 | #28: 3 | #28: 3 |  |
| `SEQINSEVT` | #29: 3 | #29: 3 | #29: 3 |  |
| `SEQDELEVT` | #30: 1 | #30: 1 | #30: 1 |  |
| `SEQMOVEEVT` | #31: 3 | #31: 3 | #31: 3 |  |
| `SEQINSERTRANGE` | #35: 4 | #35: 4 | #35: ? |  |
| `SEQDELETERANGE` | #36: 5 | #36: 5 | #36: 5 |  |
| `SEQDISPLACEEARLIER` | #37: 5 | #37: 5 | #37: 5 |  |
| `SEQDISPLACELATER` | #38: 5 | #38: 5 | #38: 5 |  |
| `SEQTEMPORALSORT` | #39: 1 | #39: 1 | #39: 1 |  |
| **Time base and tempo map** | | | | |
| `SEQGETMAXPPQ` | #45: 0 | #45: 0 | #45: 0 |  |
| `SEQSETPPQ` | #46: 2 | #46: 2 | #46: 2 |  |
| `SEQRESCALETIME` | #47: 4 | #47: 4 | #47: 4 |  |
| `SEQGETPPQ` | #48: 1 | #48: 1 | #48: 1 | 0002 (3.00) |
| `SEQGETMINTEMPO` | #49: 0 | #49: 0 | #49: 0 |  |
| `SEQGETMAXTEMPO` | #50: 0 | #50: 0 | #50: 0 |  |
| `SEQCLEARTEMPOMAP` | #51: 2 | #51: 2 | #51: 2 |  |
| `SEQRECALCTEMPOMAP` | #52: 1 | #52: 1 | #52: 1 |  |
| `SEQGETTEMPOMAPLEN` | #53: 1 | #53: 1 | #53: 1 |  |
| `SEQTEMPOIXBYTICK` | #54: 3 | #54: 3 | #54: 3 |  |
| `SEQGETTEMPOATIX` | #55: 4 | #55: 4 | #55: 4 |  |
| `SEQGETTEMPO` | #56: 3 | #56: 3 | #56: 3 | 0002,0000,0000 (3.00) |
| `SEQSETTEMPO` | #57: 4 | #57: 4 | #57: 4 |  |
| `SEQINSTEMPO` | #58: 4 | #58: 4 | #58: 4 |  |
| `SEQDELTEMPO` | #59: 3 | #59: 3 | #59: 3 |  |
| `SEQENUMTEMPOS` | #60: 9 | #60: 9 | #60: 9 |  |
| `SEQTEMPOCOPY` | #61: 10 | #61: 10 | #61: 10 |  |
| `SEQTEMPOPASTE` | #62: 8 | #62: 8 | #62: 8 |  |
| `SEQSWAPTEMPOMAPS` | #63: 2 | #63: 2 | #63: 2 |  |
| `SEQGETTEMPORATIO` | #75: 2 | #75: 2 | #75: 2 | 0002,0000 (3.00) |
| `SEQSETTEMPORATIO` | #76: 3 | #76: 3 | #76: 3 |  |
| `SEQSELECTTEMPORATIO` | #77: 2 | #77: 2 | #77: 2 |  |
| `SEQWHICHTEMPORATIO` | #78: 1 | #78: 1 | #78: 1 | 0002 (3.00) |
| `SEQTEMPORATIO2STR` | #79: 3 | #79: 3 | #79: 3 | 0020,041f,7d36 (1.01) |
| **Timecode** | | | | |
| `SEQSETFPS` | #64: 2 | #64: 2 | #64: 2 |  |
| `SEQGETFPS` | #65: 1 | #65: 1 | #65: 1 | 0002 (3.00) |
| `SEQSETFRAMEOFS` | #66: 3 | #66: 3 | #66: 3 |  |
| `SEQGETFRAMEOFS` | #67: 1 | #67: 1 | #67: 1 |  |
| `SEQTICK2FRAME` | #68: 3 | #68: 3 | #68: 3 |  |
| `SEQFRAME2TICKRNDOPT` | #69: 4 | #69: 4 | #69: 4 |  |
| `SEQFRAME2HMSF` | #70: 11 | #70: 11 | #70: 11 |  |
| `SEQHMSF2FRAME` | #71: 6 | #71: 6 | #71: 6 |  |
| `SEQTICK2HMSFSTR` | #72: 5 | #72: 5 | #72: 5 | 0002,0000,0000,0477,a938 (3.00) |
| `SEQHMSFSTR2HMSF` | #73: 10 | #73: 10 | #73: 10 |  |
| `SEQHMSFSTR2MBT` | #74: 9 | #74: 9 | #74: 9 |  |
| **Meter map and positions** | | | | |
| `SEQCLEARMETERMAP` | #80: 3 | #80: 3 | #80: 3 |  |
| `SEQRECALCMETERMAP` | #81: 1 | #81: 1 | #81: 1 |  |
| `SEQGETMETERMAPLEN` | #82: 1 | #82: 1 | #82: 1 | 0001 (2.10) |
| `SEQMETERIXBYTICK` | #83: 3 | #83: 3 | #83: 3 | 0001,0000,0000 (2.10) |
| `SEQGETMETERATIX` | #84: 4 | #84: 4 | #84: 4 | 0001,0000,041f,74a8 (2.10) |
| `SEQGETMETER` | #85: 3 | #85: 3 | #85: 3 | 0002,0000,0000 (3.00) |
| `SEQSETMETER` | #86: 5 | #86: 5 | #86: 5 |  |
| `SEQINSMETER` | #87: 4 | #87: 4 | #87: 4 |  |
| `SEQDELMETER` | #88: 2 | #88: 2 | #88: 2 |  |
| `SEQENUMMETERS` | #89: 7 | #89: 7 | #89: 7 | 0002,0000,ffff,02df,00ba,0000,0000 (2.10) |
| `SEQMETERCOPY` | #90: 8 | #90: 8 | #90: 8 |  |
| `SEQMETERPASTE` | #91: 6 | #91: 6 | #91: 6 |  |
| `SEQSWAPMETERMAPS` | #92: 2 | #92: 2 | #92: 2 |  |
| `SEQMBT2TICK` | #93: 4 | #93: 4 | #93: 4 | 0001,0080,0001,0000 (2.10) |
| `SEQTICK2MBT` | #94: 9 | #94: 9 | #94: 9 | 0002,0000,01e0,041f,7320,041f,732a,041f,7322 (2.10) |
| `SEQTICK2MEAS` | #95: 3 | #95: 3 | #95: 3 | 0002,0000,0000 (3.00) |
| `SEQTICK2MBTSTR` | #96: 6 | #96: 6 | #96: 6 | 0002,0000,0000,0477,a95e,0001 (3.00) |
| `SEQMBTSTR2MBT` | #97: 9 | #97: 9 | #97: 9 |  |
| `SEQTICKNEXTMEAS` | #98: 3 | #98: 3 | #98: 3 | 0002,0000,0000 (2.10) |
| `SEQTICKPREVMEAS` | #99: 3 | #99: 3 | #99: 3 |  |
| `SEQTICKNEXTBEAT` | #100: 3 | #100: 3 | #100: 3 |  |
| `SEQTICKPREVBEAT` | #101: 3 | #101: 3 | #101: 3 |  |
| **Transport** | | | | |
| `SEQSETPLAYINGSEQ` | #102: 1 | #102: 1 | #102: 1 | 0002 (3.00) |
| `SEQSETSTANDBYSEQ` | #103: 1 | #103: 1 | #103: 1 | 0000 (3.00) |
| `SEQSETSRCCLOCK` | #104: 1 | #104: 2 | #104: 2 | 0002,0000 (2.10) |
| `SEQGETSRCCLOCK` | #105: 0 | #105: 1 | #105: 1 | 0002 (3.00) |
| `SEQPLAYSTART` | #106: 2 | #106: 2 |  | 02df,00ba (2.10) |
| `SEQSTART` |  |  | #106: 4 | 061f,0028,0000,0000 (3.00) |
| `SEQPLAYSTOP` | #107: 0 | #107: 0 |  |  |
| `SEQSTOP` |  |  | #107: 0 |  |
| `SEQEXPIRED` | #111: 0 | #111: 0 | #113: 0 |  |
| `SEQREWIND` | #112: 1 | #112: 1 | #114: 1 | 0002 (2.10) |
| `SEQSETNOW` | #113: 3 | #113: 3 | #115: 3 | 0002,0000,01e0 (2.10) |
| `SEQGETNOW` | #114: 1 | #114: 1 | #116: 1 | 0002 (3.00) |
| `SEQGETEND` | #115: 1 | #115: 1 | #117: 1 | 0002 (3.00) |
| `SEQSETDUR` | #116: 3 | #116: 3 | #118: 3 |  |
| `SEQGETDUR` | #117: 1 | #117: 1 | #119: 1 |  |
| **Recording and input** | | | | |
| `SEQRECORDSTART` | #108: 7 | #108: 7 |  |  |
| `SEQRECORD` |  |  | #108: 1 |  |
| `SEQRECORDSTOP` | #109: 0 | #109: 0 |  |  |
| `SEQGETRECORDBUFFER` |  |  | #109: 0 |  |
| `SEQRECORDCLEANUP` | #110: 5 | #110: 5 |  |  |
| `SEQSETRECORDBUFFERPARAMS` |  |  | #110: 3 |  |
| `SEQCANCELLOOPRECORDTAKE` |  |  | #111: 0 |  |
| `SEQSTEPRECSETUP` | #118: 2 | #118: 2 | #120: 2 |  |
| `SEQSTEPRECPARAMS` | #119: 4 | #119: 4 | #121: 4 |  |
| `SEQRECORDEVENT` | #120: 2 | #120: 2 | #122: 2 |  |
| `SEQRECCOUNT` | #121: 0 | #121: 0 | #123: 0 |  |
| `SEQSETRECCOUNT` | #122: 2 | #122: 2 | #124: 2 |  |
| `SEQSTEPRECTERM` | #123: 0 | #123: 0 | #125: 0 |  |
| `SEQTXMIDIMSG` | #124: 5 | #124: 5 | #126: 5 |  |
| `SEQRECEIVESYSXBANK` | #140: 6 | #140: 6 | #142: 6 |  |
| `SEQRECEIVESYSXBANKEX` |  |  | #143: 8 |  |
| `SEQSETMIDIMACRO` |  | #164: 2 | #165: 2 | 0000,ffff (3.00) |
| `SEQCLEARMIDIMACROSHIFT` |  | #165: 0 | #166: 0 |  |
| **Sysex banks** | | | | |
| `SEQCREATESYSXBANK` | #125: 1 | #125: 1 | #127: 1 |  |
| `SEQDESTROYSYSXBANK` | #126: 1 | #126: 1 | #128: 1 |  |
| `SEQEMPTYSYSXBANK` | #127: 1 | #127: 1 | #129: 1 |  |
| `SEQENUMSYSXBANKS` | #128: 5 | #128: 5 | #130: 5 | 0002,02df,00ea,0000,0000 (3.00) |
| `SEQSETSYSXBANKPARAM` | #129: 4 | #129: 4 | #131: 4 |  |
| `SEQGETSYSXBANKPARAM` | #130: 2 | #130: 2 | #132: 2 |  |
| `SEQSETSYSXBANKNAME` | #131: 3 | #131: 3 | #133: 3 |  |
| `SEQGETSYSXBANKNAME` | #132: 4 | #132: 4 | #134: 4 |  |
| `SEQGETSYSXBANKLEN` | #133: 1 | #133: 1 | #135: 1 |  |
| `SEQGETSYSXASCLEN` | #134: 2 | #134: 2 | #136: 2 |  |
| `SEQSYSXBIN2ASC` | #135: 4 | #135: 4 | #137: 4 |  |
| `SEQSYSXASC2BIN` | #136: 5 | #136: 5 | #138: 5 |  |
| `SEQSAVESYSXBANK` | #137: 3 | #137: 3 | #139: 3 |  |
| `SEQLOADSYSXBANK` | #138: 3 | #138: 3 | #140: 3 |  |
| `SEQSENDSYSXBANK` | #139: 3 | #139: 3 | #141: 3 |  |
| **Files** | | | | |
| `SEQLOADCAKEWALKFILE` | #141: 7 | #141: 7 | #144: 3 | 0477,aa74,0000 (3.00) |
| `SEQSAVECAKEWALKFILE` | #142: 9 | #142: 9 | #145: 5 |  |
| `SEQLOADMIDIFILE` | #143: 3 | #143: 3 | #146: 3 |  |
| `SEQSAVEMIDIFILE` | #144: 5 | #144: 5 | #147: 5 |  |
| `SEQIOSEEK` | #145: 4 | #145: 4 | #148: 4 | 0005,0000,0000,0001 (2.10) |
| `SEQIOWRITE` | #146: 4 | #146: 4 | #149: 5 |  |
| `SEQIOREAD` | #147: 4 | #147: 4 | #150: 5 | 0005,041f,73aa,0004 (2.10) |
| `SEQGETFILEERROR` | #148: 0 | #148: 0 | #151: 0 |  |
| **Devices** | | | | |
| `SEQCONFIGMIDI` | #149: 1 | #149: 1 | #152: 1 |  |
| `SEQGETPORTNAME` | #150: 3 | #150: 3 | #153: 3 | 0000,0477,aaf2 (3.00) |
| **Markers** | | | | |
| `SEQNUMMARKERS` | #151: 1 | #151: 1 | #154: 1 | 0002 (3.00) |
| `SEQMARKERFORTIME` | #152: 3 | #152: 3 | #155: 3 | 0002,0000,0000 (1.01) |
| `SEQSETMARKERNAME` | #153: 5 | #153: 5 | #156: 5 |  |
| `SEQGETMARKERNAME` | #154: 6 | #154: 6 | #157: 6 |  |
| `SEQSETMARKERTIME` | #155: 6 | #155: 6 | #158: 6 |  |
| `SEQGETMARKERTIME` | #156: 7 | #156: 7 | #159: 7 | 0002,ffff,ffff,041f,7f18,0000,0000 (1.01) |
| `SEQINSMARKER` | #157: 6 | #157: 6 | #160: 6 |  |
| `SEQDELMARKER` | #158: 3 | #158: 3 | #161: 3 |  |
| `SEQSORTMARKERS` | #159: 3 | #159: 3 | #162: 3 |  |
| `SEQMARKERCOPY` |  |  | #177: 10 |  |
| `SEQMARKERPASTE` |  |  | #178: 9 |  |
| `SEQSWAPMARKERMAPS` |  |  | #179: 2 |  |
| **Internal** | | | | |
| `___EXPORTEDSTUB` | #21: ? | #169: ? | #21: ? |  |
| `DLGPROC` | #163: 5 | #21: ? |  | 0054,0030,004a,0000,0000 (1.01) |
| `SEQHUGEMEMCPY` | #160: 6 | #160: 6 | #163: 6 |  |
| `TIMERTRAP` | #161: ? | #161: ? | #181: ? |  |
| `RXTRAP` | #162: ? | #162: ? | #182: ? |  |
| `?DLGPROC@@ZCHPBUHWND__@@IIJ@Z` |  |  | #183: ? |  |
