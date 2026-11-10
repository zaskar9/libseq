/* libseq: the TTSSEQ.DLL 1.01 interface in C.
 *
 * One function per export of TTSSEQ.DLL 1.01, with the DLL's names, argument order and meanings
 * (docs/reverse-engineering/signatures.md and ttsseq-api.md). The DLL's words are uint16_t, its
 * double words uint32_t (int32_t or int16_t where the value is signed), its far pointers plain
 * pointers. The DLL exports the names in capitals (Pascal calling convention); this header uses the
 * mixed case of the documentation. The functions come in the order of the export table, with the
 * ordinal in the comment. DESIGN.md maps every export, also those that have no function here.
 *
 * There is one engine per process, as there was one TTSSEQ.DLL per Windows session. A host
 * installs its MIDI ports and clock with LibSeqSetPlatform before SeqOpen. Functions whose names
 * begin with LibSeq are libseq's own: they do what Windows did for the DLL.
 *
 * A handle the engine does not hold makes TTSSEQ.DLL crash or misbehave; libseq answers 0 (or does
 * nothing). Callbacks are called from inside the call that takes them, except event procedures,
 * which stay registered, and the step recording's message (LIBSEQPLATFORM.postMessage).
 */
#ifndef LIBSEQ_TTSSEQ_H
#define LIBSEQ_TTSSEQ_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NOLINTBEGIN(modernize-use-using, modernize-avoid-c-arrays, readability-identifier-naming) */

/* ---- types */

/** A sequence: a small number, the lowest free one (1 for the first). */
typedef uint16_t HSEQ;
/** A stream: the sequence in the high byte, the stream's number in the low byte (0x0100). */
typedef uint16_t HSTREAM;
/** A sysex bank: numbered like a stream. */
typedef uint16_t HSYSXBANK;

/** An event: the 8-byte record of a .WRK stream, in its byte order (little-endian). A 24-bit time,
 *  a status byte (a MIDI status with its channel, 0xF0 for a sysex bank event, or a string event's
 *  kind), two data bytes and a 16-bit duration (notes; other kinds keep whatever the field holds). */
typedef struct SEQEVENT {
    uint8_t time[3];
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
    uint16_t duration;
} SEQEVENT;

/** SeqEnumStreams, SeqEnumSysxBanks: per stream or bank (seq, handle, value), then (0, 0, value).
 *  libseq does not use the result. */
typedef uint16_t (*SEQENUMPROC)(HSEQ seq, uint16_t handle, uint32_t value);
/** SeqEnumTempos: per entry its elapsed time (microseconds times the ppq, 32 bits of it) and the
 *  value, then (0, 0). libseq does not use the result. */
typedef uint16_t (*SEQENUMTEMPOPROC)(uint32_t elapsed, uint32_t value);
/** SeqEnumMeters: per entry its key (the signed byte, not sign-extended: 0x00FD for 3 flats), its
 *  meter and the value, then (0, 0, 0). libseq does not use the result. */
typedef uint16_t (*SEQENUMMETERPROC)(uint16_t key, uint16_t meter, uint32_t value);
/** An event procedure (SeqRegisterEventKind): (message, event, length). Messages: 1 the event
 *  plays (a copy, length 0), 3 it is freed (length 0), 4 load its data from the .WRK file (length
 *  7), 5 save it there (length 7), 7 the MIDI file (length 0: write it? length 7: write it). A
 *  load fails when it returns 0. The procedure reads and writes the file with SeqIORead and
 *  SeqIOWrite. */
typedef uint16_t (*SEQEVENTPROC)(uint16_t message, SEQEVENT* event, uint16_t length);
/** A start that waits (SeqPlayStart, SeqRecordStart): (code, a, b). 1 counting in (a: the beats
 *  left), 2 waiting for MIDI sync, 3 waiting for time code, 4 locking to it, 5 still waiting. It
 *  returns 0 to go on; anything else gives up. */
typedef uint16_t (*SEQSTARTPROC)(uint16_t code, uint16_t a, uint16_t b);
/** A sysex receive (SeqReceiveSysxBank): the bytes received so far and the free memory. It returns
 *  0 to go on receiving. */
typedef uint16_t (*SEQSYSXRECEIVEPROC)(uint32_t received, uint32_t freeMemory);
/** A load or save (SeqLoadCakewalkFile, SeqSaveCakewalkFile): (code, word, value). Load: 0xFE with
 *  the file when it is open, each chunk's type with the file before the engine reads it (return 1
 *  for a chunk the program read itself), 3 when the VARS chunk is done, 0xFF with the file at the
 *  end. Save: 0xFE with 0 first and with a percentage after each stream and bank, 3 after VARS (0 at
 *  either of the first two cancels the save), 0xFF with the file before the end marker. */
typedef uint16_t (*SEQFILEPROC)(uint16_t code, uint16_t word, uint32_t value);

/* ---- the exports of TTSSEQ.DLL 1.01, by ordinal */

/* 1 WEP: the Windows exit procedure; not here (DESIGN.md). */

/** 2: 0x0101. */
uint16_t SeqGetVersion(void);
/** 3: reads the options (TTSSEQ.INI) and sends Local Control Off on every channel of every port
 *  (SendLocalOff). The programs pass 0, 0. Returns 1. */
uint16_t SeqOpen(uint16_t reserved1, uint16_t reserved2);
/** 4: destroys every sequence and stops the timer. Returns 1. */
uint16_t SeqClose(void);
/** 5: 1 once when the playback has ended by itself; its stop output goes out then. */
uint16_t SeqIdle(void);
/** 6: a new, empty sequence with the lowest free handle. The programs pass 0, 0. */
HSEQ SeqCreateSeq(uint16_t reserved1, uint16_t reserved2);
/** 7: 1, also for a handle that holds no sequence. */
uint16_t SeqDestroySeq(HSEQ seq);
/** 8: 1, or 0 for a parameter 1.01 does not have. */
uint16_t SeqSetSeqParam(HSEQ seq, uint16_t param, int32_t value);
/** 9. */
int32_t SeqGetSeqParam(HSEQ seq, uint16_t param);
/** 10: a new stream in the sequence. The last two words have no effect found. */
HSTREAM SeqCreateStream(HSEQ seq, uint16_t reserved1, uint16_t reserved2);
/** 11: the stream goes, after its string events are freed. */
uint16_t SeqDestroyStream(HSTREAM stream);
/** 12: calls proc(seq, stream, value) per stream, then (0, 0, value). Returns 1. */
uint32_t SeqEnumStreams(HSEQ seq, SEQENUMPROC proc, uint32_t value);
/** 13: a kind of string event: the lowest free number from 1, 0 when all 127 are taken. A name
 *  may be registered twice. TTSSEQ.DLL keeps the name's pointer; libseq keeps a copy. Flags: 1 no
 *  message when the events play, 2 none when they are freed. */
uint16_t SeqRegisterEventKind(const char* name, SEQEVENTPROC proc, uint32_t flags);
/** 14: removes the first kind of that name. */
uint16_t SeqUnregisterEventKind(const char* name);
/** 15: calls the procedure of the event's kind (its status) with the same arguments and returns its
 *  result; 0 when no kind of that number is registered. */
uint16_t SeqCallEventProc(uint16_t message, SEQEVENT* event, uint16_t length);
/** 16: the number of events. */
uint32_t SeqStreamLen(HSTREAM stream);
/** 17: the time of the last event (a note's duration does not count). */
uint32_t SeqStreamDur(HSTREAM stream);
/** 18, 19: the index of the first event at or after (GE), or after (GT), the tick. */
uint32_t SeqEvtAtTimeGE(HSTREAM stream, uint32_t tick);
uint32_t SeqEvtAtTimeGT(HSTREAM stream, uint32_t tick);
/** 20: 1 when the stream has events in the measure (from 1). */
uint32_t SeqMeasHasEvts(HSTREAM stream, uint16_t measure);
/* 21 ___EXPORTEDSTUB: not here (DESIGN.md). */
/** 22: the cursor. */
uint32_t SeqTell(HSTREAM stream);
/** 23: 1 when the cursor is past the last event. */
uint16_t SeqEos(HSTREAM stream);
/** 24: moves the cursor (at most to the end); returns it. The programs pass 0 as the second word. */
uint32_t SeqSeek(HSTREAM stream, uint16_t reserved, uint32_t index);
/** 25: moves the cursor on; 1 while it is at an event. */
uint32_t SeqNxtEvt(HSTREAM stream);
/** 26, 27: the event at the cursor; 0 at the end. */
uint16_t SeqGetEvt(HSTREAM stream, SEQEVENT* event);
uint16_t SeqGetEvtCopy(HSTREAM stream, SEQEVENT* event);
/** 28: replaces the event at the cursor. */
uint16_t SeqPutEvt(HSTREAM stream, const SEQEVENT* event);
/** 29: inserts the event at the cursor. */
uint16_t SeqInsEvt(HSTREAM stream, const SEQEVENT* event);
/** 30: deletes the event at the cursor. */
uint16_t SeqDelEvt(HSTREAM stream);
/** 31: gives the event at the cursor the tick and moves it after the last event at or before it;
 *  returns its new index. */
uint32_t SeqMoveEvt(HSTREAM stream, uint32_t tick);
/** 32: a copy of the stream in the sequence, with its events and name, and with its parameters
 *  when copyParams is not 0 (a new stream's parameters otherwise). */
HSTREAM SeqCloneStream(HSEQ seq, HSTREAM stream, uint16_t copyParams);
/** 33: moves the stream into the sequence; returns its new handle. */
HSTREAM SeqAdoptStream(HSEQ seq, HSTREAM stream);
/** 34: a new stream in seq with the events of a, and those of b moved later by offset; at equal
 *  times a comes first. a goes, b too when destroyB is not 0; a = 0 means no stream. seqB has no
 *  effect found (the programs pass b's sequence). */
HSTREAM SeqMergeStreams(HSEQ seq, HSTREAM a, HSEQ seqB, HSTREAM b, uint16_t destroyB, uint32_t offset);
/** 35: inserts copies of all events of src before index, keeping their times, without sorting.
 *  Returns 1. */
uint32_t SeqInsertRange(HSTREAM dst, uint32_t index, HSTREAM src);
/** 36: deletes the events from first up to end (not included). 1; the first index when the range
 *  is empty, the length when nothing changes (ttsseq-api.md). */
uint32_t SeqDeleteRange(HSTREAM stream, uint32_t first, uint32_t end);
/** 37, 38: moves the events from the tick on earlier or later; the length (ttsseq-api.md). */
uint32_t SeqDisplaceEarlier(HSTREAM stream, uint32_t from, uint32_t amount);
uint32_t SeqDisplaceLater(HSTREAM stream, uint32_t from, uint32_t amount);
/** 39: sorts by time (Shell's sort, which is not stable); the length. */
uint32_t SeqTemporalSort(HSTREAM stream);
/** 40: a patch, volume or pan also goes out at once, on the stream's channel and port. */
uint16_t SeqSetStreamParam(HSTREAM stream, uint16_t param, int32_t value);
/** 41. */
int32_t SeqGetStreamParam(HSTREAM stream, uint16_t param);
/** 42: writes the 22 bytes of the parameters; returns 22. */
uint32_t SeqGetStreamAllParams(HSTREAM stream, uint8_t* buffer);
/** 43. */
uint16_t SeqSetStreamName(HSTREAM stream, const char* name);
/** 44: as much of the name as fits in size bytes with its 0; returns 1. */
uint16_t SeqGetStreamName(HSTREAM stream, char* buffer, uint16_t size);
/** 45: 480. */
uint16_t SeqGetMaxPPQ(void);
/** 46. */
uint32_t SeqSetPPQ(HSEQ seq, uint16_t ppq);
/** 47: the time in the other resolution. */
uint32_t SeqRescaleTime(uint32_t time, uint16_t fromPpq, uint16_t toPpq);
/** 48. */
uint16_t SeqGetPPQ(HSEQ seq);
/** 49, 50: 1000 and 25000 (10.00 and 250.00 bpm). Tempos are bpm times 100. */
uint16_t SeqGetMinTempo(void);
uint16_t SeqGetMaxTempo(void);
/** 51: one entry. */
uint16_t SeqClearTempoMap(HSEQ seq, uint16_t tempo);
/** 52: the number of entries. */
uint16_t SeqRecalcTempoMap(HSEQ seq);
/** 53. */
uint16_t SeqGetTempoMapLen(HSEQ seq);
/** 54: the entry in effect at the tick. */
uint32_t SeqTempoIxByTick(HSEQ seq, uint32_t tick);
/** 55: the entry's tempo (0xFFFF past the end); its tick goes to *tick. */
uint32_t SeqGetTempoAtIx(HSEQ seq, uint16_t index, uint32_t* tick);
/** 56: the tempo in effect at the tick. */
uint16_t SeqGetTempo(HSEQ seq, uint32_t tick);
/** 57: changes the entry in effect at the tick. */
uint16_t SeqSetTempo(HSEQ seq, uint32_t tick, uint16_t tempo);
/** 58. */
uint16_t SeqInsTempo(HSEQ seq, uint32_t tick, uint16_t tempo);
/** 59: deletes the entry in effect at the tick. */
uint16_t SeqDelTempo(HSEQ seq, uint32_t tick);
/** 60: calls proc per entry from the first on, then (0, 0) (ttsseq-api.md). Returns 1. */
uint32_t SeqEnumTempos(HSEQ seq, uint32_t from, uint32_t thru, SEQENUMTEMPOPROC proc, uint32_t value);
/** 61: copies the entries from..thru into clip. Sets *flag to 1 when src has an entry in the range;
 *  reserved has no effect found. Returns 1. */
uint16_t SeqTempoCopy(HSEQ src, HSEQ clip, uint32_t from, uint32_t thru, uint32_t reserved, uint16_t* flag);
/** 62: pastes clip's entries at the tick, reps times length apart. Returns 1. */
uint16_t SeqTempoPaste(HSEQ clip, HSEQ dst, uint32_t at, uint32_t length, uint16_t reps, uint16_t slide);
/** 63: returns 0. */
uint16_t SeqSwapTempoMaps(HSEQ a, HSEQ b);
/** 64, 65: 24, 25, 29 (30 drop frame) or 30. */
uint16_t SeqSetFps(HSEQ seq, uint16_t fps);
uint16_t SeqGetFps(HSEQ seq);
/** 66, 67: the SMPTE offset in frames. */
uint16_t SeqSetFrameOfs(HSEQ seq, uint32_t frames);
uint32_t SeqGetFrameOfs(HSEQ seq);
/** 68. */
uint32_t SeqTick2Frame(HSEQ seq, uint32_t tick);
/** 69: option 0 rounds to the nearest tick, 1 rounds up. */
uint32_t SeqFrame2TickRndOpt(HSEQ seq, uint32_t frame, uint16_t option);
/** 70: returns the hours. */
uint32_t SeqFrame2HMSF(HSEQ seq, uint32_t frame, uint16_t* hours, uint16_t* minutes, uint16_t* seconds,
                       uint16_t* frames);
/** 71: the last word has no effect found. */
uint32_t SeqHMSF2Frame(HSEQ seq, uint16_t hours, uint16_t minutes, uint16_t seconds, uint16_t frames,
                       uint16_t reserved);
/** 72: "00:00:02:00"; returns the buffer. */
char* SeqTick2HMSFStr(HSEQ seq, uint32_t tick, char* buffer);
/** 73: returns 1. */
uint32_t SeqHMSFStr2HMSF(const char* string, uint16_t* hours, uint16_t* minutes, uint16_t* seconds, uint16_t* frames);
/** 74: returns 1. */
uint32_t SeqHMSFStr2MBT(HSEQ seq, const char* string, uint16_t* measure, uint16_t* beat, uint16_t* tick);
/** 75 to 78: three tempo ratios in 64ths (0x20, 0x40, 0x80 at the start), one of them selected. */
uint16_t SeqGetTempoRatio(HSEQ seq, uint16_t index);
uint16_t SeqSetTempoRatio(HSEQ seq, uint16_t index, uint16_t ratio);
uint16_t SeqSelectTempoRatio(HSEQ seq, uint16_t index);
uint16_t SeqWhichTempoRatio(HSEQ seq);
/** 79: "1.25" for 0x50; returns the buffer. */
char* SeqTempoRatio2Str(uint16_t ratio, char* buffer);
/** 80: one entry at measure 1. A key is the number of sharps, negative for flats; a meter is the
 *  power of two of the denominator in the high byte and the numerator in the low byte (0x0204 is
 *  4/4). */
uint16_t SeqClearMeterMap(HSEQ seq, int16_t key, uint16_t meter);
/** 81: the number of entries. */
uint32_t SeqRecalcMeterMap(HSEQ seq);
/** 82. */
uint16_t SeqGetMeterMapLen(HSEQ seq);
/** 83: the entry in effect at the tick. */
uint16_t SeqMeterIxByTick(HSEQ seq, uint32_t tick);
/** 84: key << 16 | meter (the key a byte); the entry's measure (from 1) goes to *measure.
 *  0xFFFFFFFF past the end. */
uint32_t SeqGetMeterAtIx(HSEQ seq, uint16_t index, uint16_t* measure);
/** 85: key << 16 | meter in effect at the tick. */
uint32_t SeqGetMeter(HSEQ seq, uint32_t tick);
/** 86: changes the entry in effect at the tick; positions are recalculated. */
uint16_t SeqSetMeter(HSEQ seq, uint32_t tick, int16_t key, uint16_t meter);
/** 87: kept in measure order; an entry at the same measure is replaced. */
uint16_t SeqInsMeter(HSEQ seq, uint16_t measure, int16_t key, uint16_t meter);
/** 88: deletes the entry in effect at the measure, also the first one. */
uint16_t SeqDelMeter(HSEQ seq, uint16_t measure);
/** 89: calls proc per entry from the first on, then (0, 0, 0); measures from 1. Returns 1. */
uint32_t SeqEnumMeters(HSEQ seq, uint16_t from, uint16_t thru, SEQENUMMETERPROC proc, uint32_t value);
/** 90: copies the measures from..thru (from 1) into clip, and with cut takes them out of src. Sets
 *  *flag to 1 when src has an entry in the range; reserved has no effect found. 0 unless
 *  1 <= from <= thru. */
uint16_t SeqMeterCopy(HSEQ src, HSEQ clip, uint16_t from, uint16_t thru, uint16_t cut, uint16_t reserved,
                      uint16_t* flag);
/** 91: pastes clip's entries at the measure, reps times length apart. Returns 1. */
uint16_t SeqMeterPaste(HSEQ clip, HSEQ dst, uint16_t measure, uint16_t length, uint16_t reps, uint16_t slide);
/** 92: returns 0. */
uint16_t SeqSwapMeterMaps(HSEQ a, HSEQ b);
/** 93 to 101: positions as measure, beat and tick, from 1, 1, 0. */
uint32_t SeqMBT2Tick(HSEQ seq, uint16_t measure, uint16_t beat, uint16_t tick);
/** 94: returns the tick of the position. */
uint32_t SeqTick2MBT(HSEQ seq, uint32_t tick, uint16_t* measure, uint16_t* beat, uint16_t* ticks);
uint32_t SeqTick2Meas(HSEQ seq, uint32_t tick);
/** 96: "2:1:033"; returns the buffer. flag: ttsseq-api.md. */
char* SeqTick2MBTStr(HSEQ seq, uint32_t tick, char* buffer, uint16_t flag);
/** 97: returns 1. */
uint32_t SeqMBTStr2MBT(HSEQ seq, const char* string, uint16_t* measure, uint16_t* beat, uint16_t* tick);
uint32_t SeqTickNextMeas(HSEQ seq, uint32_t tick);
uint32_t SeqTickPrevMeas(HSEQ seq, uint32_t tick);
uint32_t SeqTickNextBeat(HSEQ seq, uint32_t tick);
uint32_t SeqTickPrevBeat(HSEQ seq, uint32_t tick);
/** 102: the sequence the transport plays (0 for none); also the one whose thru settings the input
 *  follows. */
uint16_t SeqSetPlayingSeq(HSEQ seq);
/** 103: kept; no effect found. */
uint16_t SeqSetStandbySeq(HSEQ seq);
/** 104: 0 the internal clock, 1 MIDI sync, 3 MIDI Time Code; another value returns 0 and changes
 *  nothing. */
uint16_t SeqSetSrcClock(uint16_t source);
/** 105. */
uint16_t SeqGetSrcClock(void);
/** 106: plays the playing sequence from its Now. 0 when it plays already, or when proc gives up a
 *  wait for sync. proc may be NULL. */
uint16_t SeqPlayStart(SEQSTARTPROC proc);
/** 107: 0 when it does not play. */
uint16_t SeqPlayStop(void);
/** 108: records into a new take of the playing sequence from its Now, after the count-in, during
 *  which proc gets (1, beats left, 0). A target stream punches from in up to out; 0 for none. */
uint16_t SeqRecordStart(HSTREAM target, uint32_t in, uint32_t out, SEQSTARTPROC proc);
/** 109: 0 when nothing was recorded. */
uint16_t SeqRecordStop(void);
/** 110: ends the take. *stream is read and written: kept with 0 there, the take becomes a new
 *  stream, whose handle goes there; kept with a stream there, the two are merged into a new one.
 *  reserved has no effect found (the programs pass 0). See ttsseq-api.md. */
uint16_t SeqRecordCleanup(HSEQ seq, uint16_t keep, uint16_t reserved, HSTREAM* stream);
/** 111: 1 while stopped, 0 while playing. */
uint16_t SeqExpired(void);
/** 112: Now to 0. */
uint32_t SeqRewind(HSEQ seq);
/** 113: moves Now; while playing, the playback goes on from there. Returns the tick. */
uint32_t SeqSetNow(HSEQ seq, uint32_t tick);
uint32_t SeqGetNow(HSEQ seq);
uint32_t SeqGetEnd(HSEQ seq);
/** 116: returns the value. */
uint32_t SeqSetDur(HSEQ seq, uint32_t ticks);
uint32_t SeqGetDur(HSEQ seq);
/** 118: starts a step recording into a new stream of the sequence and returns it. After each step
 *  the engine posts message 0x464 (wParam 0x100, lParam the number of events) to hwnd, through
 *  LIBSEQPLATFORM.postMessage. */
HSTREAM SeqStepRecSetup(HSEQ seq, void* hwnd);
/** 119: where the next step goes, its notes' duration and a fixed velocity (0: as played). */
void SeqStepRecParams(uint32_t position, uint16_t duration, uint16_t velocity);
/** 120: adds the event to a running take at Now (its time is ignored); not during step recording. */
uint16_t SeqRecordEvent(const SEQEVENT* event);
/** 121: the events of the take so far. */
uint32_t SeqRecCount(void);
/** 122: keeps the first count events of the take. */
uint16_t SeqSetRecCount(uint32_t count);
/** 123: ends step recording; the number of events. */
uint16_t SeqStepRecTerm(void);
/** 124: sends one channel message at once; the channel replaces the status's low nibble. Port
 *  0xFFFF is every port, channel 0xFF every channel. Note-offs go out only for notes that are on. 1,
 *  or 0 for pitch wheel, system messages, channel 16 and above or data above 0x7F, which are not
 *  sent. */
uint16_t SeqTxMidiMsg(uint16_t port, uint16_t channel, uint16_t status, uint16_t data1, uint16_t data2);
/** 125. */
HSYSXBANK SeqCreateSysxBank(HSEQ seq);
uint16_t SeqDestroySysxBank(HSYSXBANK bank);
uint16_t SeqEmptySysxBank(HSYSXBANK bank);
/** 128: like SeqEnumStreams. */
uint32_t SeqEnumSysxBanks(HSEQ seq, SEQENUMPROC proc, uint32_t value);
uint16_t SeqSetSysxBankParam(HSYSXBANK bank, uint16_t param, int32_t value);
int32_t SeqGetSysxBankParam(HSYSXBANK bank, uint16_t param);
uint16_t SeqSetSysxBankName(HSYSXBANK bank, const char* name);
/** 132: as much of the name as fits; 1 when all of a name did. */
uint16_t SeqGetSysxBankName(HSYSXBANK bank, char* buffer, uint16_t size);
/** 133: the bytes. */
uint32_t SeqGetSysxBankLen(HSYSXBANK bank);
/** 134: the size of the text with its 0 (TTSSEQ.DLL divides by zero for 0 bytes per line). */
uint32_t SeqGetSysxAscLen(HSYSXBANK bank, uint16_t bytesPerLine);
/** 135: "F0 7E ... " lines with CR LF; the bytes. An empty bank leaves the buffer as it is. */
uint32_t SeqSysxBin2Asc(HSYSXBANK bank, char* buffer, uint16_t bytesPerLine);
/** 136: text to bytes. 0, or 1 (no F0 at the start), 2 (no F7 at the end), 4 (not hex; the offset of
 *  the character goes to *position). On an error the bank keeps its bytes. */
uint16_t SeqSysxAsc2Bin(HSYSXBANK bank, const char* text, uint32_t* position);
/** 137, 138: the bank's bytes as a file. */
uint16_t SeqSaveSysxBank(HSYSXBANK bank, const char* file);
uint16_t SeqLoadSysxBank(HSYSXBANK bank, const char* file);
/** 139: sends the bank in packets of SysxSendPacketSize bytes, 60 ms apart. The programs pass 0, 0. */
uint16_t SeqSendSysxBank(HSYSXBANK bank, uint16_t reserved1, uint16_t reserved2);
/** 140: adds incoming sysex to the bank until proc returns non-zero. Universal messages (F0 7E,
 *  F0 7F) are left out. 1, or -2 when the bank stayed empty, -4 when the input buffers could not
 *  be prepared. The last three words have no effect found. */
int16_t SeqReceiveSysxBank(HSYSXBANK bank, SEQSYSXRECEIVEPROC proc, uint16_t reserved1, uint16_t reserved2,
                           uint16_t reserved3);
/** 141: a new sequence from a .WRK file; 0 when it fails (SeqGetFileError says why). proc may be
 *  NULL; it gets value as its last argument. */
HSEQ SeqLoadCakewalkFile(const char* file, uint16_t reserved, SEQFILEPROC proc, uint32_t value);
/** 142: the sequence as 1.01 writes it. */
uint16_t SeqSaveCakewalkFile(HSEQ seq, const char* file, uint32_t reserved, SEQFILEPROC proc, uint32_t value);
/** 143: a new sequence from a Standard MIDI File; 0 when it fails. */
HSEQ SeqLoadMidiFile(const char* file, uint16_t reserved);
/** 144: a format 1 Standard MIDI File. */
uint16_t SeqSaveMidiFile(HSEQ seq, const char* file, uint32_t reserved);
/** 145 to 147: the file the engine has open, in the load and save callbacks and the event
 *  procedures. SeqIOSeek uses the handle it gets; SeqIOWrite and SeqIORead do not. Seek: origin 0
 *  the start, 1 here, 2 the end; returns the position. Read and write return the bytes moved. */
uint32_t SeqIOSeek(uint16_t file, int32_t offset, uint16_t origin);
uint16_t SeqIOWrite(uint16_t file, const void* buffer, uint16_t count);
uint16_t SeqIORead(uint16_t file, void* buffer, uint16_t count);
/** 148: what the last file operation reported (7 for a missing file). */
uint16_t SeqGetFileError(void);
/* 149 SeqConfigMidi: the device dialog; not here (DESIGN.md). */
/** 150: "1: device"; past the ports the devices come round again, with that number. Returns 1. */
uint32_t SeqGetPortName(uint16_t port, char* buffer);
/** 151. */
uint32_t SeqNumMarkers(HSEQ seq);
/** 152: the last marker at or before the tick, 0 when none is. */
uint32_t SeqMarkerForTime(HSEQ seq, uint32_t tick);
uint16_t SeqSetMarkerName(HSEQ seq, uint32_t index, const char* name);
/** 154: as much of the name as fits; returns its length. */
uint16_t SeqGetMarkerName(HSEQ seq, uint32_t index, char* buffer, uint16_t size);
uint16_t SeqSetMarkerTime(HSEQ seq, uint32_t index, uint32_t time, uint16_t smpteLock);
/** 156: smpteLock may be NULL. */
uint16_t SeqGetMarkerTime(HSEQ seq, uint32_t index, uint32_t* time, uint16_t* smpteLock);
/** 157: kept in time order. */
uint16_t SeqInsMarker(HSEQ seq, const char* name, uint32_t time, uint16_t smpteLock);
uint16_t SeqDelMarker(HSEQ seq, uint32_t index);
/** 159: the number of markers. The double word has no effect found. */
uint16_t SeqSortMarkers(HSEQ seq, uint32_t reserved);
/** 160: copies count bytes (overlapping too); returns the count. */
uint32_t SeqHugeMemCpy(void* dst, const void* src, uint32_t count);
/* 161 TIMERTRAP, 162 RXTRAP, 163 DLGPROC: Windows callbacks; not here (DESIGN.md). */

/* ---- libseq: what Windows did for TTSSEQ.DLL */

/** TTSSEQ.INI's [Options], with 1.01's defaults (ttsseq-api.md). */
typedef struct LIBSEQOPTIONS {
    int syncOutputPort;              /* SyncOutputPort: the port (from 1) the Song Position goes to */
    int sendLocalOff;                /* SendLocalOff: 0 for no Local Control Off at SeqOpen */
    int timebase;                    /* Timebase */
    unsigned timerPeriod;            /* TimerPeriod, milliseconds */
    unsigned timerResolution;        /* TimerResolution, milliseconds */
    unsigned sysxSendPacketSize;     /* SysxSendPacketSize */
    unsigned sysxReceiveBuffers;     /* SysxReceiveBuffers */
    unsigned sysxReceiveBuffersSize; /* SysxReceiveBuffersSize */
    unsigned chaseDelay;             /* ChaseDelay, tenths of a second */
    unsigned flywheelAmount;         /* FlywheelAmount, tenths of a second */
} LIBSEQOPTIONS;

/** The MIDI ports and the clock (1.01 has them from Windows). Every member may be NULL but send and
 *  milliseconds; context is passed back to each. While the engine is open the host calls
 *  LibSeqTimer every timer period, and LibSeqReceive for every message that comes in. */
typedef struct LIBSEQPLATFORM {
    void* context;
    /** The number of output ports, from 0 (TTSSEQ.INI's [MIDI Output Devices]). */
    int (*outputPorts)(void* context);
    /** The name of the device behind a port; the string stays valid until the next call. */
    const char* (*outputName)(void* context, int port);
    /** TTSSEQ.INI's [Options]: options holds the defaults, the host changes what it has. */
    void (*options)(void* context, LIBSEQOPTIONS* options);
    /** A short message of 1 to 3 bytes, or a sysex message F0 ... F7, to send at once. */
    void (*send)(void* context, int port, const uint8_t* message, size_t length);
    /** A clock in milliseconds that only goes forward. */
    uint32_t (*milliseconds)(void* context);
    /** SeqOpen and SeqClose: the host starts and stops its input. */
    void (*opened)(void* context);
    void (*closed)(void* context);
    /** From now on call LibSeqTimer every periodMs milliseconds, the first time periodMs from now;
     *  stopTimer: no more. */
    void (*startTimer)(void* context, unsigned periodMs);
    void (*stopTimer)(void* context);
    /** The engine waits in a loop (count-in, sync, sysex): let a little time pass. */
    void (*wait)(void* context);
    /** PostMessage: SeqStepRecSetup's message to its window. */
    void (*postMessage)(void* context, void* hwnd, uint16_t message, uint16_t wParam, uint32_t lParam);
} LIBSEQPLATFORM;

/** Installs the platform (copied; NULL for none). */
void LibSeqSetPlatform(const LIBSEQPLATFORM* platform);
/** The multimedia timer's callback: the engine sends what is due. */
void LibSeqTimer(void);
/** The MIDI input callback: one message that came in. */
void LibSeqReceive(const uint8_t* message, size_t length);
/** As when Windows unloads the DLL: the engine and all it holds go; the next call starts a new
 *  one. The platform stays installed. */
void LibSeqUnload(void);

/* NOLINTEND(modernize-use-using, modernize-avoid-c-arrays, readability-identifier-naming) */

#ifdef __cplusplus
}
#endif

#endif /* LIBSEQ_TTSSEQ_H */
