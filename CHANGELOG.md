# Changelog

Each release implements one version of Twelve Tone's sequencer engine (`README.md`). Fixes after
a release raise the patch level.

## On the way to 2.10.0: TTSSEQ.DLL 2.10

The engine of Cakewalk Apprentice for Windows 1.04. Each entry is one step from 1.01 to 2.10.

- **Lifecycle.** `SeqGetVersion` is 0x020A. `SeqWire` comes before `SeqOpen`: without it the
  engine sees no MIDI devices, and `SeqOpen` returns 3. `SeqOpen` returns 0 when it works. New:
  `SeqSleep` closes the ports and stops the timer without giving up anything else, and `SeqWake`
  opens them again. A playback started while asleep begins at the wake, and its clock runs at 100
  bpm until the next tempo change.

Found on the way, true of 1.01 too and not in release 1.1.0:

- `SeqOpen` and `SeqClose` count. The ports open when the count becomes 1 and close when it
  becomes 0; the count also goes below 0. Every `SeqClose` destroys the sequences.
- While the ports are closed nothing goes out. `SeqTxMidiMsg` still returns 1 and remembers its
  notes as on, also across `SeqClose`.

## 1.1.0 (2026-11-10): TTSSEQ.DLL 1.01

The engine of the Cakewalk Professional for Windows demo (1992), rebuilt from observation alone.

### What it does

- **Data**: sequences and streams with the engine's handles and parameters, the 8-byte events,
  the event cursor and its edit primitives, merging, ranges and sorting, markers (locked ones
  follow tempo and frame rate), sysex banks in binary and as text, and string events with their
  kinds and event procedures.
- **Time**: the time base, tempo and meter maps with copy and paste, positions as measure, beat
  and tick, SMPTE time code at 24, 25, 29 (drop frame) and 30 frames, and tempo ratios.
- **Files**: Cakewalk `.WRK` files read and written as 1.01 writes them, with the program's file
  callbacks and the `SeqIO` functions inside them; Standard MIDI Files read and written byte for
  byte as 1.01 does; sysex bank files.
- **Playback**: the playing sequence with its thru settings, chase, loops, the stop output, Song
  Position, and `SeqTxMidiMsg`.
- **Input**: MIDI thru, real-time recording with count-in and punch, step recording, sysex
  receive and send, and following MIDI clock and MIDI Time Code.
- **Devices and options**: what `TTSSEQ.INI` holds, the output devices and `[Options]`, comes
  from the host through a platform interface, together with the MIDI ports, the clock and the
  timer.

### Interfaces

- **C**, `include/libseq/ttsseq.h`: 1.01's own interface, one function per export, with the
  DLL's names, argument order and meanings, and one engine per process. Functions named
  `LibSeq*` stand in for what Windows did.
- **C++**, `include/libseq/engine.hpp`: `libseq::Engine` and the objects it holds, with callbacks
  as `std::function` and the host as a `libseq::Platform`.
- **Swift**, `Package.swift`: the module `LibSeq` (`SeqEngine`, `SeqSequence`, `SeqStream`,
  `SeqEvent`, ...) over the C++ interface, through Swift's C++ interoperability.

`DESIGN.md` describes all three and maps the 163 exports of 1.01 to them.

### How it is known

Nothing was disassembled or decompiled. The engine was observed through its installation disks,
through the Cakewalk programs running under Wine and in an emulator, and through calls into the
original DLL from scripts (`docs/reverse-engineering/`). Those calls produced 63 oracle suites:
what 1.01 returns for each script and the MIDI it sends, message by message. Wine ran them in
virtual time, so the MIDI comes with the millisecond it left the engine. libseq passes all 63
suites, through its C interface. A C program, a C++ test and three Swift tests check the other
interfaces.

### Where libseq differs from the DLL

- A handle the engine does not hold makes the DLL crash or answer garbage; libseq answers 0 or
  does nothing. The same goes for `SeqGetSysxAscLen` with 0 bytes per line, where the DLL divides
  by zero.
- Some results carry what the DLL left in a register: the tick's high word in DX for
  `SeqGetTempo` and `SeqMeterIxByTick`, the velocity in AX for `SeqStepRecParams`. They are not
  part of the interface; the script runner adds them for the suites that read them.
- The device dialog `SeqConfigMidi` is not part of libseq. The host lists its devices and options
  through the platform interface.
- `SeqRegisterEventKind` keeps a copy of the name, where the DLL keeps the program's pointer.
- Arguments with no effect found are accepted and ignored. They are named `reserved` in
  `ttsseq.h`.

### Requirements

A C++20 compiler and CMake 3.20 or later; tested with GCC and Clang on Linux and with Apple
clang on macOS. The Swift package needs Swift 5.9 and macOS 14 or iOS 17; its tests need Swift
6.
