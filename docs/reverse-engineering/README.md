# How TTSSEQ was reverse engineered

This folder documents how the behaviour of Twelve Tone's sequencer engine was established: the
method, the order of the steps, what each step produced and where its results are. libseq is
written against these results and nothing else.

## The engine

`TTSSEQ.DLL` is the sequencer engine of Cakewalk for Windows: a 16-bit Windows DLL with a C
interface of 160 to 180 functions (`SeqOpen`, `SeqLoadCakewalkFile`, `SeqPlayStart`, ...). The
Cakewalk program on top of it does the user interface; the engine owns the data (sequences,
streams of events, tempo and meter maps, markers, sysex banks), the files, the timer and MIDI.
Three versions were studied:

| Version | File | From |
|---|---|---|
| 1.01 | `TTSSEQ.DLL` | Cakewalk Professional for Windows demo (1992), disk `CWDEMO1` |
| 2.10 | `TTSSEQ.DLL` | Cakewalk Apprentice for Windows 1.04 (1993) |
| 3.00 | `TTSSEQ30.DLL` | Cakewalk Professional for Windows 2.00, disk 1 |

## Ground rules

- **Observation only.** The engine was examined by looking at its files, by running the Cakewalk
  programs, and by calling the DLLs through their exported functions and watching what they do.
  Nothing was disassembled or decompiled. This is what German copyright law (§ 69d (3) UrhG, from
  Art. 5 (3) of the EU Software Directive) allows the lawful user of a program: to observe, study
  and test how it works while loading, running and storing it. The stricter route of § 69e UrhG
  (decompilation for interoperability) was considered, documented as not needed
  (`alternatives.md`), and not taken.
- **No changes to Cakewalk's files.** The programs and DLLs ran as shipped. Where the environment
  needed adjusting, it was the environment: one byte of Wine's own ALSA driver (the manufacturer
  ID its MIDI ports report), a fake ALSA sequencer library that Wine loads instead of the real
  one, Wine's 16-bit MMSYSTEM, which did not pass buffers for system exclusive input between
  16-bit programs and the driver (`tools/ttsprobe/wine/mmsystem16-midiin.patch`), the same
  module's timer, clock and MIDI devices, to run them on a virtual clock
  (`tools/ttsprobe/wine/mmsystem16-vtime.patch`), and Wine's KRNL386, to call timer and MIDI
  callbacks at interrupt time as Windows 3.1 does (`tools/ttsprobe/wine/krnl386-irq.patch`).
- **No Cakewalk files in this repository.** The DLLs and programs come from the user's own disks.
  The repository holds only what the observations produced, and songs written for the purpose.

## The steps

### 1. The installation disk (2026-10-20)

Headers, export and import tables, resources and strings of the files on the Cakewalk Apprentice
1.04 disk, read with small scripts (`tools/static/`). Result: the architecture (program on top of
an engine DLL), the engine's API by name, the program's features, and most of the `.WRK` format.
`installation-media.md`; the header and import listings are in `static/`.

### 2. Running the program under Wine (2026-10-20 to 25)

Wine still runs 16-bit Windows programs. Cakewalk Apprentice 1.04 ran unmodified under Wine 9.0
on a virtual display, driven by `xdotool`. Changing one setting at a time, saving and comparing
the bytes settled the remaining file layouts (`alternatives.md`). The same set-up then produced
the file oracles: songs written for the purpose, loaded and saved by the program as `.WRK` and
`.MID`, and edited with each command of its Edit menu.

### 3. Capturing the MIDI output in an emulator (2026-10-26)

To see what the engine sends, Cakewalk Apprentice 1.04, Cakewalk Professional 2.00 and the 1992
demo ran on Windows 3.1 in the 86Box emulator (i486, Sound Blaster AWE32 with its Windows 3.1
drivers). 86Box passed the MPU-401 port to the host, where a small CoreMIDI program wrote every
message with a timestamp. Engine test songs, each aimed at one question (message order, note-off
handling, search-back, metronome, MIDI sync, loops), were played and captured.

### 4. Tracing the programs (2026-10-26 and 27)

Wine's `+snoop` channel logs every call into a 16-bit DLL with its arguments and result. With a
fake ALSA sequencer (`tools/ttsprobe/wine/fakeseq.c`), Wine sees MIDI ports and the programs
open them; the fake writes everything sent to its output port to a log. The traces show which
engine functions the programs call, with which arguments, in which order (`traces/`).

### 5. Calling the engine directly (2026-10-27 and 28)

`ttsprobe` (`tools/ttsprobe/probe/ttsprobe.c`) is a small 16-bit Windows program that loads one of
the DLLs and calls its functions from a script. It made the engine itself the object of study:

- **Sizes.** Pascal-convention functions remove their own arguments, and Wine's tracer prints how
  many words each one removed. Calling every export once gave the argument size of nearly every
  function in all three versions (`exports/argsizes-*.txt`, `api-catalogue.md`).
- **Parameters.** Loading copies of a song with one setting changed at a time and reading every
  parameter back mapped the engine's sequence and stream parameters to the file (`params/`).
- **Playback.** Playing the engine test songs through the probe, without any Cakewalk program,
  gave the same messages in the same order as the emulator captures of step 3.
- **Callbacks and recording.** Callback thunks in the probe let the engine call back into it;
  a scripted MIDI input in the fake sequencer let it record.

The findings are collected in `ttsseq-api.md`.

### 6. Oracle sets (2026-10-29)

Scripts that call the engine systematically, run on all three versions, with their outputs kept
as the reference for libseq. `signatures.md` lists the call layouts they use.

Four functions of 1.01 had no working layout when the first sets were made: `SeqMergeStreams`,
`SeqTxMidiMsg`, `SeqSysxAsc2Bin` and `SeqReceiveSysxBank`. Their layouts came from the programs
calling them: Cakewalk Apprentice 1.04 pasting (`SeqMergeStreams`) and pressing its panic button
(`SeqTxMidiMsg`), Cakewalk Professional 2.00 receiving sysex and editing a bank's bytes in its
Sysx view (`SeqReceiveSysxBankEx`, `SeqSysxAsc2Bin`); the probe then varied each argument. The
receive at first failed with -4. Wine's relay trace of the MMSYSTEM calls showed why: Wine 9.0
rejected the engine's `midiInPrepareHeader`, because it does not pass input buffers between 16
and 32 bits. With that added to Wine (`tools/ttsprobe/wine/`), sysex input works, and the suites
`merge`, `midiout`, `sysextext` and `sysexin` were added (2026-10-29).

The rest followed on 2026-10-30 and 11-01. The meter calls and `SeqInsertRange` came from a
trace of Apprentice's Meter/Key view and from Wine's relay trace of what the engine itself asks
Windows for. `SeqIOSeek`, `SeqIORead` and `SeqIOWrite` only work while the engine has a file
open, so the probe learned to run commands inside its callbacks (suite `fileio`).
`SeqConfigMidi` is a dialog: the probe called it while `xdotool` worked the dialog, with a second
fake output port, and the results are in `devices/`. Its effect, the port assignment in
`TTSSEQ.INI`, has a suite of its own (`ports`). The dialog itself is not ported.

### 7. External sync (2026-11-01)

The fake MIDI input learned to send MIDI Clock, Start, Continue, Stop, Song Position Pointer and
MIDI Time Code. The first run on 1.01 hung: with MIDI sync, `SeqPlayStart` waits for Start in a
loop that calls nothing, and only the MIDI input callback can end it. Windows 3.1 calls that
callback at interrupt time, in the middle of the loop; Wine waits until no thread runs 16-bit
code, which never happens. Wine's call trace showed the program thread holding the Win16 lock
and the callback threads waiting for it. A change to Wine's KRNL386 (`krnl386-irq.patch`) runs
such callbacks at once, one at a time, and makes the engine's own `cli` ... `sti` keep them out.
With it, the probe showed how the engines wait for Start, follow the clock and the time code,
and what the start callback is told. 2.10 and 3.00 at first ignored the clock altogether; varying
the order of the calls showed that they take the clock source when the sequence becomes the
playing one. A trace of Cakewalk Apprentice 1.04 showed that it always sets the internal clock
after loading a file. The `sync` suite records the results.

One effect looked like a flaw of the engines: a Continue after a Song Position sometimes left
them deaf to the clock. Counting runs showed that it happened only when the program kept running
beside the callbacks, which cannot happen on Windows 3.1. Once the patch also stopped the
program's 16-bit code while a callback ran, and served the callbacks in order, 90 runs gave no
failure, and the suite plays clocks after such a Continue again.

### 8. The options in TTSSEQ.INI (2026-11-01)

Wine's relay trace of `GetPrivateProfileInt` listed the `[Options]` keys each version reads,
with their defaults and the call that reads them: eleven in 1.01, thirteen in 2.10, fourteen in
3.00, more than the strings had suggested. `tools/ttsprobe/sweep/options.py` then ran small
scripts once with the usual file and once per changed key, and compared the output, the MIDI and
the engine's own Windows calls. Most keys do what their names say; a few needed a closer look.
The sysex receive buffers showed that 1.01 and 2.10 test for universal sysex in every input
buffer rather than every message, which drops part of a long message. An invalid sync port opens
a message and the device dialog from `SeqOpen`. One key, `SyncAllowRateTweak`, showed no effect.
The suites `options1` and `options2` keep the results.

### 9. Sequence parameter 3 (2026-11-02)

Reading the parameter after one call at a time showed what sets it: every call that sets Now or
makes it jump, and nothing else. Setting it to 0 before a wide range of calls changed nothing that
could be seen, and the programs never read it. It is kept as a flag with those rules (suite
`param3`).

### 10. The leading words of SeqRecordStart (2026-11-02)

Cakewalk Apprentice 1.04 passes five zero words, so the words were first found by the probe alone
(`tools/ttsprobe/sweep/recstart.py`). Setting each word to 1 in turn changed nothing, except the
first, which crashed the engine, as an invalid handle does. A stream's handle there worked, and a
stream with notes in it showed what the words are: a target stream and a punch range of two
32-bit ticks. The engine cuts the range out of the stream when the take starts and keeps only the
part of the take inside it. Narrow ranges over one note per tick then gave the edges, and moving
events before the start showed that what decides whether a punch happens is the stream's content,
not playback. On the way, start positions that were not multiples of 30 ticks came back rounded
down to a sixteenth note, but only while the sequence sends Song Position. That also explains why
Apprentice resumed 5 to 25 ticks early in the 86Box captures. The suite `punch` records the
results. One case still crashed: discarding a punch take.

### 11. The Professional demo records (2026-11-02)

Step 10 had assumed that the Professional demo cannot record, because the first trace of it never
called `SeqRecordStart`. Its strings say otherwise: only saving is disabled, and its menus have
Record, Punch-in values, Auto-shuttle, multi-take mode and SMPTE/MTC settings. So the demo is a
second program on 1.01 to trace, and the first that uses these features
(`tools/ttsprobe/record/demo.sh`, `traces/trace-101-demo-record.txt`).

Its Punch-in dialog crashed under Wine. Wine's own trace showed why: the program swaps its dialog
procedure out and back while handling `WM_SETFONT`, and Wine 9.0 handed it back a marker instead
of the procedure. A one-line change to Wine's user32 (`user32-dlgproc.patch`) fixed it, and the
dialogs work. The demo then made the same punch calls the probe had found, with the current
track's stream as the target. It also discarded a punch take without a crash, and its kept punch
take came back as one stream holding the whole track. The difference was the last argument of
`SeqRecordCleanup`: the demo passes a pointer to a word that already holds the track's stream,
and the engine reads it. With the stream there the engine merges the take into it, or puts the
cut events back; the probe had passed 0. The suite `punch` now covers both. The demo also showed
how it records loops (the auto-shuttle parameters and an ordinary take) and that multi-take mode
is its own logic, and it never reads sequence parameter 3 either.

### 12. MIDI Time Code with offsets and other frame rates (2026-11-02)

The demo's Time format dialog showed which calls set the frame rate and the SMPTE offset.
`tools/ttsprobe/sweep/mtcexp.py` then sent quarter frames at each rate, with hours and minutes,
against sequences set up with and without an offset. Code of the wrong rate was simply ignored,
which first looked like a hang. The offset moved Now as expected; code before it was chased until
it reached it. Searching for a limit on large times found that code from about 34 minutes after
the offset is ignored too, at any rate and with any offset, close to 2048 s. Drop frame code
turned out to count by its label, and a song with tempo changes gave the tick the frame
conversion gives. All three versions agree; the suite `mtc` keeps seven cases.

The limit did not stay put. A shorter chase moved it to later code, so it applies to the time at
the end of the chase. A slower tempo moved it later and a faster one earlier, while a finer PPQ
left it where it was: it is about 4096 quarter notes. The tool then learned to send code in
stretches, to jump or to run fast. A jump stops the playback at once, run-on or not, in 1.01 and
2.10 even a jump of two frames; 3.00 tolerates exactly that one. The suite now has thirteen
cases.

### 13. Files in and out (2026-11-05)

The `files` suite loads every song of the corpus, saves it as a `.WRK` file and as a MIDI file,
reads the MIDI file back and saves that as a `.WRK` file again. Comparing these 136 files with
what libseq wrote, chunk by chunk and event by event, gave most of the rules. A few needed probes
of their own, now the suite `midifiles`.

- Keys moved past 127 by key+ landed in the top octave, but keys moved below 0 came out as small
  numbers that changed with nothing in the note. Varying the velocities of the notes around
  them showed the link: the key is the next event's second data byte modulo 12.
- Some program changes read from a MIDI file had a second data byte and a duration, which a
  program change does not have. Most matched the message read before them. One did not: it
  matched the last event of the file the engine had written just before. A probe that wrote
  different files before reading the same one confirmed it. The writer and the reader share one
  event.
- The metronome note of the read back files was that of the song loaded before, not the default.
  Probes with several sequences showed that parameters 17 to 21 are one setting for all of
  them, and that a `.WRK` file sets it only when its sequence becomes the current one.
- SYN_MAPS's cue point came back at tick 2240 instead of 960. The tempo map read from the file
  keeps the slip of `SeqInsTempo` (found in step 2 of the library), and the frame taken at 960
  goes back to a tick through those cached times.

With these, libseq writes and reads all of them byte for byte.

### 14. String events and the file callbacks (2026-11-05)

Event kinds let a program keep its own events in the engine's streams: texts, MCI commands,
waves. The `fileio` suite already showed the load callback and an event procedure reading its
text. Libseq needed the rest, so the probe learned two commands, `dumpcb` and `fillcb`, to read and
change the bytes an event procedure gets a pointer to. That showed the pointer leads into the
stream itself, and that bytes the procedure writes there stay with the event.

Saving showed the other messages: 5 for a `.WRK` file, 7 twice for a MIDI file (asking, then
writing). Destroying the sequences was the surprise. The free message reached only some events.
First it looked like kind 1 alone, then like kind 1's name. Varying one thing at a time pointed
to the value the kind was registered with, which the procedures never see. A failed load frees by
yet another rule. Both rules now fit every case probed, which is all that can be said without
looking inside the DLL. The suite `eventkinds` keeps these cases on all three versions.

### 15. Playback (2026-11-06)

The suites had recorded what 1.01 sends to the fake MIDI port all along; the runner had ignored
it. It now keeps what libseq sends, on a clock of its own that only `sleep` and `idle` move, and
compares: the same messages in the same order, at times close to the DLL's. A few captures were
older than their suites, and recording them again fixed them.

Before any playback this already showed more than expected: the engine sends a stream's patch,
volume and pan whenever they are set, also while it loads a file, but only for the current
sequence. Marker messages sent between the calls of a probe showed which call sent what.

The player came over from libseq 0.8.1, whose rules had matched the probe before. 22 of the 25
songs played right at once. The rest were a muted stream that was not skipped, one stall of
Wine's timer in a capture (gone when recorded again), and an endless loop whose cut-off fell on a
note, now moved between two notes.

Playing string events settled the question chapter 14 left open. Only some events got the play
message, and they were the ones that did not get the free message. Registering kinds with the
values 0 to 4 showed why: the registration value is a set of flags, bit 0 for no play message,
bit 1 for no free message. The values tried before (0x11, 0x22, ...) had only made it look like
a rule about kind 1.

### 16. Input, recording and sync (2026-11-06)

The suites for recording and sync had been recorded weeks before, with scripted input on the
fake port. To use them the runner had to play that input to libseq at the same moments, counted
from `SeqOpen` as the fake port counts them, and let time pass inside the calls that wait: the
count-in, the waits for MIDI sync and time code, a sysex receive. The engine now calls
`Platform::wait` on every turn of such a loop, and the runner moves its clock on by a
millisecond there.

No suite covered the thru, so a new one does (`thru`). Its first runs seemed to show note-offs of
one key vanishing; the filter that hid the sustain-offs of `SeqClose` from the listing hid them
too. With that fixed the rules were plain. The engine keeps a copy of the thru settings, which only
`SeqSetPlayingSeq` and a parameter set on the current sequence change, and one table of the notes
on for the thru, `SeqTxMidiMsg` and the playback: a note `SeqTxMidiMsg` started ends when the
playback stops. The same runs showed that the playing sequence is the current sequence of chapter
15.

Recording passed after one surprise. The DLL's first count-in click came late whenever the start
did not sit on a beat: the count-in clicks on the beats of the start's measure counted back,
not on beats counted from the start. The ticks recorded after a count-in depend on when Wine
delivers the input, so the runner compares them with a slack of 8 ticks.

MIDI clock followed the documented rules. Time code needed two finds. Now at the lock is always a
multiple of 30 ticks at 120 ppq, a sixteenth note: with Song Position on, the start rounds down
to a MIDI beat here too, which explains values that had looked like 0.25 s past the chase. And
the DLL, like libseq, locked once to the tail of the previous case's code before its own came:
code that jumps during the chase is locked to again, which the MIDI capture shows as a second
lock output. A repeated quarter frame 0 at the join of two pieces of code is what makes a jump of
two frames visible.

### 17. Virtual time (2026-11-07)

Until now every time in the suites was compared with a margin: 40 ms for playback, 8 ticks for
recording. Under Wine the DLL's timer and the MIDI input ran on the host's clock and came a few
milliseconds early or late, differently on every run, so the DLL's own timing could not be told
from Wine's. Wine's 16-bit MMSYSTEM got a virtual clock (`mmsystem16-vtime.patch`): the DLL's
timer, `timeGetTime` and the MIDI devices run on milliseconds that pass only where the probe
waits, and a call takes no time. Every run now gives the same output to the millisecond, and an
experiment can put a call or an input at any millisecond: one input per millisecond over half a
second, a start at every phase of the timer, code that breaks off after each piece of a group.

That turned timing into a question with exact answers. A small simulator was written for each
part of the engine and tried against the logs until every case agreed, and then the rules went
into libseq, which reproduces the logs itself. The rules are a reconstruction of the DLL's
algorithm: each reproduces the DLL in every case tried, and each is kept only as far as the
experiments force it. The engine counts ticks with a remainder from one tick before its timer starts; a count-in waits
for the next MIDI clock, clicks on Now's beats and ends with the clock back at -1; time code is
decoded in groups of two frames, a group that starts inside another moves to the nearest group
start, and the chase ends 240 quarter frames after the second label. `ttsseq-api.md`, Timing,
lists every rule with the experiments behind it and the alternatives they ruled out.

The exact comparison also corrected older readings. `SeqClose` sends nothing itself: the All
Sound Off and sustain off seen at every close were Wine's driver resetting the port. The thru's
channel 16 turns a pitch wheel into an F0 status that the DLL does send, and Wine's driver
dropped. Some of the experiments became suites, `mtcbreaks` and `countins`, so the rules are
checked with the others. One suite stays on the host's clock: in virtual time the DLL's sysex
receive hangs after a `midiInReset`, so `sysexin` is compared without its times.

## In this folder

| File | Content |
|---|---|
| `installation-media.md` | Step 1: the files on the installation disk, the API by group, the `.WRK` format |
| `alternatives.md` | Step 2: the questions left after step 1 and how running the program answered them |
| `ttsseq-api.md` | Everything known about the engine's interface and behaviour, with the source of each statement |
| `api-catalogue.md` | Every export of the three versions with its ordinal and argument size |
| `signatures.md` | Call layouts and result types, as the oracle scripts use them |
| `exports/` | The export tables, compared side by side, and the measured argument sizes |
| `traces/` | Summaries of the program traces (which functions, how often, sample arguments) |
| `params/` | Parameter read-backs per version: which file byte sets which engine parameter |
| `static/` | Step 1: the NE headers and the resolved imports of the files on the disk |
| `devices/` | `SeqConfigMidi`'s dialog per version, and what each way of working it changed |
| `first-calls/` | The first probe script and its output on each version |
