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
  16-bit programs and the driver (`tools/ttsprobe/wine/mmsystem16-midiin.patch`), and Wine's
  KRNL386, to call timer and MIDI callbacks at interrupt time as Windows 3.1 does
  (`tools/ttsprobe/wine/krnl386-irq.patch`).
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

Cakewalk Apprentice 1.04 passes five zero words, and the Professional demo never records, so
the words were found by the probe alone (`tools/ttsprobe/sweep/recstart.py`). Setting each word
to 1 in turn changed nothing, except the first, which crashed the engine, as an invalid handle
does. A stream's handle there worked, and a stream with notes in it showed what the words are: a
target stream and a punch range of two 32-bit ticks. The engine cuts the range out of the stream
when the take starts and keeps only the part of the take inside it. Narrow ranges over one note
per tick then gave the edges, and moving events before the start showed that what decides
whether a punch happens is the stream's content, not playback. On the way, start positions that
were not multiples of 30 ticks came back rounded down to a sixteenth note, but only while the
sequence sends Song Position. That also explains why Apprentice resumed 5 to 25 ticks early in
the 86Box captures. The suite `punch` records the results.

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
