# libseq

A C++20 library that does what `TTSSEQ.DLL` did: the sequencer engine of Cakewalk for Windows,
written by Twelve Tone Systems in the early 1990s. It reads and writes Cakewalk `.WRK` files and
Standard MIDI Files, holds sequences in the engine's own terms (sequences, streams, events, tempo
and meter maps, markers, sysex banks), converts between ticks, measures and SMPTE time, and plays
and records through an interface the application provides. MIT licensed, no dependencies beyond
the C++ standard library.

The name drops the company from the engine's: the DLL describes itself as "Twelve Tone Systems
Seqencer Engine" (sic), so `TTSSEQ` is TTS plus SEQ.

## One engine version per release

The library is rebuilt in the order Twelve Tone built the engine. Each release implements exactly
one engine version, and each later release starts from the one before:

| Release | Engine | Shipped with |
|---|---|---|
| 1.1.0 | `TTSSEQ.DLL` 1.01 | Cakewalk Professional for Windows demo (1992) |
| 2.10.0 | `TTSSEQ.DLL` 2.10 | Cakewalk Apprentice for Windows 1.04 (1993) |
| 3.0.0 | `TTSSEQ30.DLL` 3.00 | Cakewalk Professional for Windows 2.00 |

To use a version, check out its tag. The history between the tags shows, commit by commit, how
the engine changed from one version to the next. Fixes after a release raise the patch level.

## How the engine is known

Only by observation: the files on the installation disks, the programs running under Wine and in
an emulator, and calls into the original DLLs through their exported interface. Nothing was
disassembled or decompiled. The method, the tools and every observation they produced are part of
this repository, so each statement about the engine can be traced to the observation behind it.

## Repository

| Folder | Content |
|---|---|
| `include/libseq/`, `src/` | The library: `ttsseq.h`, the DLL's interface in C, over `engine.hpp`, the C++ interface (`DESIGN.md`) |
| `tests/runner/` | `ttsrun`, which runs the probe's scripts against the library's C interface, and the oracle tests built on it |
| `tests/c/` | A C program on the C interface |
| `docs/reverse-engineering/` | How the engine was studied, step by step, and everything known about its interface |
| `tools/static/` | The scripts that read the files on the installation disk |
| `tools/ttsprobe/` | The tools that observed it: a probe that calls the DLLs from scripts, a fake MIDI driver for Wine, analysis scripts |
| `tests/data/songs/` | The songs every observation starts from |
| `tests/data/cakewalk/` | What the Cakewalk programs made of them: saved files, edits, captured MIDI output |
| `tests/data/ttsseq/` | What the engine DLLs answer when called directly, version by version |

## Building and testing

    cmake -S . -B build && cmake --build build && ctest --test-dir build

The tests are the oracle suites of `tests/data/ttsseq/101`: `ttsrun` runs each suite's script against
the library's C interface and compares what it prints, line by line, with what `TTSSEQ.DLL` 1.01 printed for the
same script, and the MIDI it sends, message by message at the same millisecond. `tests/runner/suites101.txt`
lists the suites the library passes so far.
`build/tests/runner/ttsrun --songs tests/data/songs SCRIPT [EXPECTED]` runs one script by hand.

## Status

Release 1.1.0 (TTSSEQ 1.01) is being built one layer at a time, each with the oracle suites it
makes pass. Done: sequences and streams with the engine's handles and parameters, reading `.WRK`
files into them, the time base (tempo and meter maps with their copy and paste, positions and
timecode), the event cursor with the edit primitives, markers and sysex banks, saving `.WRK`
files and reading and writing MIDI files byte for byte as 1.01 does, string events with their
kinds and the program's file callbacks, playback through a platform interface for the MIDI
ports and the clock, and the input side: MIDI thru, real-time recording with count-in and punch,
step recording, sysex receive and send, `SeqTxMidiMsg`, and following MIDI clock and MIDI Time
Code, and the devices and options that `TTSSEQ.INI` holds, through the platform interface. Their
timing reproduces 1.01 to the millisecond, measured with Wine in virtual time
(`docs/reverse-engineering/ttsseq-api.md`, Timing). All of it is reachable through 1.01's own
interface in C, one function per export (`include/libseq/ttsseq.h`, `DESIGN.md`). An earlier exploration
(versions 0.1.0 to 0.8.1) is kept on the branch `archive/0.x`.
