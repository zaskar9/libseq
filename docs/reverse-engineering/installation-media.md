# Step 1: the installation disk of Cakewalk Apprentice 1.04

The first look at the engine: the files of the one installation disk of Cakewalk Apprentice for
Windows 1.04, examined without running them (October 2026). Everything here comes from file
headers, resource tables, exported and imported symbol names, text strings and data layouts. No
code was disassembled. The `.WRK` layouts were checked against the GPL `QWrk` parser of the
drumstick library, used only as a reference.

## Summary

- **Product**: Cakewalk Apprentice for Windows 1.04, "for Creative Labs" (the Sound Blaster
  bundle). Vendor: Twelve Tone Systems, Inc. Author credited in every binary: Greg Hendershott
  (credits string: "Engineers: Greg Hendershott, Phil Sours.").
- **Platform**: 16-bit Windows 3.1 (NE executables, expected Windows version 3.10), Standard or
  Enhanced mode.
- **Language**: C. No C++ or MFC traces; the window procedures are plain C functions exported by
  name (`TRKWNDPROC`, `STAFFWNDPROC`, ...), and the program contains the source-file names of its
  `assert()` calls (`clip.c cv.c edit1.c ev.c evtfilt.c file.c gendlg.c input.c meter.c rt.c`).
- **Compiler and linker**: Microsoft C (run-time error strings `R6000`...`R6018`, the import of
  `__fpMath` from `WIN87EM`). The NE headers record LINK 5.10 for the program and LINK 5.50
  (Visual C++ 1.0) for `TTSSEQ.DLL` 2.10 (August 1993): the engine was rebuilt with a newer
  toolchain than the program.
- **Libraries**: the Windows 3.1 API (KERNEL, USER, GDI, KEYBOARD, COMMDLG, SHELL) and the
  Multimedia Extensions (MMSYSTEM). No third-party library.
- **Architecture**: a program (`WCWLITE.EXE`) on top of a separate sequencer-engine DLL
  (`TTSSEQ.DLL`, "Twelve Tone Systems Sequencer Engine") with a C API of 168 exported functions,
  named `Seq...`. The program calls 135 of them. The Staff view's glyphs are in four small
  resource DLLs, one per display adapter (`BITSCGA/EGA/HERC/VGA.DLL`).
- **Installer**: Microsoft Setup Toolkit (script `WINCAKE.MST`).

`TTSSEQ.DLL` contains the string "Reverse engineering of this software is prohibited by copyright
law." The work described in this folder stays within what the law allows the lawful user of a
program: observing, studying and testing how it works while running it (see `README.md`).

## Compression

Two formats of Microsoft's `COMPRESS.EXE` are on the disk: `KWAJ` (LZ77 and Huffman; the 1992
files) and `SZDD` (LZSS; `TTSSEQ.DL_`, `WCWLITE.HL_`, `READLITE.WR_`, `SETUP.IN_`, the four files
refreshed for the November 1993 release). Microsoft's `EXPAND.EXE` reads only `SZDD`; `msexpand`
built from libmspack reads both. The real file names come from `SETUP.LST` and `SETUP.INF`.

## The files

| File | Purpose |
|---|---|
| `WCWLITE.EXE` | The program: NE, 38 code segments and one data segment, an MDI frame application. Version 1.04, "Cakewalk Apprentice for Windows". Built from the same code base as Cakewalk Professional (`WINCAKE.EXE`) and the demo (`WCWDEMO.EXE`). |
| `TTSSEQ.DLL` | The engine, version 2.10 (1993-08-06): MIDI input and output, the multimedia timer, event streams, tempo and meter maps, SMPTE and MTC arithmetic, sysex banks, `.WRK` and `.MID` loading and saving. Its version resource names "Cakewalk Professional for Windows". It reads `TTSSEQ.INI`. |
| `BITS*.DLL` | Bitmaps for the Staff view: note heads, accidentals, rests, stems, in four sizes. |
| `WCWLITE.HLP` | WinHelp 3.1 file, 117 topics. |
| `PAT.INI` | Patch-name lists (General MIDI, MT-32, Roland GS and others), copied to `PATCHES.INI` on installation and compiled into the cache `PATCHES.BIN` by the program. |
| `READLITE.WRI` | The README (Microsoft Write). |
| `*.WRK` | Five sample songs in `.WRK` format 2.0. They are not used in this repository. |
| `SETUP.*`, `_MSTEST.EXE`, `MS*.DLL`, `*.INC` | The Microsoft Setup Toolkit and its script. |

## What the program imports

- MMSYSTEM: only `sndPlaySound`, `mciSendString`, `mciGetErrorString`. The program never touches
  MIDI itself; it does so only through the engine. MCI strings implement the "MCI command" event
  kind, `sndPlaySound` the "Wave" event kind.
- USER and GDI: a classic Win16 MDI application; COMMDLG for file dialogs; SHELL for drag and drop.
- `TTSSEQ`: 135 of the engine's 168 functions.

Window procedures registered with the engine as event-kind handlers through
`SeqRegisterEventKind`: `TEXTEVTPROC`, `MCIEVTPROC`, `WAVEEVTPROC`, the three non-MIDI event
kinds (the `.WRK` string table lists exactly "MCIcmd", "Wave", "Text").

## What the engine imports

Exactly `midiOut{GetNumDevs, GetDevCaps, GetErrorText, Open, Close, PrepareHeader,
UnprepareHeader, ShortMsg, LongMsg, Reset}`, `midiIn{GetNumDevs, GetDevCaps, GetErrorText, Open,
Close, PrepareHeader, UnprepareHeader, AddBuffer, Start, Reset}` and `time{GetTime, SetEvent,
KillEvent, GetDevCaps, BeginPeriod, EndPeriod}`. That is its whole hardware interface: a periodic
multimedia timer drives playback, `midiOutShortMsg` and `midiOutLongMsg` send, `midiIn` buffers
receive. `TIMERTRAP` (the timer callback) and `RXTRAP` (the MIDI input callback) are exported.

## The engine's API by group

- Lifecycle: `SeqOpen/Close/Idle/GetVersion`, `SeqCreateSeq/DestroySeq`, `SeqSet/GetSeqParam`,
  `SeqSleep/Wake/Wire`.
- Streams: `SeqCreateStream/DestroyStream/EnumStreams/CloneStream/AdoptStream/MergeStreams`,
  `SeqSet/GetStreamParam`, `SeqSet/GetStreamName`.
- Event cursor: `SeqTell/Eos/Seek/NxtEvt/GetEvt/GetEvtCopy/PutEvt/InsEvt/DelEvt/MoveEvt`,
  `SeqEvtAtTimeGE/GT`, `SeqInsertRange/DeleteRange/DisplaceEarlier/DisplaceLater/TemporalSort`.
- Event kinds of the program: `SeqRegisterEventKind/UnregisterEventKind/CallEventProc`.
- Time: `SeqGet/SetPPQ`, `SeqRescaleTime`, the tempo map, the meter map, measure:beat:tick and
  SMPTE conversions, tempo ratios.
- Transport: `SeqSetPlayingSeq/StandbySeq`, `SeqSet/GetSrcClock`, `SeqPlayStart/Stop`,
  `SeqRecordStart/Stop/Cleanup`, `SeqRewind`, `SeqSet/GetNow`, `SeqGetEnd`, `SeqSet/GetDur`,
  `SeqExpired`.
- Step record and input: `SeqStepRecSetup/Params/Term`, `SeqRecordEvent`, `SeqRecCount`,
  `SeqTxMidiMsg`, `SeqSetMidiMacro`.
- Sysex banks: `SeqCreate/Destroy/EmptySysxBank`, `SeqSysxBin2Asc/Asc2Bin`,
  `SeqSave/Load/Send/ReceiveSysxBank`.
- Files: `SeqLoad/SaveCakewalkFile`, `SeqLoad/SaveMidiFile`, `SeqIOSeek/Read/Write`,
  `SeqGetFileError`.
- Devices: `SeqConfigMidi` (the "Select MIDI Devices" dialog), `SeqGetPortName`.
- Markers: `SeqNumMarkers`, `SeqMarkerForTime`, `SeqIns/DelMarker`, `SeqSet/GetMarkerName/Time`,
  `SeqSortMarkers`.

## Features named by the program's menus, dialogs and strings

Menus: File (New, Open, Save, Save As, Merge, Extract), Realtime (Play, Record, Rewind, Step
Record), Mark, GoTo, Edit (Undo, Copy, Cut, Paste, Paste to One Track, Quantize, Length, Slide,
Transpose, Controller Fill), Track (Name, Status, Archive, Loop, Key+, Vel+, Time+, Port,
Channel, Patch, Volume, Pan, Solo, Clone, Kill, Wipe), Window, Settings (Metronome, MIDI Devices,
Patch Names, MIDI Thru, MIDI Out, Time Format), Help.

The dialogs show the data model: event kinds Note, Key Aftertouch, Controller, Patch, Channel
Aftertouch, Pitch Wheel, Sysex, Text, MCI Command, Wave; an event filter with minimum and maximum
per field; quantize with resolution and strength; step recording with step size and duration;
punch-in, auto-shuttle, MIDI sync out (Start, Continue, Stop, Clock, SPP), SMPTE and MTC at 24,
25, 30 drop and 30 non-drop frames with an offset; the metronome on the PC speaker or as a MIDI
note; a channel-to-track table for recording; a sysex bank editor. Timebases offered: 48, 72, 96,
120, 144, 168, 192, 216, 240, 360, 384 and 480 ticks per quarter note.

## The `.WRK` format 2.0

Header: `"CAKEWALK"`, `0x1A`, minor and major version bytes (0, 2). Then chunks (`type:u8`,
`length:u32`, data) up to the type `0xFF`. All integers little-endian. The layouts below marked
(obs) were confirmed by changing a setting in the running program, saving and comparing bytes
(step 2, `alternatives.md`); later observations are in `ttsseq-api.md`.

Write order: TIMEBASE, VARS, STRTAB, NTEMPO, METER, METERKEY, TIMEFMT, METRONOME (17), THRU (16),
MARKERS, then per stream TRACK, TRKNAME, STREAM or STRSTREAM (18), TRKPATCH, TRKREPS, TRKOFFS,
TRKVOL; then COMMENTS and END.

| id | name | layout |
|---|---|---|
| 10 | TIMEBASE | `ppq:u16` |
| 3 | VARS | 89 bytes: 0 Now `u32`, 4 From `u32`, 8 Thru `u32`, 12 key signature, 13 clock source, 14 auto-save minutes, 15 SPP locate delay, 17 zero continuous controllers, 18 send SPP, 19 Continue allowed, 20 patch/wheel/controller search-back, 21 auto-stop and 22 stop time `u32`, 26 auto-rewind and 27 rewind time `u32`, 31 metronome on playback, 32 on recording, 33 accent, 34 count-in measures, 37 thru on, 57 auto-restart, 58 to 61 tempo ratios, 64 punch and 65/69 punch in/out `u32`, 73 end-all time `u32`, 77 transmit MIDI sync (254 off, 255 on) |
| 22 | STRTAB | `count:u16`, then `{len:u8, name, code:u8}`: "MCICmd" 1, "Wave" 2, "Text" 3 |
| 4 / 15 | TEMPO / NTEMPO | `count:u16`, 18-byte records: `time:u32`, 4 bytes, `tempo:u16`, 8 bytes. NTEMPO is bpm × 100, TEMPO bpm × 1 (rewritten as NTEMPO on save; obs) |
| 5 | METER | `count:u16`, 12-byte records: 4 bytes, `measure:u16`, `num:u8`, `den_log2:u8`, 4 bytes |
| 23 | METERKEY | `count:u16`, 5-byte records: `measure:u16`, `num`, `den_log2`, `key:s8` |
| 11 | TIMEFMT | `fps:u16`, `offset:u32` in frames (obs) |
| 17 | METRONOME | `port, channel, key, velocity, duration, 0, 0`, each `u16` (obs) |
| 16 | THRU | `mode:u8` (1 manual, 2 auto), 0, `port:u8`, `channel:s8`, `key+:s8`, `vel+:s8`, 255, 255 (obs) |
| 21 | MARKERS | `count:u32`, records `smpte_lock:u8, 0, ticks:u24, 5 × 0, len:u8, name`, sorted by time (obs) |
| 1 | TRACK | `track:u16`, two Pascal strings, `channel:u8` (255 none), `key+:s8`, `vel+:s8`, `port:u8`, `flags:u8` (bit 1 mute, bit 2 loop; obs) |
| 24 | TRKNAME | `track:u16`, Pascal string |
| 2 | STREAM | `track:u16`, `count:u16`, 8-byte events `time:u24, status:u8, d1:u8, d2:u8, dur:u16` |
| 18 | STRSTREAM | `track:u16`, `count:u32`, variable events; used when a stream holds a string event (obs) |
| 14 | TRKPATCH | `track:u16`, `patch:u16` (obs) |
| 12 | TRKREPS | `track:u16`, `reps:u16` (obs) |
| 9 | TRKOFFS | `track:u16`, `ticks:s16` (obs) |
| 19 | TRKVOL | `track:u16`, `volume:s16`, `pan:s16` (obs) |
| 8 | COMMENTS | `len:u16`, text |
| 6 | SYSEX | `bank:u8, len:u16, autosend:u8, namelen:u8, name, bytes` |
| 7 | MEMRGN | ten 42-byte records (memory regions of Cakewalk for DOS); dropped on save (obs) |
| 25 | (Professional) | `u16 16`, then 16 × `{7, 63, i, 0}`; ignored and dropped by Cakewalk Apprentice 1.04 (obs) |

Chunks found later: 26 (baggage) and 28 (two parameters) in engine 3.00, and NTRKOFS (27), the
32-bit form of TRKOFFS; see `ttsseq-api.md`.
