# Step 2: running the program instead of reading its code

Written on 2026-10-20, when the open questions of step 1 had to be answered: either by
decompiling parts of the program, which the law allows only when the information needed for
interoperability cannot be obtained otherwise, or by observing the running program. This log
records which route was tried for each question and what it gave. Its conclusion still holds:
observation was enough, and nothing was decompiled.

Environment: Wine 9.0 (32-bit, Ubuntu 24.04 x86-64), Xvfb at 1024 × 768, xdotool for input.

## Route 1: static inspection (step 1)

Header parsing, export and import tables, resource tables, text strings, data layouts, and a
comparison with the GPL `QWrk` parser. It gave the compiler, the libraries, the architecture, the
API surface, the feature inventory and most of the `.WRK` format (`installation-media.md`).

Open after it: chunks 17 and 25, the exact TIMEFMT and THRU layouts, how Standard MIDI Files are
written and read, how special events are encoded, `PATCHES.BIN`, and the engine's timing.

## Route 2: running the program

The assumption that the program could not be run was wrong: Wine on x86 Linux still runs Win16
programs. `WINEARCH=win32 wineboot`, the decompressed files in `C:\WCWLITE`, `PAT.INI` copied to
`PATCHES.INI` (as the installer does), then `wine WCWLITE.EXE` under Xvfb. The program started
without any adjustment, loaded the sample songs, showed all nine views, saved `.WRK` and `.MID`
files, imported `.MID` files and ran the transport on its internal clock.

Each answer below came from changing one setting in the program, saving, and comparing the bytes
with the save before:

| Question | Method | Result |
|---|---|---|
| Chunk 17 | Settings > Metronome, every field | the metronome note: `u16 port, channel, key, velocity, duration, 0, 0`. "Use PC speaker" is not in the file. |
| VARS 31, 33, 34 | the same | metronome on playback, accent, count-in measures (32: metronome on recording) |
| TIMEFMT | Settings > Time Format, 25 fps, offset 01:02:03:00 | `u16 fps, u32 offset in frames` (93075 = 3723 s × 25) |
| VARS 15, 17 to 20, 77 | Settings > MIDI Out | 15 SPP locate delay, 17 zero continuous controllers, 18 send SPP, 19 Continue allowed, 20 search-back, 77 transmit MIDI sync (254 off, 255 on) |
| THRU chunk | Settings > MIDI Thru | byte 0 mode (1 manual, 2 auto), 2 port, 3 channel, 4 key+, 5 vel+; 6 and 7 stay 255 |
| MARKERS chunk | Markers view | `u8 smpte_lock, u8 0, u24 ticks, 5 × 0, u8 len, name`; a SMPTE-locked marker stores ticks computed through the tempo map; sorted by time |
| Track parameters | Track menu | TRACK flags bit 1 mute, bit 2 loop; TRKREPS, TRKOFFS, TRKVOL (volume and pan), TRKPATCH; written only when set |
| Special events | Event list, Insert | a stream with any string event is written as chunk 18; STRTAB lists the kinds |
| TEMPO and NTEMPO | open a file with the old chunk, save | NTEMPO is bpm × 100, TEMPO bpm × 1; MEMRGN is dropped |
| SMF export | Save As MIDI File | format 1, division = ppq; track 0 has meter, key, tempo, markers (Marker meta), SMPTE-locked markers (Cue meta); each non-empty stream: port meta, name, program change from the patch, CC 7 from the volume, CC 10 from the pan, text events as Text meta; note-offs as note-ons with velocity 0. Key+ is applied to the notes; loop, Time+ and MCI events are not exported; muted streams are, empty ones are not |
| SMF import | open the exported file, save as `.WRK` | the reverse: Cue meta to a locked marker, program change, CC 7 and CC 10 at time 0 to stream parameters, Text meta to text events, port meta to the port; the time format resets to 30 fps |
| Chunk 25 | in two sample songs; not written by this edition | `u16 16`, then 16 × `{7, 63, i, 0}`: most likely the Faders view of Cakewalk Professional. Ignored. |
| `PATCHES.BIN` | written by the program | a cache of `PATCHES.INI`, not an exchange format |

Without MIDI devices in the container, MIDI output, recording and sysex transfer could not be
observed yet. They became observable later: in an emulator with real Windows 3.1 (step 3) and
under Wine with a fake MIDI driver (steps 4 and 5).

## Conclusion of 2026-10-20

The information needed to read and write the program's files was obtained by inspection and
observation. Decompilation was not needed and was not started. The engine's internal timing was
the one area observation could not reach then; it was later observed through the engine's MIDI
output (steps 3 to 5), again without decompilation.
