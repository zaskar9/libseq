# Design

libseq has two interfaces to one engine.

- **`include/libseq/ttsseq.h`, the C interface**, is `TTSSEQ.DLL` 1.01's own. It has one function
  per export, with the DLL's names, argument order and meanings, so a program written for the
  DLL reads the same against libseq, and the interface itself documents what the DLL offered.
- **`include/libseq/engine.hpp`, the C++ interface**, is the modern one: `libseq::Engine` with
  the sequences, streams, maps and banks it holds as objects, callbacks as `std::function`, and
  the system around it as a `libseq::Platform`. The C interface is a thin layer over it
  (`src/ttsseq.cpp`).

Both reproduce the DLL in all tested cases: the oracle suites (`tests/data/ttsseq/101`) run
through the C interface (`tests/runner/`), and a small C program (`tests/c/ttsseq_c.c`) checks
that it compiles and links as C.

## The C interface

### Types

| DLL | C |
|---|---|
| word | `uint16_t`; `int16_t` where the value is signed (a key; `SeqReceiveSysxBank`'s -2 and -4) |
| double word | `uint32_t`; `int32_t` where the value is signed (parameters, `SeqIOSeek`'s offset) |
| far pointer | a pointer: `char*` for strings, `uint16_t*` and `uint32_t*` for words written back, `SEQEVENT*` for an event, `void*` for memory and windows |
| sequence, stream, sysex bank handle | `HSEQ`, `HSTREAM`, `HSYSXBANK` (words) |
| the 8-byte event record | `SEQEVENT`: a 24-bit time, status, two data bytes, a 16-bit duration, in the record's byte order |
| a callback | a C function pointer, one type per kind (`SEQENUMPROC`, `SEQSTARTPROC`, ...) |
| a file name | a path on the host, `const char*` |

The DLL exports its names in capitals, as the Pascal calling convention of 16-bit Windows does;
the header uses the mixed case of the documentation (`SEQGETVERSION` is `SeqGetVersion`).

Results have the width the DLL returns (`signatures.md`). Some DLL results carry more than the
function computes: a register the function did not clear. `SeqGetTempo` and `SeqMeterIxByTick`,
read as double words, have the tick's high word in DX, and `SeqStepRecParams`, which returns
nothing, leaves the velocity in AX. These are not part of the C interface; the script runner adds
them (`tests/runner/`), since the suites read them.

The DLL gives a callback no context of its own, and neither does the C interface. The enumerations
and the file callbacks pass on the program's double word, as the DLL does; a program that needs
more keeps it where the callback can find it.

### One engine

Windows loaded a DLL once for all programs, so there was one engine. The C interface has one engine
per process, created by the first call. A handle the engine does not hold makes the DLL crash or
answer garbage; the C interface answers 0, or does nothing.

### What Windows did

The DLL had its timer, its MIDI devices, its options and its clock from Windows. The functions and
types whose names begin with `LibSeq` stand in for that:

| Windows | libseq |
|---|---|
| `TTSSEQ.INI`'s `[MIDI Output Devices]` and `[Options]` | `LIBSEQPLATFORM.outputPorts`, `outputName`, `options` (`LIBSEQOPTIONS`) |
| `midiOutShortMsg`, `midiOutLongMsg` | `LIBSEQPLATFORM.send` |
| `timeGetTime`, `GetCurrentTime` | `LIBSEQPLATFORM.milliseconds` |
| `timeSetEvent`, `timeKillEvent`; the timer calling `TIMERTRAP` | `LIBSEQPLATFORM.startTimer`, `stopTimer`; the host calls `LibSeqTimer` |
| `midiInOpen`, `midiInClose`; the driver calling `RXTRAP` | `LIBSEQPLATFORM.opened`, `closed`; the host calls `LibSeqReceive` |
| the program's message loop during a wait | `LIBSEQPLATFORM.wait` |
| `PostMessage` (step recording) | `LIBSEQPLATFORM.postMessage` |
| loading and unloading the DLL (`WEP`) | the first call; `LibSeqUnload` |

A host installs its platform with `LibSeqSetPlatform` before `SeqOpen`.

## The export map

All 163 exports of `TTSSEQ.DLL` 1.01 (`docs/reverse-engineering/exports/exports101.txt`), with the
C function that stands for each and what it uses of the C++ interface. Six exports have no
function: Windows plumbing, and the device dialog, which belongs to the application.

| Ordinal | Export | C | C++ |
|---|---|---|---|
| 1 | `WEP` | (none) | The Windows exit procedure, which Windows calls when it unloads the DLL. `LibSeqUnload` does what unloading did. |
| 2 | `SEQGETVERSION` | `SeqGetVersion` | `Engine::kVersion` |
| 3 | `SEQOPEN` | `SeqOpen` | `Engine::open` |
| 4 | `SEQCLOSE` | `SeqClose` | `Engine::close` |
| 5 | `SEQIDLE` | `SeqIdle` | `Engine::idle` |
| 6 | `SEQCREATESEQ` | `SeqCreateSeq` | `Engine::createSequence` |
| 7 | `SEQDESTROYSEQ` | `SeqDestroySeq` | `Engine::destroySequence` |
| 8 | `SEQSETSEQPARAM` | `SeqSetSeqParam` | `Engine::setSeqParam` |
| 9 | `SEQGETSEQPARAM` | `SeqGetSeqParam` | `Sequence::param` |
| 10 | `SEQCREATESTREAM` | `SeqCreateStream` | `Sequence::createStream` |
| 11 | `SEQDESTROYSTREAM` | `SeqDestroyStream` | `Engine::destroyStream` |
| 12 | `SEQENUMSTREAMS` | `SeqEnumStreams` | `Sequence::streams` |
| 13 | `SEQREGISTEREVENTKIND` | `SeqRegisterEventKind` | `Engine::registerEventKind` |
| 14 | `SEQUNREGISTEREVENTKIND` | `SeqUnregisterEventKind` | `Engine::unregisterEventKind` |
| 15 | `SEQCALLEVENTPROC` | `SeqCallEventProc` | `Engine::callEventProc` |
| 16 | `SEQSTREAMLEN` | `SeqStreamLen` | `Stream::events` |
| 17 | `SEQSTREAMDUR` | `SeqStreamDur` | `Stream::duration` |
| 18 | `SEQEVTATTIMEGE` | `SeqEvtAtTimeGE` | `Stream::indexAtOrAfter` |
| 19 | `SEQEVTATTIMEGT` | `SeqEvtAtTimeGT` | `Stream::indexAfter` |
| 20 | `SEQMEASHASEVTS` | `SeqMeasHasEvts` | `MeterMap::toTick`, `Stream::hasEventsIn` |
| 21 | `___EXPORTEDSTUB` | (none) | By its name a stub of the compiler's runtime library. No traced program imports it. |
| 22 | `SEQTELL` | `SeqTell` | `Stream::cursor` |
| 23 | `SEQEOS` | `SeqEos` | `Stream::cursor` |
| 24 | `SEQSEEK` | `SeqSeek` | `Stream::cursor` |
| 25 | `SEQNXTEVT` | `SeqNxtEvt` | `Stream::cursor` |
| 26 | `SEQGETEVT` | `SeqGetEvt` | `Stream::cursor` |
| 27 | `SEQGETEVTCOPY` | `SeqGetEvtCopy` | as `SeqGetEvt` |
| 28 | `SEQPUTEVT` | `SeqPutEvt` | `Stream::put` |
| 29 | `SEQINSEVT` | `SeqInsEvt` | `Stream::insert` |
| 30 | `SEQDELEVT` | `SeqDelEvt` | `Stream::erase` |
| 31 | `SEQMOVEEVT` | `SeqMoveEvt` | `Stream::move` |
| 32 | `SEQCLONESTREAM` | `SeqCloneStream` | `Sequence::clone` |
| 33 | `SEQADOPTSTREAM` | `SeqAdoptStream` | `Engine::adoptStream` |
| 34 | `SEQMERGESTREAMS` | `SeqMergeStreams` | `Engine::mergeStreams` |
| 35 | `SEQINSERTRANGE` | `SeqInsertRange` | `Stream::insertRange` |
| 36 | `SEQDELETERANGE` | `SeqDeleteRange` | `Stream::eraseRange` |
| 37 | `SEQDISPLACEEARLIER` | `SeqDisplaceEarlier` | `Stream::displaceEarlier` |
| 38 | `SEQDISPLACELATER` | `SeqDisplaceLater` | `Stream::displaceLater` |
| 39 | `SEQTEMPORALSORT` | `SeqTemporalSort` | `Stream::sort` |
| 40 | `SEQSETSTREAMPARAM` | `SeqSetStreamParam` | `Engine::setStreamParam` |
| 41 | `SEQGETSTREAMPARAM` | `SeqGetStreamParam` | `Stream::param` |
| 42 | `SEQGETSTREAMALLPARAMS` | `SeqGetStreamAllParams` | `Stream::allParams` |
| 43 | `SEQSETSTREAMNAME` | `SeqSetStreamName` | `Stream::setName` |
| 44 | `SEQGETSTREAMNAME` | `SeqGetStreamName` | `Stream::name` |
| 45 | `SEQGETMAXPPQ` | `SeqGetMaxPPQ` | `kMaxPpq` |
| 46 | `SEQSETPPQ` | `SeqSetPPQ` | `Sequence::setPpq` |
| 47 | `SEQRESCALETIME` | `SeqRescaleTime` | `rescaleTime` |
| 48 | `SEQGETPPQ` | `SeqGetPPQ` |  |
| 49 | `SEQGETMINTEMPO` | `SeqGetMinTempo` | `kMinTempo` |
| 50 | `SEQGETMAXTEMPO` | `SeqGetMaxTempo` | `kMaxTempo` |
| 51 | `SEQCLEARTEMPOMAP` | `SeqClearTempoMap` | `TempoMap::clear` |
| 52 | `SEQRECALCTEMPOMAP` | `SeqRecalcTempoMap` | `TempoMap::recalc` |
| 53 | `SEQGETTEMPOMAPLEN` | `SeqGetTempoMapLen` | `TempoMap::size` |
| 54 | `SEQTEMPOIXBYTICK` | `SeqTempoIxByTick` | `TempoMap::indexAt` |
| 55 | `SEQGETTEMPOATIX` | `SeqGetTempoAtIx` | `TempoMap::entries` |
| 56 | `SEQGETTEMPO` | `SeqGetTempo` | `TempoMap::tempoAt` |
| 57 | `SEQSETTEMPO` | `SeqSetTempo` | `TempoMap::set` |
| 58 | `SEQINSTEMPO` | `SeqInsTempo` | `TempoMap::insert` |
| 59 | `SEQDELTEMPO` | `SeqDelTempo` | `TempoMap::remove` |
| 60 | `SEQENUMTEMPOS` | `SeqEnumTempos` | `TempoMap::enumCount` |
| 61 | `SEQTEMPOCOPY` | `SeqTempoCopy` | `TempoMap::copyFrom` |
| 62 | `SEQTEMPOPASTE` | `SeqTempoPaste` | `TempoMap::paste` |
| 63 | `SEQSWAPTEMPOMAPS` | `SeqSwapTempoMaps` | `Sequence::tempoMap` (swapped) |
| 64 | `SEQSETFPS` | `SeqSetFps` | `Sequence::setFps` |
| 65 | `SEQGETFPS` | `SeqGetFps` | `Sequence::fps` |
| 66 | `SEQSETFRAMEOFS` | `SeqSetFrameOfs` | `Sequence::frameOffset` |
| 67 | `SEQGETFRAMEOFS` | `SeqGetFrameOfs` | `Sequence::frameOffset` |
| 68 | `SEQTICK2FRAME` | `SeqTick2Frame` | `Sequence::tickToFrame` |
| 69 | `SEQFRAME2TICKRNDOPT` | `SeqFrame2TickRndOpt` | `Sequence::frameToTick` |
| 70 | `SEQFRAME2HMSF` | `SeqFrame2HMSF` | `framesToHMSF` |
| 71 | `SEQHMSF2FRAME` | `SeqHMSF2Frame` | `hmsfToFrames` |
| 72 | `SEQTICK2HMSFSTR` | `SeqTick2HMSFStr` | `Sequence::tickToFrame`, `formatHMSF` |
| 73 | `SEQHMSFSTR2HMSF` | `SeqHMSFStr2HMSF` | `parseHMSF` |
| 74 | `SEQHMSFSTR2MBT` | `SeqHMSFStr2MBT` | `parseHMSF`, `Sequence::frameToTick`, `MeterMap::toMBT` |
| 75 | `SEQGETTEMPORATIO` | `SeqGetTempoRatio` | `Sequence::tempoRatios` |
| 76 | `SEQSETTEMPORATIO` | `SeqSetTempoRatio` | `Sequence::tempoRatios` |
| 77 | `SEQSELECTTEMPORATIO` | `SeqSelectTempoRatio` | `Sequence::tempoRatio` |
| 78 | `SEQWHICHTEMPORATIO` | `SeqWhichTempoRatio` | `Sequence::tempoRatio` |
| 79 | `SEQTEMPORATIO2STR` | `SeqTempoRatio2Str` | `tempoRatioString` |
| 80 | `SEQCLEARMETERMAP` | `SeqClearMeterMap` | `MeterMap::clear` |
| 81 | `SEQRECALCMETERMAP` | `SeqRecalcMeterMap` | `MeterMap::recalc` |
| 82 | `SEQGETMETERMAPLEN` | `SeqGetMeterMapLen` | `MeterMap::size` |
| 83 | `SEQMETERIXBYTICK` | `SeqMeterIxByTick` | `MeterMap::indexAtTick` |
| 84 | `SEQGETMETERATIX` | `SeqGetMeterAtIx` | `MeterMap::entries` |
| 85 | `SEQGETMETER` | `SeqGetMeter` | `MeterMap::indexAtTick` |
| 86 | `SEQSETMETER` | `SeqSetMeter` | `MeterMap::set` |
| 87 | `SEQINSMETER` | `SeqInsMeter` | `MeterMap::insert` |
| 88 | `SEQDELMETER` | `SeqDelMeter` | `MeterMap::remove` |
| 89 | `SEQENUMMETERS` | `SeqEnumMeters` | `MeterMap::enumCount` |
| 90 | `SEQMETERCOPY` | `SeqMeterCopy` | `MeterMap::copyFrom` |
| 91 | `SEQMETERPASTE` | `SeqMeterPaste` | `MeterMap::paste` |
| 92 | `SEQSWAPMETERMAPS` | `SeqSwapMeterMaps` | `Sequence::meterMap` (swapped) |
| 93 | `SEQMBT2TICK` | `SeqMBT2Tick` | `MeterMap::toTick` |
| 94 | `SEQTICK2MBT` | `SeqTick2MBT` | `MeterMap::toMBT` |
| 95 | `SEQTICK2MEAS` | `SeqTick2Meas` | `MeterMap::toMBT` |
| 96 | `SEQTICK2MBTSTR` | `SeqTick2MBTStr` | `MeterMap::toMBT`, `formatMBT` |
| 97 | `SEQMBTSTR2MBT` | `SeqMBTStr2MBT` | `parseMBT` |
| 98 | `SEQTICKNEXTMEAS` | `SeqTickNextMeas` | `MeterMap::nextMeasure` |
| 99 | `SEQTICKPREVMEAS` | `SeqTickPrevMeas` | `MeterMap::prevMeasure` |
| 100 | `SEQTICKNEXTBEAT` | `SeqTickNextBeat` | `MeterMap::nextBeat` |
| 101 | `SEQTICKPREVBEAT` | `SeqTickPrevBeat` | `MeterMap::prevBeat` |
| 102 | `SEQSETPLAYINGSEQ` | `SeqSetPlayingSeq` | `Engine::setPlayingSeq` |
| 103 | `SEQSETSTANDBYSEQ` | `SeqSetStandbySeq` | `Engine::setStandbySeq` |
| 104 | `SEQSETSRCCLOCK` | `SeqSetSrcClock` | `Engine::setSrcClock` |
| 105 | `SEQGETSRCCLOCK` | `SeqGetSrcClock` | `Engine::srcClock` |
| 106 | `SEQPLAYSTART` | `SeqPlayStart` | `Engine::playStart` |
| 107 | `SEQPLAYSTOP` | `SeqPlayStop` | `Engine::playStop` |
| 108 | `SEQRECORDSTART` | `SeqRecordStart` | `Engine::recordStart` |
| 109 | `SEQRECORDSTOP` | `SeqRecordStop` | `Engine::recordStop` |
| 110 | `SEQRECORDCLEANUP` | `SeqRecordCleanup` | `Engine::recordCleanup` |
| 111 | `SEQEXPIRED` | `SeqExpired` | `Engine::expired` |
| 112 | `SEQREWIND` | `SeqRewind` | `Engine::setNow` (0) |
| 113 | `SEQSETNOW` | `SeqSetNow` | `Engine::setNow` |
| 114 | `SEQGETNOW` | `SeqGetNow` | `Sequence::now` |
| 115 | `SEQGETEND` | `SeqGetEnd` | `Sequence::end` |
| 116 | `SEQSETDUR` | `SeqSetDur` | `Sequence::dur` |
| 117 | `SEQGETDUR` | `SeqGetDur` | `Sequence::dur` |
| 118 | `SEQSTEPRECSETUP` | `SeqStepRecSetup` | `Engine::stepRecSetup` |
| 119 | `SEQSTEPRECPARAMS` | `SeqStepRecParams` | `Engine::stepRecParams` |
| 120 | `SEQRECORDEVENT` | `SeqRecordEvent` | `Engine::recordEvent` |
| 121 | `SEQRECCOUNT` | `SeqRecCount` | `Engine::recCount` |
| 122 | `SEQSETRECCOUNT` | `SeqSetRecCount` | `Engine::setRecCount` |
| 123 | `SEQSTEPRECTERM` | `SeqStepRecTerm` | `Engine::stepRecTerm` |
| 124 | `SEQTXMIDIMSG` | `SeqTxMidiMsg` | `Engine::txMidiMsg` |
| 125 | `SEQCREATESYSXBANK` | `SeqCreateSysxBank` | `Sequence::createSysxBank` |
| 126 | `SEQDESTROYSYSXBANK` | `SeqDestroySysxBank` | `Sequence::sysxBanks` |
| 127 | `SEQEMPTYSYSXBANK` | `SeqEmptySysxBank` | `SysxBank::data` |
| 128 | `SEQENUMSYSXBANKS` | `SeqEnumSysxBanks` | `Sequence::sysxBanks` |
| 129 | `SEQSETSYSXBANKPARAM` | `SeqSetSysxBankParam` | `SysxBank::setParam` |
| 130 | `SEQGETSYSXBANKPARAM` | `SeqGetSysxBankParam` | `SysxBank::param` |
| 131 | `SEQSETSYSXBANKNAME` | `SeqSetSysxBankName` | `SysxBank::name` |
| 132 | `SEQGETSYSXBANKNAME` | `SeqGetSysxBankName` | `SysxBank::name` |
| 133 | `SEQGETSYSXBANKLEN` | `SeqGetSysxBankLen` | `SysxBank::data` |
| 134 | `SEQGETSYSXASCLEN` | `SeqGetSysxAscLen` | `sysxToText` |
| 135 | `SEQSYSXBIN2ASC` | `SeqSysxBin2Asc` | `sysxToText` |
| 136 | `SEQSYSXASC2BIN` | `SeqSysxAsc2Bin` | `sysxFromText` |
| 137 | `SEQSAVESYSXBANK` | `SeqSaveSysxBank` | `SysxBank::data` to a file |
| 138 | `SEQLOADSYSXBANK` | `SeqLoadSysxBank` | `SysxBank::data` from a file |
| 139 | `SEQSENDSYSXBANK` | `SeqSendSysxBank` | `Engine::sendSysxBank` |
| 140 | `SEQRECEIVESYSXBANK` | `SeqReceiveSysxBank` | `Engine::receiveSysxBank` |
| 141 | `SEQLOADCAKEWALKFILE` | `SeqLoadCakewalkFile` | `Engine::loadCakewalkFile` |
| 142 | `SEQSAVECAKEWALKFILE` | `SeqSaveCakewalkFile` | `Engine::saveCakewalkFile` |
| 143 | `SEQLOADMIDIFILE` | `SeqLoadMidiFile` | `Engine::loadMidiFile` |
| 144 | `SEQSAVEMIDIFILE` | `SeqSaveMidiFile` | `Engine::saveMidiFile` |
| 145 | `SEQIOSEEK` | `SeqIOSeek` | `Engine::ioSeek` |
| 146 | `SEQIOWRITE` | `SeqIOWrite` | `Engine::ioWrite` |
| 147 | `SEQIOREAD` | `SeqIORead` | `Engine::ioRead` |
| 148 | `SEQGETFILEERROR` | `SeqGetFileError` | `Engine::fileError` |
| 149 | `SEQCONFIGMIDI` | (none) | The device dialog, which writes `TTSSEQ.INI` (`ttsseq-api.md`, Devices). It belongs to the application: a host lists its devices through `LIBSEQPLATFORM.outputPorts` and `outputName`, and its options through `options`. |
| 150 | `SEQGETPORTNAME` | `SeqGetPortName` | `Engine::portName` |
| 151 | `SEQNUMMARKERS` | `SeqNumMarkers` | `Sequence::markers` |
| 152 | `SEQMARKERFORTIME` | `SeqMarkerForTime` | `Sequence::markerForTime` |
| 153 | `SEQSETMARKERNAME` | `SeqSetMarkerName` | `Sequence::markers` |
| 154 | `SEQGETMARKERNAME` | `SeqGetMarkerName` | `Sequence::markers` |
| 155 | `SEQSETMARKERTIME` | `SeqSetMarkerTime` | `Sequence::setMarkerTime` |
| 156 | `SEQGETMARKERTIME` | `SeqGetMarkerTime` | `Sequence::markerTime` |
| 157 | `SEQINSMARKER` | `SeqInsMarker` | `Sequence::insertMarker` |
| 158 | `SEQDELMARKER` | `SeqDelMarker` | `Sequence::markers` |
| 159 | `SEQSORTMARKERS` | `SeqSortMarkers` | `Sequence::sortMarkers` |
| 160 | `SEQHUGEMEMCPY` | `SeqHugeMemCpy` | `std::memmove` |
| 161 | `TIMERTRAP` | `LibSeqTimer` | The multimedia timer's callback, exported because Windows calls it. The host calls `LibSeqTimer` instead. |
| 162 | `RXTRAP` | `LibSeqReceive` | The MIDI input callback, exported because Windows calls it. The host calls `LibSeqReceive` instead. |
| 163 | `DLGPROC` | (none) | By its name the procedure of `SeqConfigMidi`'s dialog, exported because Windows calls it. |

## Versions

Each release implements one engine version (`README.md`). The C interface follows: at the tag of
a version, `ttsseq.h` has that version's exports, and the history between the tags shows each
function as it was added, changed or dropped.
