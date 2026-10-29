# TTSSEQ call layouts (as used by the oracle suites)

Found by calling each function through `ttsprobe` until a layout returned and behaved sensibly
(1.01; the same in 2.10 and 3.00 unless noted). Arguments in declaration order; `d` is a 32-bit
value (two words), `p` a far pointer (two words). Result: `w` word in AX, `d` dword in DX:AX, from
the `dxs` sentinel runs (`widths.py`). "?" marks a layout that returns but whose meaning is not
established.

## Sequences and parameters

| Call | Result | Notes |
|---|---|---|
| `SeqGetVersion()` | w | 0x0101, 0x020A, 0x0300 |
| `SeqOpen(0, 0)` | w | |
| `SeqClose()` | w | |
| `SeqCreateSeq(0, 0)` | w | the lowest free handle |
| `SeqDestroySeq(seq)` | w | an invalid handle crashes |
| `SeqSetSeqParam(seq, param, d value)` | w | 1, or 0 for a parameter the version does not have |
| `SeqGetSeqParam(seq, param)` | d | |

## Streams and events

| Call | Result | Notes |
|---|---|---|
| `SeqCreateStream(seq, ?, ?)` | w | a new stream in the sequence |
| `SeqAdoptStream(seq, stream)` | w | returns a new handle for the stream |
| `SeqCloneStream(seq, stream, flag)` | w | flag 0 copies the events, 1 does not |
| `SeqDestroyStream(stream)` | w | |
| `SeqEnumStreams(seq, p callback, d value)` | d | callback(seq, stream, d value) per stream, then (0, 0, d value) |
| `SeqStreamLen(stream)`, `SeqStreamDur(stream)` | d | |
| `SeqMeasHasEvts(stream, measure)` | d | |
| `SeqGet/SetStreamParam(stream, param[, d value])` | d / w | |
| `SeqGetStreamName(stream, p buffer, size)` | w | returns 1 |
| `SeqSetStreamName(stream, p name)` | w | |
| `SeqGetStreamAllParams(stream, p buffer)` | d | 0x16 bytes |
| `SeqSeek(stream, 0, d index)` | d | |
| `SeqTell(stream)` | d | |
| `SeqEos(stream)` | w | |
| `SeqNxtEvt(stream)` | d | |
| `SeqGetEvt(stream, p event)`, `SeqGetEvtCopy(stream, p event)` | w | 8-byte event |
| `SeqPutEvt(stream, p event)`, `SeqInsEvt(stream, p event)` | w | at the cursor |
| `SeqDelEvt(stream)` | w | at the cursor |
| `SeqMoveEvt(stream, d tick)` | d | returns the new index |
| `SeqEvtAtTimeGE/GT(stream, d tick)` | d | an index |
| `SeqDeleteRange(stream, d from, d thru)` | d | |
| `SeqDisplaceEarlier/Later(stream, d from, d amount)` | d | |
| `SeqTemporalSort(stream)` | d | |
| `SeqInsertRange(stream, ?, d ?)` | d | ? returns 0 |
| `SeqMergeStreams` | | layout not found (7 words in 1.01/2.10, 9 in 3.00) |
| `SeqRegisterEventKind(p name, p proc, d value)` | w | the kind (1, 2, ...); the name is kept, not copied |
| `SeqUnregisterEventKind(p name)` | w | |
| `SeqCallEventProc(w, p, w)` | w | ? returns 0 |
| event procedure | | proc(message, p data, length): 4 while loading, 3 on a failed load, 1 while playing |

## Time base, tempo and ratios

| Call | Result | Notes |
|---|---|---|
| `SeqGetMaxPPQ()`, `SeqGetMinTempo()`, `SeqGetMaxTempo()` | w | 480, 10.00, 250.00 bpm |
| `SeqGetPPQ(seq)` / `SeqSetPPQ(seq, ppq)` | w / d | |
| `SeqRescaleTime(d time, from ppq, to ppq)` | d | |
| `SeqGetTempoMapLen(seq)` | w | |
| `SeqGetTempoAtIx(seq, index, p d tick)` | d | returns the tempo, writes its tick |
| `SeqTempoIxByTick(seq, d tick)` | d | |
| `SeqGetTempo(seq, d tick)` | d | bpm × 100 |
| `SeqSetTempo(seq, d tick, tempo)` | w | changes the entry in effect at the tick |
| `SeqInsTempo(seq, d tick, tempo)` | w | |
| `SeqDelTempo(seq, d tick)` | w | deletes the entry in effect at the tick |
| `SeqClearTempoMap(seq, tempo)` | w | one entry |
| `SeqRecalcTempoMap(seq)` | w | |
| `SeqEnumTempos(seq, d from, d thru, p callback, d value)` | d | callback with 4 words; meaning not settled |
| `SeqSwapTempoMaps(seq, seq)` | w | |
| `SeqTempoCopy(dst, src, d from, d thru, d 0, p ?)` | w | from the traces |
| `SeqTempoPaste(dst, src, d at, d ?, 1, slide)` | w | from the traces |
| `SeqGetTempoRatio(seq, i)`, `SeqSetTempoRatio(seq, i, ratio)` | w | ratio in 64ths |
| `SeqSelectTempoRatio(seq, i)`, `SeqWhichTempoRatio(seq)` | w | |
| `SeqTempoRatio2Str(ratio, p buffer)` | w | "1.25" for 0x50 |

## Meter and positions

| Call | Result | Notes |
|---|---|---|
| `SeqGetMeterMapLen(seq)` | w | |
| `SeqGetMeterAtIx(seq, index, p d measure)` | d | the meter (0x0204 = 4/4) |
| `SeqMeterIxByTick(seq, d tick)` | d | |
| `SeqGetMeter(seq, d tick)` | d | |
| `SeqInsMeter(seq, measure, numerator, denominator power)` | w | |
| `SeqDelMeter(seq, n)` | w | |
| `SeqSetMeter(seq, w, w, w, w)` | w | ? |
| `SeqClearMeterMap(seq, numerator, denominator power)` | w | |
| `SeqRecalcMeterMap(seq)` | d | |
| `SeqEnumMeters(seq, from, thru, p callback, d value)` | d | callback with 4 words, then (0, 0, d value) |
| `SeqSwapMeterMaps(seq, seq)` | w | |
| `SeqMeterCopy(dst, src, d from, d thru, p ?)` | w | from the traces |
| `SeqMeterPaste(dst, src, d at, 1, 0)` | w | from the traces |
| `SeqMBT2Tick(seq, measure, beat, tick)` | d | |
| `SeqTick2MBT(seq, d tick, p measure, p beat, p tick)` | d | |
| `SeqTick2Meas(seq, d tick)` | d | |
| `SeqTick2MBTStr(seq, d tick, p buffer, flag)` | w | "2:1:033" |
| `SeqMBTStr2MBT(seq, p string, p measure, p beat, p tick)` | d | |
| `SeqTickNext/PrevMeas/Beat(seq, d tick)` | d | |

## Timecode

| Call | Result | Notes |
|---|---|---|
| `SeqGetFps(seq)` / `SeqSetFps(seq, fps)` | w | |
| `SeqGetFrameOfs(seq)` / `SeqSetFrameOfs(seq, d frames)` | d / w | |
| `SeqTick2Frame(seq, d tick)` | d | |
| `SeqFrame2TickRndOpt(seq, d frame, option)` | d | |
| `SeqFrame2HMSF(seq, d frame, p h, p m, p s, p f)` | d | |
| `SeqHMSF2Frame(seq, h, m, s, f, ?)` | d | |
| `SeqTick2HMSFStr(seq, d tick, p buffer)` | w | "00:00:02:00" |
| `SeqHMSFStr2HMSF(p string, p h, p m, p s, p f)` | d | |
| `SeqHMSFStr2MBT(seq, p string, p measure, p beat, p tick)` | d | |

## Markers

| Call | Result | Notes |
|---|---|---|
| `SeqNumMarkers(seq)` | d | |
| `SeqGetMarkerName(seq, d index, p buffer, size)` | w | the name's length |
| `SeqSetMarkerName(seq, d index, p name)` | w | |
| `SeqGetMarkerTime(seq, d index, p d time, p flag)` | w | the flag pointer may be null |
| `SeqSetMarkerTime(seq, d index, d time, flag)` | w | |
| `SeqInsMarker(seq, p name, d time, flag)` | w | kept in time order |
| `SeqDelMarker(seq, d index)` | w | |
| `SeqSortMarkers(seq, d ?)` | w | |
| `SeqMarkerForTime(seq, d tick)` | d | |

## Sysex banks

| Call | Result | Notes |
|---|---|---|
| `SeqCreateSysxBank(seq)` | w | a new bank handle |
| `SeqDestroySysxBank(bank)`, `SeqEmptySysxBank(bank)` | w | |
| `SeqEnumSysxBanks(seq, p callback, d value)` | d | like `SeqEnumStreams` |
| `SeqGet/SetSysxBankParam(bank, param[, d value])` | d / w | |
| `SeqGet/SetSysxBankName(bank, p buffer, size)` / `(bank, p name)` | w | |
| `SeqGetSysxBankLen(bank)` | d | |
| `SeqGetSysxAscLen(bank, bytes per line)` | d | 0 divides by zero |
| `SeqSysxBin2Asc(bank, p buffer, bytes per line)` | d | "F0 7E ... " with CR LF |
| `SeqSaveSysxBank(bank, p file)`, `SeqLoadSysxBank(bank, p file)` | w | |
| `SeqSendSysxBank(bank, 0, 0)` | w | sends the bank |
| `SeqSysxAsc2Bin`, `SeqReceiveSysxBank` | | layouts not found |

## Files, transport, input, devices

| Call | Result | Notes |
|---|---|---|
| `SeqLoadCakewalkFile` / `SeqSaveCakewalkFile` | w | see API.md |
| `SeqLoadMidiFile(p file, 0)` | w | the new sequence |
| `SeqSaveMidiFile(seq, p file, d 0)` | w | |
| `SeqGetFileError()` | w | |
| `SeqSetPlayingSeq(seq)`, `SeqSetStandbySeq(seq)` | w | |
| `SeqSetNow(seq, d tick)`, `SeqSetDur(seq, d ticks)` | d | return the value |
| `SeqGetNow`, `SeqGetEnd`, `SeqGetDur`, `SeqRewind` (seq) | d | |
| `SeqExpired()` | w | |
| `SeqGetSrcClock([seq])`, `SeqSetSrcClock([seq,] source)` | w | the sequence from 2.10 on |
| `SeqRecordStart(5 words, p callback)`, `SeqRecordStop()`, `SeqRecCount()`, `SeqRecordCleanup(seq, keep, 0, p stream)` | w / d | 1.01, 2.10; see API.md |
| `SeqStepRecSetup(seq, stream)`, `SeqStepRecParams(w, w, w, w)`, `SeqRecordEvent(p)`, `SeqSetRecCount(d)`, `SeqStepRecTerm()` | | ? step record takes its events from MIDI input |
| `SeqTxMidiMsg` (5 words) | | layout not found |
| `SeqGetPortName(port, p buffer)` | d | "1: Fake - MIDI Out" |
| `SeqHugeMemCpy(p dst, p src, d count)` | d | |
| `SeqIOSeek/Read/Write` | | only valid inside the load and save callbacks |
| `SeqConfigMidi(hwnd)` | | a dialog; not in the oracles |
