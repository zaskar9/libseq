# TTSSEQ API: what is known

Black-box catalogue of the Twelve Tone sequencer engine, versions 1.01, 2.10 and 3.00. Every
statement comes from one of five sources, named in brackets:

- [exp] the DLL's export table;
- [sweep] Wine's tracer measuring how many argument words a function removes from the stack when
  `ttsprobe` calls it (the size is the callee's own, whatever the caller pushed);
- [trace] what the demo, Cakewalk Apprentice 1.04 or Cakewalk Professional 2.00 pass and get back (`traces/`);
- [probe] calls made with `ttsprobe` and their results;
- [relay] Wine's relay trace of the calls the engine itself makes into Windows (MMSYSTEM, KERNEL)
  while the probe calls it.

`api-catalogue.md` lists every export with its ordinal and size per version. Sizes are in 16-bit
words; a far pointer or a 32-bit value is two words, written high word first (as the tracer
prints them). All functions are Pascal: the callee pops its arguments [sweep].

## How the versions differ

- Exports: 163 (1.01), 169 (2.10), 183 (3.00) [exp]; see `exports/exports-compared.txt`.
- Only seven functions change their size [sweep]:

  | Function | 1.01 | 2.10 | 3.00 |
  |---|---|---|---|
  | `SeqSetSrcClock` | 1 | 2 | 2 |
  | `SeqGetSrcClock` | 0 | 1 | 1 |
  | `SeqLoadCakewalkFile` | 7 | 7 | 3 |
  | `SeqSaveCakewalkFile` | 9 | 9 | 5 |
  | `SeqIORead`, `SeqIOWrite` | 4 | 4 | 5 |
  | `SeqMergeStreams` | 7 | 7 | 9 |

  2.10 gives the clock source a sequence argument. 3.00 drops the program callback from the file
  functions (programs keep their own data in "baggage" instead, below) and widens the I/O byte
  count to 32 bits.
- 3.00 renames `SeqPlayStart`/`SeqPlayStop` to `SeqStart`/`SeqStop` and `SeqStart` takes 4 words
  (a callback and a 32-bit value) where `SeqPlayStart` took 2 (a callback) [exp, sweep, trace].

## Handles

- A sequence is a small number: `SeqCreateSeq(0, 0)` returns 1 for the first one [probe, trace].
  `SeqLoadCakewalkFile` creates a sequence and returns its handle in AX [probe].
- A stream handle is `sequence << 8 | id`: 0x0200 is stream 0 of sequence 2 [trace, probe]. The id
  is an allocation number, not the track: `SeqCloneStream(2, 0x0200, 1)` returned 0x0207, and
  Cakewalk Apprentice 1.04 then set stream parameter 11 to 1 to make it track 2 [trace].

## Lifecycle

| Call | Observed |
|---|---|
| `SeqGetVersion()` | AX = 0x0101, 0x020A, 0x0300 [probe] |
| `SeqWire(1)` | 2.10 opens the MIDI port only when called before `SeqOpen` (Cakewalk Apprentice 1.04 does) [probe] |
| `SeqOpen(0, 0)` | opens the configured ports; returns 1 on 1.01 and 0 on 2.10/3.00 when it works [probe] |
| `SeqIdle()` | called from the program's message loop; returns 1 once when playback has reached the end, and the engine sends its stop output then [trace, probe] |
| `SeqClose()` | sends All Sound Off (CC 120) and sustain off (CC 64 = 0) on every channel [probe] |

On opening its port each engine sends Local Control Off (CC 122 = 0) on all 16 channels
[probe]. 2.10 and 3.00 skip a port whose manufacturer ID is 255; 1.01 does not [probe].

## Sequences and parameters

`SeqSetSeqParam(seq, param, value32)` and `SeqGetSeqParam(seq, param)` (4 and 2 words) [sweep].
After loading, the parameters hold the file's settings (VARS). The map below comes from loading
178 copies of one song, each with one VARS byte changed (once plus one, once cleared or set to
255), and reading parameters 0 to 63 after each load [probe]. The numbering is the same in all
three versions:

| Param | Holds | Stored in | 1.01 | 2.10 | 3.00 |
|---|---|---|---|---|---|
| 1 | patch/wheel/controller search-back | VARS 20 | yes | yes | yes |
| 2 | zero continuous controllers on stop | VARS 17 | yes | yes | yes |
| 3 | unknown: 1 after loading or creating; both programs set it to 1 after a take; no effect on playback | | yes | yes | yes |
| 4 | send MIDI Song Position Pointer | VARS 18 | yes | yes | yes |
| 5 | use Continue (0 = "Start, never Continue") | VARS 19 | yes | yes | yes |
| 6 | transmit MIDI sync (1 only when the byte is 255) | VARS 77 | no | yes | yes |
| 7 | MIDI thru mode: 0 none, 1 manual, 2 auto | chunk 16, byte 0 | yes | yes | yes |
| 8 to 11 | thru mapping: port, channel (-1 = as received), Key+, Vel+ | chunk 16 | yes | yes | yes |
| 12 | "Local On" port (-1 = none) | | yes | yes | yes |
| 13, 14 | metronome during playback, during recording | VARS 31, 32 | yes | yes | yes |
| 15, 16 | metronome accent, count-in measures | VARS 33, 34 | yes | yes | yes |
| 17 to 21 | metronome port, channel, key, velocity, duration | chunk 17 | yes | yes | yes |
| 22 | metronome on the PC speaker | | yes | yes | yes |
| 23, 24, 25 | auto-stop, auto-rewind, auto-restart | VARS 21, 26, 57 | yes | yes | yes |
| 26 | rewind time (32-bit) | VARS 27 to 30 | yes | yes | yes |
| 27 | stop time (32-bit) | VARS 22 to 25 | yes | yes | yes |
| 28 | record and echo filter (below) | VARS 38 | yes | yes | yes |
| 29 | SPP locate delay (16-bit, in 55 ms ticks) | VARS 15, 16 | no | yes | yes |
| 30, 31 | unknown (31 is a flag, 1 by default); no effect on playback, not set by any Professional dialog tried | chunk 28 | no | no | yes |
| 32 | record and echo channels, bit n = channel n + 1 (0xFFFF = all) | VARS 35, 36 | no | no | yes |

Sources: the VARS column from loading 178 copies of one song, each with one VARS byte changed; the
chunk columns from setting each parameter through the probe and saving (3.00); the meanings from
Cakewalk Professional 2.00's Settings dialogs, whose OK button calls `SeqSetSeqParam` [probe, trace].

Parameter 28 has one bit per channel message type, bit (status >> 4) - 8: 0x01 Note Off, 0x02
Note On (the dialog's "Note" sets both), 0x04 Key Aftertouch, 0x08 Controller, 0x10 Patch, 0x20
Channel Aftertouch, 0x40 Pitch Wheel. Files hold 0x7F; a new sequence starts with 0xDB (Key and
Channel Aftertouch off, and 0x80, which no dialog sets) [trace, probe].

The VARS bytes for Now, From and Thru (0 to 11) and the tempo ratios (58 to 61) have their own
functions.

Cakewalk Apprentice 1.04 sets parameters 23 to 27 to 0 after every load, which switches the auto-shuttle off;
Cakewalk Professional 2.00 leaves them as loaded [trace]. Played by the probe, which does not touch them,
E3_SHTL loops from 960 back to 480 in all three versions [probe]: the auto-shuttle is a feature
of the engine, 2.10 included.

## Streams

| Call | Words | Observed |
|---|---|---|
| `SeqStreamLen(stream)` | 1 | number of events (156 for the first stream of Cakewalk's sample song `CAKEWALK.WRK`) [probe] |
| `SeqStreamDur(stream)` | 1 | length in ticks [trace] |
| `SeqMeasHasEvts(stream, measure)` | 2 | 1 or 0 [trace] |
| `SeqGetStreamName(stream, buffer, size)` | 4 | Cakewalk Apprentice 1.04 passes size 0x81 [trace] |
| `SeqGetStreamParam(stream, param)` | 2 | see the table below [probe] |
| `SeqSetStreamParam(stream, param, value32)` | 4 | [trace] |
| `SeqGetStreamAllParams(stream, buffer)` | 3 | returns 0x16 [trace] |
| `SeqCloneStream(seq, stream, flags)` | 3 | returns the new stream [trace] |
| `SeqDestroyStream(stream)` | 1 | Track > Kill [trace] |
| `SeqEnumStreams(seq, callback, value32)` | 5 | calls the program's `StreamsProc` [trace] |

Stream parameters, from loading copies of one song with one track-record byte changed at a
time [probe]; the same in all three versions:

| Param | Holds | From the file |
|---|---|---|
| 0 | Key+ (signed) | chunk 1, byte 10 |
| 1 | Vel+ (signed) | chunk 1, byte 11 |
| 2 | Time+ (signed ticks) | TRKOFFS (9) and its 32-bit form NTRKOFS (27) |
| 3 | port | chunk 1, byte 12 |
| 4 | forced channel (-1 = none) | chunk 1, byte 9 |
| 5 | patch (-1 = none) | TRKPATCH (14) |
| 6 | volume (-1 = none) | TRKVOL (19) |
| 7 | muted | chunk 1, flags bit 1 |
| 8 | archived | chunk 1, flags bit 4 |
| 9 | loop count (9999 when the loop flag is set) | chunk 1, flags bit 2; TRKREPS (12) |
| 10 | pan (-1 = none) | TRKVOL (19) |
| 11 | track position | chunk 1, bytes 0 and 1 |
| 12 to 15 | used while recording (below); not saved | |

Track > Archive in Cakewalk Apprentice 1.04 calls `SeqSetStreamParam(stream, 8, 1)` and saves flag bit 4
(0x10) [trace]. An archived track does not play, like a muted one, in all three versions [probe].

## Events

- The event record is 8 bytes, the same in all three versions [probe]: 24-bit time, status byte,
  two data bytes, 16-bit duration. It is the event record of the `.WRK` file. Example, the first
  note of the sample song `CAKEWALK.WRK`: `00 00 00 91 41 72 33 00` = tick 0, note on channel 2, key 65 (F5), velocity 114,
  duration 51.
- The cursor is per stream: `SeqSeek(stream, 0, index32)` (4 words) positions by event index,
  `SeqTell(stream)` returns it, `SeqNxtEvt(stream)` advances and returns 1 while there are more,
  `SeqGetEvt(stream, buffer)` (3 words) copies the current event out, `SeqPutEvt` writes it back
  (Edit > Transpose does get/put per event), `SeqEvtAtTimeGE(stream, tick32)` (3 words) returns an
  index [probe, trace].

## Time, tempo, meter

| Call | Words | Observed |
|---|---|---|
| `SeqGetPPQ(seq)` | 1 | 120 [probe] |
| `SeqGetTempo(seq, tick32)` | 3 | bpm × 100 (default 10000) [probe] |
| `SeqGetMeter(seq, tick32)` | 3 | the key signature in DX, the meter in AX: 0x0204 = 4/4, the numerator in the low byte, the power of two of the denominator in the high byte [probe, trace] |
| `SeqMBT2Tick(seq, measure, beat, tick)` | 4 | measure 128 beat 1 = 60960 (1-based) [probe] |
| `SeqTick2Meas(seq, tick32)` | 3 | measure (1-based) [probe] |
| `SeqTick2MBT(seq, tick32, &measure, &beat, &tick)` | 9 | [sweep] |
| `SeqTick2MBTStr(seq, tick32, buffer, flag)` | 6 | returns the buffer: "2:1:039" for 519 [probe] |
| `SeqMBTStr2MBT(seq, string, &m, &b, &t)` | 9 | [sweep] |
| `SeqGetTempoRatio(seq, i)` | 2 | 0x20, 0x40, 0x80 = 0.5, 1.0, 2.0 in 64ths [trace] |
| `SeqWhichTempoRatio(seq)` | 1 | the selected one [trace] |

## Sending MIDI directly

`SeqTxMidiMsg(port, channel, status, data1, data2)` (5 words) sends one channel message at once,
outside playback. Cakewalk Apprentice 1.04's panic button makes 137 such calls, each with port
0xFFFF and channel 0xFF: note-off for all 128 keys, pitch wheel centre, modulation 0, volume 127,
pan 64, sustain, sostenuto and soft pedal 0, "reset all controllers" and "all notes off" [trace]. Through the probe, the same in all three versions
unless noted [probe]:

- The channel argument replaces the low nibble of the status. Port 0xFFFF sends to every port,
  channel 0xFF to all 16 channels.
- The engine knows which notes are on. A note-off (0x8n, or 0x9n with velocity 0) goes out only
  for a note that is on; for a note that is not on nothing is sent, but the result is still 1.
  A second note-on for a note that is on first sends a note-on with velocity 0. A note left on is
  not turned off by `SeqClose`, which sends "all sound off" (controller 120) instead.
- Result 1 when sent; 0, and nothing sent, for pitch wheel (0xEn, whatever the data), statuses
  below 0x80 or above 0xEF, channels above 15, and data bytes above 0x7F. So the panic's pitch
  wheel reset never goes out.
- Port 1, which does not exist with one output port: 1.01 and 2.10 send nothing (result 1), 3.00
  sends on port 0.

## Transport

- Play in Cakewalk Apprentice 1.04: `SeqSetPlayingSeq(seq)`, `SeqSetStandbySeq(0)`, `SeqPlayStart(callback)`;
  Stop: `SeqPlayStop()` [trace]. A null callback works [probe].
- The engine does not stop by itself at the end: `SeqIdle()` notices the end, returns 1 and the
  stop output (All Notes Off) goes out [probe].
- `SeqGetNow(seq)` is polled constantly by all three programs [trace].
- Song Position Pointer: when playback starts, every version sends `F2` with the start position
  in MIDI beats (tick / (ppq / 4)), before Start or Continue, also when MIDI sync is off; 1.01,
  which has no sync output, sends only the SPP [probe, Wine's MIDI trace of `midiOutShortMsg`].
  It follows the file's "Send MIDI SPP" setting (VARS 18): with that byte cleared no version
  sends it [probe]. The 86Box captures never showed an SPP because 86Box's MIDI output does not
  pass it on; midicap itself parses `F2`.
- MIDI sync (2.10 and 3.00, parameter 6): Start or Continue, Clock also while stopped, Stop [probe].

## Playback through the probe compared with libseq 0.8.1

The probe plays each of the 24 engine test songs from its stored Now to the end on each version
(load, `SeqSetPlayingSeq`, `SeqPlayStart`/`SeqStart` with a null callback, `SeqIdle` loop), and
the fake port records the output. Compared with the rendering of libseq 0.8.1 (branch `archive/0.x`) for the same engine
version (from just after the SPP to the stop burst):

- Same messages in the same order for every song but E3_SHTL, on all three versions. E3_SHTL
  loops (the auto-shuttle, above), which that version's engine profiles did not do on their own.
- Times mostly within 10 to 17 ms (the timer jitter the 86Box captures showed), 3.00 mostly
  within 5 ms; the worst ordinary case is 27 ms (2.10, ENG_LOOP). Exceptions:
  - 1.01, ENG_PARM: the stop at the end comes 125 ms late. The demo did the same in 86Box, so it
    is the engine's, not the program's.
  - 2.10 and 3.00, E3_SPPD (locate delay 5): Continue comes 275 ms after the SPP and the first
    note is shorter. This is an artefact of Wine; see "Locate delay" below.
- Wine against 86Box for the same engine (2.10, ENG_ORDR): same order, note times within 3.3 ms.

So the probe can replace the manual 86Box captures for engine questions, on all three versions.

## Locate delay

`SeqPlayStart` sends the SPP, then waits `delay × 55 ms` (VARS 15; 55 ms is the DOS clock tick)
by calling `GetCurrentTime` in a loop, then sends Continue, all before it returns [probe, Wine
relay trace: 2,719 `GetCurrentTime` calls on the program's thread between the two messages].
Under Wine only one thread runs 16-bit code at a time, so the engine's timer callback cannot run
during that loop: no Clock, no events, and the overdue clocks come out in a burst afterwards (15 in
one millisecond), which also shortens the first note. On Windows 3.1 the timer interrupt runs
regardless; the 86Box takes show what the engine does there: the idle Clock continues through the
delay, and playback timing starts at Continue (first note full length, the next one 2 s after
Continue). The same lock can delay the timer callback whenever the program's thread is busy, so
Wine's timing is a little less exact than 86Box's.

## Sysex banks and markers

- A bank handle packs the sequence like a stream handle: 0x0100 is bank 0 of sequence 1, in all
  three versions [probe: ENG_SYSX, bank 0 "GM on", `SeqGetSysxBankLen(0x0100)` = 6].
  `SeqGetSysxBankName(bank, buffer, size)` (4 words) returns the name's length;
  `SeqSetSysxBankName(bank, name)` 3; `SeqGetSysxAscLen(bank, flag)` 2 (31 for the 6-byte bank
  with flag 1); `SeqEmptySysxBank(bank)` 1 [probe].
- `SeqGetMarkerName(seq, index32, buffer, size)` (6 words) returns the name's length ("Locked",
  6) [probe]; marker indexes are 32-bit, as in `SeqGetMarkerTime` [trace].
- `SeqConfigMidi(hwnd)` (1 word) shows the "Select MIDI Devices" dialog and returns the button
  (2 = Cancel) [probe].
- `SeqSysxAsc2Bin(bank, text, &position)` turns the text of the "Edit Bytes" dialog back into the
  bank's bytes; Cakewalk Professional 2.00 calls it on OK and says "Not a number!" when it returns
  4 [trace]. Through the probe [probe]:
  - Pairs of hex digits separated by white space (including CR LF), upper or lower case; a group
    of one digit is one byte, a group of three is split after two digits ("123" is 12 03). Data
    bytes above 0x7F and F0 inside a message are accepted. Several messages may follow each
    other.
  - Results: 0 done; 1 the text does not start with F0 (also an empty text); 2 the last message
    has no F7; 4 a character that is not hex (a comma too), whose offset in the text goes to the
    32-bit value at `position`. On any error the bank keeps its old bytes.
  - `SeqSysxBin2Asc` and `SeqSysxAsc2Bin` are inverse: the banks of ENG_SYSX come back unchanged.
- `SeqReceiveSysxBank(bank, callback, 0, 0, 0)` (6 words), and in 3.00 also
  `SeqReceiveSysxBankEx(bank, callback, 0, 0, 0, 0, 0)` (8 words), which is what Cakewalk
  Professional 2.00 calls from its Sysx view [trace]:
  - The engine hands the MIDI input driver a buffer for system exclusive data
    (`midiInPrepareHeader`, `midiInAddBuffer`) [relay] and calls the callback in a loop, with
    the number of bytes received so far and the free memory (`GetFreeSpace`), both 32-bit.
    Professional shows them as "14 bytes received (space for 2109181952)" until Done.
  - The loop ends when the callback returns non-zero. The result is 1, or -2 (0xFFFE) when the
    bank is still empty; -4 when the input buffer could not be prepared (unpatched Wine;
    Professional then says "Out of memory") [probe, trace, relay].
  - Every complete message is appended to the bank, also to a bank that already has bytes.
    Universal messages (F0 7E and F0 7F) are left out; channel messages are ignored [probe].
  - The other words, zero in Professional's call, had no visible effect [probe].
- Receiving sysex under Wine needs Wine's 16-bit MMSYSTEM to pass input buffers between 16 and
  32 bits, which Wine 9.0 does not (`tools/ttsprobe/wine/mmsystem16-midiin.patch`).

## Files

- `SeqLoadCakewalkFile(name, 0, callback, value32)` in 1.01/2.10, `(name, 0)` in 3.00; returns the
  new sequence [trace, probe]. The callback is the program's `LoadProc`, called once per chunk
  the engine does not keep itself, with the chunk type and the file handle; the program reads
  the chunk with `SeqIORead(file, buffer, count)` [trace].
- `SeqSaveCakewalkFile(seq, name, value32, callback, value32)` (9 words) in 1.01/2.10; the
  program's `SaveProc` writes its chunks with `SeqIOWrite` (Cakewalk Apprentice 1.04: the comments) [trace].
- `SeqSaveMidiFile(seq, name, value32)` (5 words) [trace].
- The same song saved by each version through the probe (no callback) [probe]:
  - 1.01 and 2.10 write the same file except VARS byte 77 (transmit MIDI sync): 1.01 writes 0, it
    does not know the setting; 2.10 writes 254 ("off"). Without the program's callback both leave
    out the program's chunks: the string event table (22) and the comments (8).
  - 3.00 writes those itself: the comments right after VARS, then a new 6-byte chunk 28, which
    the earlier versions never write. It writes chunk 22 only when event kinds are registered
    (Professional's file has "MCICmd", "Wave", "Text").
  - The MIDI files of 1.01 and 2.10 are identical; 3.00's differs.
- Chunk 28 (3.00): three 16-bit words, 6, then parameter 31, then parameter 30. Every file of
  Cakewalk Professional 2.00 holds `06 00 01 00 00 00` [probe].
- Chunk 16 holds the MIDI thru settings (parameters 7 to 11), chunk 17 the metronome note
  (parameters 17 to 21) [probe].

### Baggage (3.00)

Named blocks of program data kept with the sequence and saved as chunk 26 (one per block: the
name, zero-padded to 32 bytes, then the data) [probe]:

| Call | Words | Observed |
|---|---|---|
| `SeqSetBaggage(seq, name, data, size32)` | 7 | stores a copy [probe, trace] |
| `SeqSetBaggageString(seq, name, string)` | 5 | the same with a zero-terminated string [probe] |
| `SeqGetBaggageSize(seq, name)` | 3 | the size, or -1 when there is no such block [probe] |
| `SeqGetBaggage(seq, name, buffer, size32)` | 7 | returns the size [trace] |
| `SeqGetBaggageDataPtr(seq, name)` | 3 | a far pointer to the data [trace] |

Cakewalk Professional 2.00 keeps "FILESTATS", "Copyright" and "Author" there (File > Info) [trace]. This is
how 3.00 replaces the program callbacks of the file functions.

### TTSSEQ.INI

All three versions read `TTSSEQ.INI` (3.00 too; its file has no other INI name) [exp strings].
Sections `[MIDI Output Devices]` and `[MIDI Input Devices]` (`0=<device name>`, ...) and
`[Options]`. The option names in the files: `SyncOutputPort`, `ConfigMIDI`, `Timebase`,
`FlywheelAmount`, `ChaseDelay`, `SysxSendPacketSize`, `SysxReceiveBuffers`,
`SysxReceiveBuffersSize`, `SendLocalOff`, `TimerPeriod`, `TimerResolution`. `SendLocalOff`
belongs with the Local Control Off that every engine sends when it opens a port. Only the names are
known; their effects are not tried yet.

## Recording

The fake sequencer now has an input port too ("Fake - MIDI In", fed from a script of timed
messages), so the probe can record without a program [probe]. Wine's input port reports
manufacturer 255 as well and needed the same patch as the output (`patch-winealsa.py`).

The calls, as Cakewalk Apprentice 1.04 and Cakewalk Professional 2.00 make them [trace] and the probe repeats them:

- 1.01 and 2.10: `SeqSetPlayingSeq(seq)`, `SeqSetStandbySeq(0)`, `SeqRecordStart(w, w, w, w, w,
  callback)` (7 words; Cakewalk Apprentice 1.04 passes five zero words and its `StartProc`); recording runs;
  `SeqRecordStop()`; `SeqRecCount()` = events taken; `SeqRecordCleanup(seq, keep, 0, &stream)`
  (5 words) puts the take into a new stream and writes its handle (0x0100) to the far pointer.
  Cakewalk Apprentice 1.04 asks "Keep take?" between the last two.
- 3.00: `SeqRecord(1)`, `SeqSetPlayingSeq(seq)`, `SeqSetStandbySeq(0)`, `SeqStart(callback,
  value32)`; `SeqGetRecordBuffer()` returns the stream that collects the take (0x0100);
  `SeqSetRecordBufferParams(0, 0, 0)`. To stop: `SeqGetRecordBuffer()`, `SeqRecord(0)`,
  `SeqStop()`. Professional then merges the buffer into a track (`SeqMergeStreams`, 9 words) and
  sets stream parameters 12 to 15 of the buffer around the take.
- Both programs set sequence parameter 3 to 1 after a take.

The count-in: the start call (`SeqRecordStart`, or `SeqStart` in 3.00) does not return until the
count-in is over (2.4 s for one 4/4 measure at 100 bpm). Meanwhile it calls the callback in a
tight loop with (1, beats left, 0): 4 once, then 3, 2, 1 and 0, the value changing on each beat.
This is `StartProc`; the programs show and update a count-in window from it. The callback must
return 0, or the start call gives up at once and returns 0 (2.10, with the callback returning 1).
Under Wine it must also yield (`PeekMessage`), or the timer that ends the count-in never runs
(the probe's `cbyield 1`). With no count-in the callback is not called; that is why playback never
called it [probe].

The take, the same in all three versions for the same input (one count-in measure, 100 bpm,
120 ppq, so 5 ms per tick) [probe]:

- Messages received during the count-in are recorded at tick 0; a note whose note-off also came
  during the count-in gets duration 0.
- After the count-in, times are the arrival time in ticks; a note's duration is note-off minus
  note-on; Note On with velocity 0 ends a note like Note Off. A drum note held 10 ms got duration 2.
- A note still held when recording stops keeps duration 0; the engine sends its note-off then.
- The filter (parameter 28) applies: with the new sequence's 0xDB, Key and Channel Aftertouch are
  not recorded. Controllers, wheel and patch changes are recorded with duration 0.
- Every message is echoed to the output within about 5 ms (MIDI thru), the filtered ones too.
- A sysex message from the input is neither recorded nor echoed. While recording, the engine
  gives the MIDI input driver no buffers for system exclusive data (`midiInAddBuffer`), so the
  driver has nowhere to put them; only `SeqReceiveSysxBank` does [probe, relay].

Cakewalk Professional 2.00 under Wine stalls for 2 s after the count-in: it hides its count-in window
and Wine waits for a window manager that the virtual display does not run
(`wait_for_withdrawn_state`). The probe has no windows and no stall.

## Program callbacks

Cakewalk Apprentice 1.04 exports them by name, so the tracer shows the engine calling them: `LoadProc` and
`SaveProc` (4 words: a code, the file handle, a 32-bit value), `StreamsProc`, `MetersProc`,
`MarkerListProc`, `SysxBanksProc`, `StartProc` [exp of WCWLITE.EXE, trace]. Through the probe's
callback thunks [probe]:

- `SeqEnumStreams(seq, callback, value32)` and `SeqEnumSysxBanks(seq, callback, value32)` (5
  words, also in 2.10) call back with (seq, stream or bank, value32) per item, then once with
  (0, 0, value32). The callback's result is ignored.
- `SeqRegisterEventKind(name, proc, value32)` (6 words) returns the new kind (1, 2, 3, ...). The
  engine keeps the name pointer, not a copy. An event procedure takes 4 words (a message, a far
  pointer, a length): message 4 while loading (it must return 1), 1 while playing; message 3 came
  with a failed load. Loading a file with string events of a registered kind fails unless the
  program's `LoadProc` is there. `SeqCallEventProc` takes 4 words.
- `StartProc`: the count-in callback (Recording, above).

## Copy and paste

From Cakewalk Apprentice 1.04 (2.10) and Cakewalk Professional 2.00 (3.00) doing Edit > Copy and Paste [trace]:

- The clipboard is a sequence of its own: sequence 1 in both traces, the song being sequence 2.
  For undo, Cakewalk Apprentice 1.04 also makes sequence 3, with an empty clone of the track
  (`SeqCloneStream(3, track, 1)`) and a copy of the whole tempo map.
- `SeqTempoCopy(src, clip, from32, thru32, 0, pointer)` (10 words);
  `SeqTempoPaste(clip, dst, at32, value32, 1, 0 or 1)` (8 words).
- `SeqMeterCopy` (8 words); `SeqMeterPaste(clip, dst, at32, 1, 0)` (6 words). Cakewalk Apprentice 1.04 refuses
  a meter paste off a measure boundary with an assertion ("Meters and non-measure boundary",
  edit1.c line 806).
- `SeqMarkerCopy` (3.00, 10 words) has the layout of `SeqTempoCopy`.
- "Slide over existing material" moves the events later with `SeqDisplaceLater(stream, at32,
  amount32)` (5 words) in both programs; neither calls `SeqInsertRange`.
- The events go in with `SeqMergeStreams`: Cakewalk Apprentice 1.04 pastes at 0x780 with
  `SeqMergeStreams(2, 0x0200, 1, 0x0100, 0, 0x780)` and from then on uses the stream it returns
  (0x0201) as the track; Cakewalk Professional 2.00 adds the number of clipboard events,
  `(2, 0x0200, 1, 0x0100, 0, 10, 0x1E0)`, and merges a recorded take the same way,
  `(1, 0x0000, 1, 0x0100, 1, 7, 0)` [trace].
- Through the probe [probe], `SeqMergeStreams(seq, a, seq_b, b, flag, offset32)`, in 3.00
  `(seq, a, seq_b, b, flag, count32, offset32)`:
  - It makes a new stream in `seq` (the lowest free handle) and returns it. The new stream has
    the events of `a` and those of `b` moved later by `offset`; in 3.00 only the first `count`
    events of `b` (more than `b` has means all of them).
  - At equal times the events of `a` come before those of `b`.
  - `a` is destroyed; `b` is kept, or destroyed too when `flag` is not 0 (Professional passes 1
    for the record buffer).
  - The new stream keeps the stream parameters and the name of `a`.
  - There is no time-base conversion: with `seq` at 240 ppq and `b`'s sequence at 120 ppq, the
    ticks of `b` are taken as they are.
  - The third word had no visible effect in any test; the programs pass the sequence of `b`.

## Call layouts

`signatures.md` lists the layout and result type of every
function the oracle suites call, found by trying layouts through the probe. Notes from that work
[probe]:

- Handles are the lowest free number: after destroying sequence 1, the next one is 1 again.
- An invalid sequence, stream or bank handle crashes the engine or returns stack contents; the
  suites only use handles that exist.
- `SeqSetSeqParam` returns 1, or 0 for a parameter the version does not have.
- `SeqCreateStream(seq, ?, ?)` adds an empty stream; `SeqAdoptStream(seq, stream)` gives it a new
  handle; `SeqCloneStream(seq, stream, flag)` copies the events with flag 0 and not with flag 1.
- The meter map, from Cakewalk Apprentice 1.04's Meter/Key view [trace] and the probe [probe]:
  `SeqInsMeter(seq, measure, key, meter)` adds an entry (or replaces the one at that measure),
  `SeqSetMeter(seq, tick32, key, meter)` changes the entry in effect at the tick, and
  `SeqDelMeter(seq, measure)` deletes the entry in effect at the measure, also the one at measure
  1. Apprentice calls `SeqRecalcMeterMap(seq)` after each; it returns the number of entries.
  `SeqSetMeter` recalculates the positions itself.
- `SeqInsMarker(seq, name, time32, flag)` keeps the markers in time order;
  `SeqGetMarkerTime(seq, index32, &time, &flag)` and `SeqSetMarkerTime(seq, index32, time32, flag)`
  carry the same flag (the "Locked" marker of the test file suggests SMPTE lock).
- `SeqGetSysxAscLen(bank, n)` and `SeqSysxBin2Asc(bank, buffer, n)` take the number of bytes per
  line; `SeqGetSysxAscLen` with 0 divides by zero.
- `SeqUnregisterEventKind(name)` takes the kind's name; a freed kind number is reused. A file with
  string events loads only when no kinds are registered, or with the program's load callback.
- `SeqSetTempo(seq, tick32, tempo)` changes the entry in effect at the tick, `SeqInsTempo` adds
  one, `SeqDelTempo(seq, tick32)` removes the entry in effect at the tick (also the one at 0).
- 3.00 does not preserve SI, DI and DS across some calls (`SeqEnumTempos`, `SeqEnumMeters`); the
  probe now saves them itself. Earlier crashes in 3.00 may have had this cause.

## Open

- Sizes: all exports that are API are measured except `SeqInsertRange` in 3.00 (4 words in 1.01
  and 2.10; every layout tried crashes in 3.00, and no program was seen calling it).
- The step record calls return but record nothing without MIDI input.
- `SeqInsertRange` (4 words in 1.01 and 2.10) has no layout that works, and no program imports
  it. On a loaded stream every call asks Windows to grow the stream's memory by 0xA011F000 bytes,
  whatever the arguments; that fails, the stream is unchanged, and the result is 0. On a new,
  empty stream the call grows the memory and then crashes [probe, relay].
- The third word of `SeqMergeStreams` and the zero words of `SeqReceiveSysxBank` and
  `SeqReceiveSysxBankEx`: no visible effect yet.
- Parameters 3, 30 and 31: not from any dialog, no effect on playback; probably recording.
- The five leading words of `SeqRecordStart` (1.01, 2.10) and the parameters of
  `SeqSetRecordBufferParams` (3.00); punch-in and loop recording ("Reject Loop Take",
  `SeqCancelLoopRecordTake`, `SeqEnumRecordStreams`).
- The `[Options]` settings of `TTSSEQ.INI`.
- Wine 9.0's ALSA driver passes Song Position Pointer and other system common messages to ALSA
  in a buffer that has gone out of scope, so the fake port logs them as zeros (`sysex 00 00 00`);
  Wine's `+midi` trace has the real bytes.
