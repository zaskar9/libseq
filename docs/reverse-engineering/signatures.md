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
| `SeqInsertRange(stream, w, w, w)` | d | no working layout: on a loaded stream it returns 0 and changes nothing, whatever the other words; on a new empty stream it crashes; 3.00 crashes. Not imported by any program |
| `SeqMergeStreams(seq, a, seq b, b, flag, d offset)`; 3.00: `(seq, a, seq b, b, flag, d count, d offset)` | w | a new stream in `seq`: the events of stream `a`, and those of `b` moved later by `offset` (3.00: the first `count` events of `b`); at equal times `a` comes first. `a` is destroyed, `b` too when `flag` is not 0. Returns the new stream. The third word has no visible effect (the programs pass `b`'s sequence) |
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

A meter entry has a key signature and a meter. `key` is the number of sharps, negative for flats
(0xFFFD = 3 flats); results carry it as a byte (0x00FD). `meter` is the power of two of the
denominator in the high byte and the numerator in the low byte: 0x0204 = 4/4, 0x0306 = 6/8. These
layouts come from Cakewalk Apprentice 1.04 editing its Meter/Key view [trace]; until 2026-10-30 the
suites passed numerator and denominator as two words, which the engine took as key and meter.

| Call | Result | Notes |
|---|---|---|
| `SeqGetMeterMapLen(seq)` | w | |
| `SeqGetMeterAtIx(seq, index, p measure)` | d | key << 16 \| meter; the entry's measure (a word) goes to the pointer |
| `SeqMeterIxByTick(seq, d tick)` | d | |
| `SeqGetMeter(seq, d tick)` | d | key << 16 \| meter in effect at the tick |
| `SeqInsMeter(seq, measure, key, meter)` | w | kept in measure order; an entry at the same measure is replaced |
| `SeqDelMeter(seq, measure)` | w | deletes the entry in effect at the measure, also the first one |
| `SeqSetMeter(seq, d tick, key, meter)` | w | changes the entry in effect at the tick; positions are recalculated |
| `SeqClearMeterMap(seq, key, meter)` | w | one entry at measure 1 |
| `SeqRecalcMeterMap(seq)` | d | the number of entries |
| `SeqEnumMeters(seq, from, thru, p callback, d value)` | d | callback(key, meter, d value) per entry, then (0, 0, d value) |
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
| `SeqSysxAsc2Bin(bank, p text, p position)` | w | text to bytes: 0, or 1 (no F0 at the start), 2 (no F7 at the end), 4 (not hex; the offset of the character goes to the dword at `position`); on an error the bank keeps its bytes |
| `SeqReceiveSysxBank(bank, p callback, 0, 0, 0)` | w | adds incoming sysex to the bank until the callback returns non-zero; callback(d bytes received, d free memory). Returns 1, or -2 when the bank stayed empty, -4 when the input buffer could not be prepared. Universal messages (F0 7E, F0 7F) are left out. The last three words have no visible effect |
| `SeqReceiveSysxBankEx(bank, p callback, 0, 0, 0, 0, 0)` | w | 3.00; with zeros, as Cakewalk Professional 2.00 calls it, the same as `SeqReceiveSysxBank` |

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
| `SeqTxMidiMsg(port, channel, status, data1, data2)` | w | sends one channel message at once: the channel replaces the status's low nibble; port 0xffff is every port, channel 0xff every channel. Note-offs (0x8n, or 0x9n with velocity 0) go out only for notes that are on; a note-on for a note that is on sends its note-off first. Returns 1, or 0 for pitch wheel (0xEn), system messages, channel 16 and above or data above 0x7f, which are not sent |
| `SeqGetPortName(port, p buffer)` | d | "1: Fake - MIDI Out" |
| `SeqHugeMemCpy(p dst, p src, d count)` | d | |
| `SeqIOSeek/Read/Write` | | only valid inside the load and save callbacks |
| `SeqConfigMidi(hwnd)` | | a dialog; not in the oracles |
