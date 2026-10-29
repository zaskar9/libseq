# Observations of the Cakewalk programs

What the Cakewalk programs themselves produced from the test songs (`../songs/`): the files they
saved and the MIDI they sent. These are the oracles from before the engine was called directly
(steps 2 and 3 in `docs/reverse-engineering/README.md`).

| Folder | Produced by | Shows |
|---|---|---|
| `app/NAME.WRK` | Cakewalk Apprentice 1.04, File > Save As (Cakewalk) after loading `songs/corpus/NAME.WRK` | the `.WRK` format as written |
| `app/NAME.MID` | Cakewalk Apprentice 1.04, File > Save As (MIDI File) of the same song | the mapping to Standard MIDI Files |
| `mid2wrk/NAME.WRK` | Cakewalk Apprentice 1.04, loading `app/NAME.MID` and saving as Cakewalk | the mapping back from Standard MIDI Files |
| `edit/NAME_X.WRK` | Cakewalk Apprentice 1.04, loading `songs/corpus/NAME.WRK`, Mark > Select All, one Edit menu command, File > Save As | the program's Edit menu (not the engine's) |
| `engine/` | Cakewalk Apprentice 1.04, Cakewalk Professional 2.00 and the 1992 demo playing `songs/engine/`, their MIDI output captured | the engine's playback; see `engine/README.md` |

The files in `app/`, `mid2wrk/` and `edit/` were produced by running Cakewalk Apprentice 1.04
under Wine 9.0 (Linux x86-64, Win16 subsystem) with the user interface driven by `xdotool`. File
names keep the 8.3 form Windows 3.1 needs.

## Edit oracles

Each file in `edit/` is a corpus song after one Edit command, with all tracks selected (Mark >
Select All) unless a From/Thru is given.

- Quantize on `QUANT.WRK`: `QUANT_16` sixteenth, `QUANT_32` thirty-second, `QUANT_8T` eighth
  triplet, `QUANT_ST` starting times only, `QUANT_DU` durations only, `QUANT_50` strength 50 %,
  `QUANT_RG` From 2:01:000 Thru 3:01:000.
- Length on `QUANT.WRK`: `LEN_50S` starts 50 %, `LEN_150B` both 150 %, `LEN_33B` both 33 %,
  `LEN_175D` durations 175 %, `LEN_50RG` both 50 % From 2:01:000 Thru 3:01:000.
- Transpose on `XPOSE.WRK`: `XPOSE_P5`, `_M7`, `_P100`, `_M100` half-steps; `XPOSE_D1`, `_D2`,
  `_DM3`, `_D7` diatonic steps.
- Slide on `QUANT.WRK`: `SLIDE_M1` +1 measure, `SLIDE_T100` +100 ticks and `SLIDE_TM200` -200
  ticks From 2:01:000 Thru 3:01:000, `SLIDE_TM600` -600 ticks on that region (clamped to -480),
  `SLIDE_MM1` -1 measure from the start (nothing happens).
- Event filter, through Quantize (starts only, sixteenth) on `QUANT.WRK` (`QUANT_F0` defaults,
  `_FKEY` key C 5..G 10, `_FKEYN` the same with Not, `_FDUR` duration 30..30, `_FCHAN` channel 1,
  `_FBEAT` beat 1, `_FTICK` tick 0..59, `_FMIX` wheel -8192..0 and Not controller 7) and on
  `EFILT.WRK` (`EFILT_VEL` velocity 20..40, `_VELN` with Not, `_KEY` C 5..B 5, `_DUR` 10..20,
  `_NUM` controller 10..20, `_VAL` value 40..60, `_KA` key aftertouch C 4..B 4 and value 10..30,
  `_PATCH` 5..15, `_CHA` 10..30, `_WHL` 0..99, `_CHAN` channels 2..5, `_BEAT` 2..3, `_TICK`
  10..50, `_INCL` Nothing then Note and Wheel), and through Slide (+100 ticks) on `SYN_KIND.WRK`
  (`SYNK_FDEF` defaults, `_FNONE` Nothing, `_FTICK` tick 10..50: the string and sysex events move
  in all three).
- Cut, Copy and Paste on `CLIP.WRK`, the region 2:01:000..3:01:000 unless noted, all tracks
  selected unless noted: `CLIP_P1` paste at 9:01:000, `_P3R` three repetitions, `_PBLEND` onto
  itself, `_PREPL` Replace at 3:01:000, `_PREPL2` with two repetitions, `_PSLIDE` Slide over at
  2:01:000, `_PSLIDE2` at 3:01:000 with two repetitions, `_CUT`, `_CUTH` with Delete hole,
  `_PTEMPO` Replace with tempo at 9:01:000, `_PREPLT` the same at 3:01:000, `_PSLTEMP` Slide over
  with tempo at 2:01:000, `_PMAPS` region 2:01:000..3:04:119 with tempo and meter/key, Replace at
  9:01:000, `_CUTMAPS` the same region cut with Delete hole, tempo and meter/key, `_PONE` and
  `_PONE2` Paste to One track 3 at 9:01:000 (one and two repetitions), `_PONER` and `_PONES` the
  same with Replace at 9:01:000 and Slide over at 2:01:000, `_PREPLSEL`, `_PSLSEL` and `_CUTHSEL`
  Replace at 9:01:000, Slide over at 2:01:000 and Cut with Delete hole with track 2 deselected.
- Controller fill on `QUANT.WRK` into track 1 (`CFILL_7` CC7 0..127 over 1:01:000..1:02:000,
  `CFILL_7D` the same downwards, `CFILL_PW` wheel -8192..8191 over a measure, `CFILL_1` CC1 100..20
  over ten ticks) and into track 4 (`CFILL_7B` CC7 50..60 on channel 4 over thirty ticks, replacing
  the CC7 at tick 17; `CFILL_7C` the same on channel 5, leaving it).

## Limits of Cakewalk Apprentice 1.04 found on the way

Cakewalk Apprentice 1.04 refuses to load a file (message "Out of memory") when a stream name is empty or when a
text event is longer than 127 bytes. String events (Text, MCI command) are kept in a Windows atom
table, which is case-insensitive: two texts that differ only in case collapse into the spelling
seen first, and an empty text event is dropped. A Wave payload is saved with its length rounded
up to a multiple of 32 bytes and zero-padded (the allocated block, not the data). A SMPTE-locked
marker is converted to whole frames and back, so its tick can move by a frame's worth of ticks.
The MIDI thru port is reset to a port that exists on the machine. After a MIDI import, event
records carry stale bytes in the fields their kind does not use: a Patch event that survives the
import (not at tick 0, so not absorbed into the track's patch) has 1 in the 16-bit duration field,
and a Channel aftertouch event has the previous key aftertouch's pressure in data2. Reading and writing them unchanged keeps such files byte-identical. Events inserted by Edit > Controller fill carry the same 1 in the duration field.
Edit > Slide clamps the amount so that the region does not move before 1:1:000 instead of showing
the error message its help describes. After an edit, the order of events on the same tick is not
the file order (an unstable sort). Under
Wine, the Event Filter dialog's numeric fields take at most two typed digits; the oracle settings
were chosen accordingly and each was confirmed by the outcome. In the Paste dialog, checking
"Tempo changes" switches "Blend" to "Replace" and choosing "Blend" clears it (Slide over keeps
it). Edit > Cut with "Delete hole" and "Tempo changes" moves the tempo map by one tick less than
the events (`CLIP_CUTMAPS`: 1440 becomes 481, not 480). The TRACK chunk holds the name
split into two 15-character fields; names longer than 30 characters survive only through the
TRKNAME chunk. The corpus stays within these limits so that every song has an oracle.

