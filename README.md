# libseq

A C++20 library that does what `TTSSEQ.DLL` did: the sequencer engine of Cakewalk for Windows,
written by Twelve Tone Systems in the early 1990s. It reads and writes Cakewalk `.WRK` files and
Standard MIDI Files, holds sequences in the engine's own terms (sequences, streams, events, tempo
and meter maps, markers, sysex banks), converts between ticks, measures and SMPTE time, and plays
and records through an interface the application provides. MIT licensed, no dependencies beyond
the C++ standard library.

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
| `docs/reverse-engineering/` | How the engine was studied, step by step, and everything known about its interface |
| `tests/data/songs/` | The songs every observation starts from |
| `tests/data/cakewalk/` | What the Cakewalk programs made of them: saved files, edits, captured MIDI output |

## Status

Nothing of the library itself exists yet on this branch. An earlier exploration (versions 0.1.0
to 0.8.1) is kept on the branch `archive/0.x`.
