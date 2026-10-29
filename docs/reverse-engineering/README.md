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
  ID its MIDI ports report) and a fake ALSA sequencer library that Wine loads instead of the real
  one.
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
| `first-calls/` | The first probe script and its output on each version |
