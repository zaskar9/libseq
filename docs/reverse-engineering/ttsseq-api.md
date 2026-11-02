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
  (a callback and a far pointer to a 32-bit value) where `SeqPlayStart` took 2 (a callback) [exp,
  sweep, trace, probe; see "External sync"].

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
| 3 | "Now jumped": a flag the engine sets when Now is set or jumps (below); no effect found | | yes | yes | yes |
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

Parameter 3 is a flag: any value other than 0 reads back as 1. The engine sets it whenever Now is
set or jumps: creating or loading a sequence, `SeqSetNow` (also to the value it has), `SeqRewind`,
every start (playback or recording) and the auto-shuttle's wrap. It leaves it as it is for
edits, tempo changes, saves, stopping, the end of a song and Now moving during playback; only the
program clears it. Cakewalk Apprentice 1.04 never reads it, and both programs set it to 1 after
a take. With it cleared, nothing tried came out different: the search-back at the start, Start or
Continue and the SPP of the sync output, saved `.WRK` and `.MID` files, thru, `SeqRecordEvent`,
step recording, position calls, switching sequences, and playing on after events were inserted
in front of the position, stopped or playing [probe, suite `param3`].

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
  sends it [probe].
- With Song Position on (parameter 4), every start, playback or recording, first rounds the start
  position down to a MIDI beat, ppq / 4 ticks: from Now 45, playback starts at 30, `SeqGetNow`
  reads 30 as soon as the start call returns, and a take counts from 30 (at 96 ppq, 45 becomes
  24). With it off the start is exact. All three versions [probe, suite `punch`]. This explains
  why Cakewalk Apprentice 1.04 resumed 5 to 25 ticks before where it had stopped in the 86Box
  captures (`tests/data/cakewalk/engine/`): those songs send Song Position. The 86Box captures never showed an SPP because 86Box's MIDI output does not
  pass it on; midicap itself parses `F2`.
- MIDI sync (2.10 and 3.00, parameter 6): Start or Continue, Clock also while stopped, Stop [probe].
- A start call while playing returns 0 [probe].
- `SeqExpired()` is 1 while the transport is stopped and 0 while it plays, from the moment the
  start call returns. At the end of the song, or after a Stop from outside, it turns 1 when
  `SeqIdle` reports the stop, not before [probe, suite `sync`].

## External sync

All of this is the same in 1.01, 2.10 and 3.00 unless noted [probe, suite `sync`, and single runs
of `tools/ttsprobe/sweep/syncexp.py`]. The fake MIDI input sent MIDI Clock (`F8`), Start (`FA`),
Continue (`FB`), Stop (`FC`), Song Position Pointer (`F2`) and MIDI Time Code quarter frames
(`F1`).

### The clock source

- `SeqSetSrcClock` takes 0 (the internal timer), 1 (MIDI sync) or 3 (MIDI Time Code) and returns
  1. Any other value (2, 4, 0x103, 0xFFFF) returns 0 and leaves the source as it was. Cakewalk for
  DOS also synced to FSK; 2 may have been its number. A `.WRK` file sets the source on load from
  VARS byte 13, through the same check.
- 1.01 has one source for the engine. 2.10 and 3.00 keep one per sequence, and the playing
  sequence's source is taken when `SeqSetPlayingSeq` makes it the playing sequence. A source
  changed after that only decides whether the start call waits: set to 1 after
  `SeqSetPlayingSeq`, the start waits for Start, but then neither the clock nor the timer moves
  the playback. Programs set the source first, as Cakewalk Apprentice 1.04 does.
- Cakewalk Apprentice 1.04 has no clock setting. After loading a file it calls
  `SeqSetSrcClock(seq, 0)`, so its songs always play on the internal clock [trace under Wine].
  Its strings "Follow MIDI Sync", "Follow SMPTE/MTC", "Waiting for MIDI Sync" and "Chasing to
  %02u:%02u:%02u:%02u." belong to the full program.
- On the internal clock, incoming Start, Continue, Stop and clocks are ignored [probe, Apprentice
  under Wine].

### Waiting for Start

- With source 1 the start call (`SeqPlayStart`, `SeqStart` in 3.00) returns only when Start or
  Continue comes in. Clocks, Song Position and Stop do not end the wait, and it does not time out
  (20 s tried). The call sends the search-back output for Now first, as on the internal clock.
- Meanwhile it calls its callback (`StartProc`, as for the count-in) with (2, 0, 0) once, then
  with (5, 0, 0) over and over. The callback returns 0 to go on waiting; anything else gives up,
  and the start call returns 0. With a null callback the engine waits in a loop that calls nothing
  at all. On Windows 3.1 the MIDI input interrupt ends it; under Wine that needs a changed Wine
  (`tools/ttsprobe/wine/krnl386-irq.patch`), or the program hangs.
- A Stop that comes in while the engine waits is kept: the start call returns 1 at the Start, and
  the playback stops at once (All Notes Off, `SeqIdle` returns 1). A Stop before the start call
  is forgotten.
- 3.00's `SeqStart(callback, p start)` writes the tick where the playback starts to the dword at
  `start` (0 from the beginning, the Song Position, the time of the time code). Cakewalk
  Professional 2.00 passes a null pointer [trace]; a pointer that is not valid crashes at the start.

### Following MIDI clock

- Start plays from tick 0. Continue plays from the last Song Position received, else from where
  the playback stopped. A Song Position while stopped does not change `SeqGetNow` until the
  Continue; while playing it does not move the playback, but it is kept for the next
  Continue. At a Song Position during the wait, and at Continue, the engine sends the search-back
  output for the new position.
- A clock is ppq / 24 ticks (5 at 120 ppq; 20 at 480, 4 at 96, 2 at 48 tried). The first clock
  after Start plays the events of ticks 0 to 4, the next one those of 5 to 9, and so on: all
  events of a clock's span go out together when the clock comes in, a few milliseconds later
  (the timer's period), not spread over the span. Shown with events off the grid (QUANT: ticks 1,
  7, 17, 37, 53; a copy of its second track with events at ticks 10, 12, 16, 17, 19 and 20).
- The song's tempo map plays no part: the time between clocks is the tempo. When the clocks pause,
  the playback holds where it is, notes stay on, and nothing times out.
- `SeqGetNow` after a clock: the time of the last event that clock played; if it played none, the
  previous value plus ppq / 24. The first clock after Start or Continue does not add. With QUANT
  the values after clocks 0, 1, 2, ... are 1, 7, 12, 17, 22, 27, 30, 37, 42, 47, 53; with
  ENG_ORDR 0, 5, 10, 15. Continue at 177 and four clocks give 192.
- Stop ends the playback: All Notes Off on every channel, and `SeqIdle` returns 1.

### Following MIDI Time Code

Tried with 30 fps quarter frames from 00:00:00:00, 00:00:01:00 and 00:00:10:00, songs at a
constant 120 bpm and no SMPTE offset:

- With source 3 the start call calls back (3, 0, 0) once and (5, 0, 0) while no code comes in.
  Once the code has been read (two frames), the engine sends the search-back output and calls
  back (4, seconds × 256 + frame, 0) with the incoming time, for about 2 s (from 10:04 to
  12:03 for code that began at 10:00; `ChaseDelay` in `TTSSEQ.INI`, in tenths of a second). Then
  the start call returns, and `SeqGetNow` is the time
  of the code in ticks (12.25 s: 2940 at 120 bpm and 120 ppq; 3.00's pointer gets the same).
- The playback follows the code. When the code stops, the engine runs on for about 0.5 s
  (`FlywheelAmount`, in tenths of a second), then stops (All Notes Off, `SeqIdle` returns 1). Code that starts after the song's end runs on past
  it.
- Open: the SMPTE offset (the file's TIMEFMT), other frame rates, tempo changes, hours and minutes
  in the callback's third word.

### Under Wine

- The engines rely on Windows 3.1 calling the timer and MIDI input callbacks at interrupt time:
  the program stands still while a callback runs. Wine calls them only while no thread runs
  16-bit code. `krnl386-irq.patch` with `WINE_WIN16_IRQ_CALLBACKS=serial` calls them at once and
  one at a time, in the order they come, stops the program's 16-bit code while one runs, and lets
  the engine's own `cli` ... `sti` hold them off (the engines use them in pairs, at one to four
  places).
- A first version of the patch let the program run on beside the callbacks. Then a Continue after
  a Song Position left the engines deaf to the clock in about one run in five, on all three
  versions: Now stayed at the position and nothing played until Stop. It happened only while the
  start call waited without a callback, in a loop that runs on beside the callbacks; with a
  callback that yields, on unchanged Wine, 30 runs gave no failure. With the program stopped
  during callbacks, 30 runs per version with the tightest timing (Continue 200 ms after the Song
  Position, first clock 50 ms later) gave none either. So it was the emulation, not the engines.
  The same change ended a time code run that once stopped early.

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
  - 2.10 and 3.00, E3_SPPD (locate delay 5): on unchanged Wine, Continue comes 275 ms after the
    SPP and the first note is shorter. This was an artefact of Wine; with interrupt-time
    callbacks, which the suite now uses, the playback matches the 86Box capture (see "Locate
    delay" below).
- Wine against 86Box for the same engine (2.10, ENG_ORDR): same order, note times within 3.3 ms.

So the probe can replace the manual 86Box captures for engine questions, on all three versions.

## Locate delay

`SeqPlayStart` sends the SPP, then waits `delay × 55 ms` (VARS 15; 55 ms is the DOS clock tick)
by calling `GetCurrentTime` in a loop, then sends Continue, all before it returns [probe, Wine
relay trace: 2,719 `GetCurrentTime` calls on the program's thread between the two messages].
On Windows 3.1 the timer interrupt runs regardless; the 86Box takes show what the engine does
there: the idle Clock continues through the delay, and playback timing starts at Continue (first
note full length, the next one 2 s after Continue). On unchanged Wine only one thread runs 16-bit
code at a time, so the engine's timer callback cannot run during that loop: no Clock, no events,
and the overdue clocks come out in a burst afterwards (15 in one millisecond), which also
shortens the first note (214 ms instead of 490) and brings the next one 1.71 s after Continue.
With interrupt-time callbacks (`krnl386-irq.patch`, `#! irq`), the probe gives what 86Box gave:
Clock every 20 ms or so through the delay, no burst, the first note 5 to 9 ms after Continue and
490 to 498 ms long, the next one 2.0 s after Continue (2.10 and 3.00, two runs each). The suite
`play_E3_SPPD` runs this way. 1.01 has no locate delay and plays the same either way. The Win16
lock can also delay the timer callback whenever the program's thread is busy, so Wine's timing
without the patch is a little less exact than 86Box's.

## Sysex banks and markers

- A bank handle packs the sequence like a stream handle: 0x0100 is bank 0 of sequence 1, in all
  three versions [probe: ENG_SYSX, bank 0 "GM on", `SeqGetSysxBankLen(0x0100)` = 6].
  `SeqGetSysxBankName(bank, buffer, size)` (4 words) returns the name's length;
  `SeqSetSysxBankName(bank, name)` 3; `SeqGetSysxAscLen(bank, flag)` 2 (31 for the 6-byte bank
  with flag 1); `SeqEmptySysxBank(bank)` 1 [probe].
- `SeqGetMarkerName(seq, index32, buffer, size)` (6 words) returns the name's length ("Locked",
  6) [probe]; marker indexes are 32-bit, as in `SeqGetMarkerTime` [trace].
- `SeqConfigMidi(hwnd)` (1 word) shows the "Select MIDI Devices" dialog (Devices, above).
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
  - The engine prepares `SysxReceiveBuffers` buffers of `SysxReceiveBuffersSize` bytes (128 and
    512 by default, `TTSSEQ.INI`). A longer message fills several buffers. 1.01 and 2.10 look for
    universal sysex in every buffer, not every message: a buffer whose second byte is 7E or 7F is
    left out, even in the middle of a message. 3.00 looks at messages only [probe, suite
    `options1`].
- `SeqSendSysxBank(bank, 0, 0)` sends the bank's bytes in packets of `SysxSendPacketSize` bytes
  (1024 by default), cut without regard to where messages begin and end, one `midiOutLongMsg`
  each. Between packets the engine waits about 65 ms in a loop that reads `GetCurrentTime`, before
  the call returns; the wait does not depend on the packet size or the timer settings [relay,
  probe]. Wine's ALSA driver adds an F0 or F7 to a packet that does not begin or end with one,
  so the fake port logs every packet as a message of its own.
  - The other words, zero in Professional's call, had no visible effect [probe].
- Receiving sysex under Wine needs Wine's 16-bit MMSYSTEM to pass input buffers between 16 and
  32 bits, which Wine 9.0 does not (`tools/ttsprobe/wine/mmsystem16-midiin.patch`).

## Files

- `SeqLoadCakewalkFile(name, 0, callback, value32)` in 1.01/2.10, `(name, 0)` in 3.00; returns the
  new sequence [trace, probe]. The callback is the program's `LoadProc` (below).
- `SeqSaveCakewalkFile(seq, name, value32, callback, value32)` (9 words) in 1.01/2.10, with the
  program's `SaveProc` [trace].
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

### The file callbacks and `SeqIO` (1.01, 2.10)

The program's `LoadProc` and `SaveProc` are called as `callback(code, word, value32)`, the value
being the one passed to the load or save call [trace of Cakewalk Apprentice 1.04, probe; the
`fileio` suite]:

- Load: `0xFE` with the open file first (Apprentice seeks to the end and back to size its
  progress bar), then once for every chunk, with the chunk's type and the file standing at the
  chunk's 32-bit length. The callback returns 1 when it has read the chunk itself and 0 to leave
  it to the engine; returning 1 without reading leaves the engine out of step with the file.
  Apprentice takes only the comments (8): it reads the length (4 bytes), the text length (2)
  and the text. `0xFF` with the file comes at the end.
- Save: `0xFE` with a percentage (0, 14, 28, ... 100) for the progress bar, and `0xFF` with the
  open file at the end, before the engine's end marker: the program appends its own chunks there
  (Apprentice: the comments, as type byte, length, text length, text).
- Code 3 comes once in both, with a word that is not the file; its meaning is not known.
- `SeqIOSeek(file, offset32, origin)` (origin 0 start, 1 current, 2 end) returns the new position;
  `SeqIORead(file, buffer, count)` and `SeqIOWrite(file, buffer, count)` return the bytes moved
  (the count is 16 bits in 1.01 and 2.10, 32 bits in 3.00).
- String events: during a load the event procedure of the kind is called with message 4 and the
  event's 7 bytes; it reads the event's text from the file with `SeqIORead`, the 32-bit length
  first, using the file from the load callback. If it does not, the load fails. This is why a
  file with string events of a registered kind loaded only with the program's load callback.
- 3.00's file functions have no callback, and no program was seen calling `SeqIOSeek/Read/Write`
  in 3.00; how 3.00 reads string events is open.

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
Sections `[MIDI Output Devices]` and `[MIDI Input Devices]` (`0=<device name>`, ...; below) and
`[Options]`. Wine's relay trace of `GetPrivateProfileInt` shows which keys each version reads,
with which default, and when; changing one key at a time showed what each does [relay, probe;
`tools/ttsprobe/sweep/options.py`, suites `options1`, `options2`, `ports`]:

| Key | Default | Read | Effect |
|---|---|---|---|
| `SyncOutputPort` | 1 | `SeqOpen` | the port for MIDI sync output (2.10, 3.00; 1.01 sends none). A value outside the output ports (0, or more than there are) shows "An invalid Sync Output Port 'n' was specified. Please re-check the following list of devices to use..." and then the device dialog, in all three versions; Cancel there makes `SeqOpen` return 3 with no port open |
| `SendLocalOff` | 1 | `SeqOpen` | Local Control Off (CC 122 = 0) on all 16 channels when the port opens; 0 sends none, any other value counts as 1 |
| `SendLocalOn` | 0 | `SeqClose` (2.10, 3.00) | not 0: Local Control On (CC 122 = 127) on all 16 channels when the port closes, before All Sound Off. 1.01 does not read it |
| `ForceNoteOff` | 0 | `SeqOpen` (3.00) | not 0: the end of a note goes out as 8n kk 40 instead of 9n kk 00. The note-off the engine sends when a key that is sounding starts again stays 9n kk 00; `SeqTxMidiMsg` sends what it is given |
| `Timebase` | 120 | `SeqOpen` | the PPQ of new sequences (`SeqCreateSeq`) in 2.10 and 3.00. Below 24 counts as 24, above 480 as 480 (in 3.00 also -5, read unsigned); of the values in between, 24, 48, 72, 96, 168, 192, 360, 384 and 480 were kept, and 25, 47, 60, 100, 200 and 400 gave 120. 1.01 reads it, but its new sequences stay at 120 |
| `TimerPeriod`, `TimerResolution` | 10 and 5; 3.00: 2 and 1 | 1.01: `SeqOpen`, every file load and `SeqSetPlayingSeq`; 2.10: `SeqOpen` and `SeqSetPlayingSeq`; 3.00: `SeqOpen` | the engine's multimedia timer: `timeSetEvent(period, resolution, ..., periodic)` and `timeBeginPeriod(resolution)`; 0 counts as 1. They change only the timing. `SeqConfigMidi` (3.00) writes High 2 and 1, Medium 5 and 2, Low 10 and 5 |
| `SysxSendPacketSize` | 1024 | `SeqOpen` | the packet size for sending sysex (above); 0 sends one byte at a time |
| `SysxReceiveBuffers`, `SysxReceiveBuffersSize` | 128 and 512 | `SeqOpen` | the input buffers for receiving sysex (above). With fewer than 2 buffers the receive returns -4 when a message comes in; a size of 0 crashed the probe |
| `RecordLengthDesired` | 16 | `SeqOpen` (2.10, 3.00) | the record buffer: n × 8 KB + 256 bytes (`GlobalReAlloc`), 1024 events per 8 KB. Events beyond it are lost: with 1, a take of 1100 notes keeps 1024 (2.10's `SeqRecCount`, 3.00's record buffer). 0 counts as 16. 1.01 does not read it and kept all 1100 |
| `ChaseDelay` | 20 | `SeqOpen` | MIDI Time Code: how long the engine locks to the code before the start call returns, in tenths of a second (5: 0.5 s, 40: 4 s). No effect on MIDI clock |
| `FlywheelAmount` | 5 | `SeqOpen` | MIDI Time Code: how long the engine runs on when the code stops, in tenths of a second (1: 0.1 s, 20: 2 s). No effect on MIDI clock |
| `SyncAllowRateTweak` | 1 | with the timer keys | no visible effect: not on following MIDI clock, not on time code 5 % fast, not on the sync output |

Other names in the files: `ConfigMIDI` (the dialog). Defaults and times are the same in all three
versions except where noted. `SyncOutputPort`, `TimerPeriod` and `TimerResolution` are written by
`SeqConfigMidi` (below); the others are only read.

### Devices: `SeqConfigMidi` and the port assignment

`SeqConfigMidi(hwnd)` shows the "Select MIDI Devices" dialog (`devices/dialog-*.png`): the input
and output devices Windows offers, a button that moves the selected devices to the top of the
list, and the port for sync output; 3.00 adds the timer precision. It returns 3 for OK and 2 for
Cancel. Observed with two fake output ports and `tools/ttsprobe/sweep/configmidi.sh`, the same in
all three versions except the timer (`devices/configmidi-*.txt`) [probe]:

- OK writes `TTSSEQ.INI`: the selected output devices in list order as `[MIDI Output Devices]`
  0, 1, ..., likewise the inputs, and `SyncOutputPort`. The order is that of the list, not of
  selection; "Move selected devices to top" is how to change it. Cancel writes nothing and
  changes nothing.
- The new assignment takes effect at once, without a new `SeqOpen`: Cakewalk port n is device
  n - 1 of the list, as `SeqGetPortName` and the MIDI from `SeqTxMidiMsg` show.
- `SeqGetPortName(port, buffer)` writes "n: device"; for a port beyond the assigned ones it
  repeats the first device's name with that number.
- `SyncOutputPort` decides where MIDI sync goes: with 2, Start, Clock, Stop and the Song Position
  Pointer go to port 2 (2.10, 3.00; the `ports` suite, which gives the engine such an INI file).
- 3.00's timer precision: High writes `TimerPeriod=2`, `TimerResolution=1`; Medium 5 and 2; Low
  10 and 5 (milliseconds); Custom takes both values from the dialog.

libseq does not port the dialog. The state behind it (which devices are which ports, which port
sends sync) belongs to libseq's platform interface; the application draws its own dialog and
keeps the settings.

## Recording

The fake sequencer now has an input port too ("Fake - MIDI In", fed from a script of timed
messages), so the probe can record without a program [probe]. Wine's input port reports
manufacturer 255 as well and needed the same patch as the output (`patch-winealsa.py`).

The calls, as Cakewalk Apprentice 1.04 and Cakewalk Professional 2.00 make them [trace] and the probe repeats them:

- 1.01 and 2.10: `SeqSetPlayingSeq(seq)`, `SeqSetStandbySeq(0)`, `SeqRecordStart(stream,
  in32, out32, callback)` (7 words; Cakewalk Apprentice 1.04 passes 0, 0, 0 and its `StartProc`;
  the first three are the punch, below); recording runs;
  `SeqRecordStop()`; `SeqRecCount()` = events taken; `SeqRecordCleanup(seq, keep, 0, &stream)`
  (5 words) puts the take into a new stream and writes its handle (0x0100) to the far pointer;
  with keep 0 it writes 0 and the take is gone [probe].
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

- Messages received during the count-in are recorded at the start tick (tick 0 from the
  beginning); a note whose note-off also came during the count-in gets duration 0.
- After the count-in, times are the arrival time in ticks; a note's duration is note-off minus
  note-on; Note On with velocity 0 ends a note like Note Off. A drum note held 10 ms got duration 2.
- A note still held when recording stops keeps duration 0; the engine sends its note-off then.
- The filter (parameter 28) applies: with the new sequence's 0xDB, Key and Channel Aftertouch are
  not recorded. Controllers, wheel and patch changes are recorded with duration 0.
- Every message is echoed to the output within about 5 ms (MIDI thru), the filtered ones too.
- A sysex message from the input is neither recorded nor echoed. While recording, the engine
  gives the MIDI input driver no buffers for system exclusive data (`midiInAddBuffer`), so the
  driver has nowhere to put them; only `SeqReceiveSysxBank` does [probe, relay].

Punch-in (1.01 and 2.10; suite `punch`) [probe]. The first three arguments of `SeqRecordStart`
are a target stream and a punch range, two 32-bit ticks `in` and `out`:

- Stream 0 means no punch; the range is then ignored. Any other value must be a stream: 1, or a
  pointer in those words, crashes the engine, as an invalid handle does elsewhere.
- The start call moves the target stream's events with `in` <= time < `out` out of the stream.
  They are gone at once (`SeqStreamLen` right after the start call shows it), so they do not
  play during the take, and they do not come back. Events before the start position are cut too.
  The range `0` to `ffffffff` empties the stream.
- `SeqRecordCleanup` then keeps only the take's events with `in` <= time and time + 1 < `out`,
  in a new stream as without a punch. The take is not merged into the target; that is the
  program's work.
- When the stream has no event in the range, nothing is cut and the take is kept whole, outside
  the range too. An empty range (`in` = `out`, or `out` < `in`) therefore does nothing, with one
  exception: a punch-out of 0 counts as 1, so 0 to 0 cuts the events at tick 0, and the take comes out empty.
- The engine makes a stream of its own for the cut events, so the take gets the handle after the
  next one (0x0102 instead of 0x0101). Discarding a punch take (`SeqRecordCleanup` with keep 0)
  crashed both versions under Wine; without a punch it works.
- The take's stream gets the default stream parameters, not those of the target. Thru is not
  affected: input is echoed during the punch range as before.

Cakewalk Apprentice 1.04 has no punch-in (its help lists it among the Professional features), and
the Professional demo does not record, so no program was seen using it. 3.00 has no
`SeqRecordStart`; its punch is still open (`SeqSetRecordBufferParams`).

Cakewalk Professional 2.00 under Wine stalls for 2 s after the count-in: it hides its count-in window
and Wine waits for a window manager that the virtual display does not run
(`wait_for_withdrawn_state`). The probe has no windows and no stall.

- `SeqRecordEvent(event)` (2 words) adds an event to the take while recording runs, at the
  current position: the time in the 8-byte event is ignored, status, data and duration are kept.
  It returns 1, and `SeqRecCount()` counts it [probe]. Cakewalk Professional 2.00 and the demo
  import it; no call was seen.

### Step recording

From Cakewalk Apprentice 1.04's Step Record dialog (Professional 2.00's makes the same calls)
[trace], repeated by the probe with a window of its own and scripted input (suite `steprec`)
[probe]:

- `SeqStepRecSetup(seq, hwnd)` starts it and returns a new, empty stream of the sequence, which
  collects the take. `hwnd` is the window the engine reports to.
- `SeqStepRecParams(position32, duration, velocity)` sets where the next step goes, the duration
  of its notes, and a velocity for them (0 keeps the played one). The dialog calls it after every
  step with the next position (and `SeqSetNow` to the same tick, which the engine does not need).
- A step is complete when the last key of it is up: a chord is one step. Its notes go in at the
  position, with the given duration (0 gives notes of length 0). Controllers, wheel and patch
  changes go in at the current position with duration 0 and make no step. The record filter
  (parameter 28) applies as in real-time recording; sysex is not taken.
- After each step the engine posts message 0x464 (WM_USER + 0x64) to the window, wParam 0x100,
  lParam the number of events so far. The dialog then moves its position on by the step size.
- `SeqSetRecCount(n32)` keeps the first n events of the take: the dialog's Delete removes the
  last step that way and moves the position back. Advance only moves the position (a rest).
- `SeqStepRecTerm()` ends it and returns the number of events. On Keep, the dialog makes the
  track with `SeqMergeStreams(seq, 0, seq, take, 1, 0)`: stream 0 as the first stream means none,
  so the new stream holds the take, which is destroyed. Then it sets sequence parameter 3 to 1,
  as after a real-time take.
- 3.00 differs [probe]: `SeqRecordEvent` also adds an event during step recording (at the step
  position); `SeqStepRecTerm` sets the count back to 0; `SeqGetRecordBuffer()` returns the take's
  stream.

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
- `StartProc`: the count-in callback (Recording, above), and the callback of a start that waits
  for MIDI sync or time code (External sync, above): (1, beats left, 0) counting in, (2, 0, 0)
  waiting for MIDI sync, (3, 0, 0) waiting for time code, (4, seconds × 256 + frame, 0) locking
  to it, (5, 0, 0) still waiting.

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
  - Stream 0 as `a` means no stream: the new stream holds only `b`'s events. The programs merge a
    take that way, `b` being the take and the flag 1 [trace, probe].

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
- `SeqInsertRange(dst, index32, src)` (4 words in all three versions) inserts copies of all
  events of stream `src` into stream `dst` before the index and returns 1. The events keep their
  times and `dst` is not sorted again (`SeqTemporalSort` does that); `src` is unchanged, the
  cursor of `dst` too. No program imports it. How the layout was found: every earlier attempt
  passed a number where the source stream goes. Wine's relay trace then showed the engine asking
  to grow the stream's memory by 0xA011F000 bytes whatever the arguments, which is the length the
  engine reports for a stream that does not exist (0x74023E00) times 8 bytes per event. So one
  argument had to be a stream handle; in the last word it worked. An index past the end of `dst`
  crashes [probe, relay].
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

- Sizes: all exports that are API are measured.
- The third word of `SeqMergeStreams` and the zero words of the sysex receive calls: no visible
  effect yet.
- Parameter 3 (a "Now jumped" flag): what reads it, if anything. Parameters 30 and 31 (3.00):
  not from any dialog, no effect on playback; probably recording.
- The parameters of `SeqSetRecordBufferParams` (3.00); punch-in and loop recording in 3.00
  ("Reject Loop Take", `SeqCancelLoopRecordTake`, `SeqEnumRecordStreams`). Why a punch take
  cannot be discarded in 1.01 and 2.10 (a Wine effect, or the engines).
- `SyncAllowRateTweak` in `TTSSEQ.INI`: no visible effect found.
- External sync: MIDI Time Code with an offset, other frame rates and tempo changes.
- The meaning of code 3 in the file callbacks; how 3.00 reads string events without a load
  callback.
- Wine 9.0's ALSA driver passes Song Position Pointer and other system common messages to ALSA
  in a buffer that has gone out of scope, so the fake port logs them as zeros (`sysex 00 00 00`);
  Wine's `+midi` trace has the real bytes.
