# Test songs

The songs every observation in this repository starts from. Nothing here comes from Cakewalk's
own sample files or from any other third party: the files were written by `gencorpus`, a tool of
the earlier exploration of this library (versions 0.x, branch `archive/0.x`, song definitions in
`tests/corpus/sequences.cpp` and `tests/corpus/engine.cpp` there), in the layout Cakewalk itself
writes.

| Folder | Content |
|---|---|
| `corpus/` | sequences that exercise the model and the formats, and arrangements of public-domain music |
| `engine/` | short sequences for the engine's playback, one question each (120 ppq, 4/4: a measure is 480 ticks) |

## Corpus

The synthetic sequences exercise the model and the formats; the realistic ones are simple
arrangements of public-domain music, transcribed for this corpus (melodies by ear and from
memory, so they are not authoritative editions).

| File | Description |
|---|---|
| `SYN_MIN.WRK` | Minimal sequence: one stream, one note, defaults everywhere |
| `SYN_KIND.WRK` | Every event kind: note, key and channel aftertouch, controller, patch, wheel, sysex, MCI, text |
| `SYN_PARM.WRK` | Stream parameters: sparse numbers, channel none, Key+/Vel+/Time+, loop, mute, patch, volume, pan |
| `SYN_MAPS.WRK` | Tempo, meter and key changes, markers, SMPTE format and offset, metronome, thru, VARS flags, comments |
| `SYN_OVLP.WRK` | Overlapping same-key notes, simultaneous note-offs, 1-tick durations, chords, large times |
| `SYN_DENS.WRK` | Dense sequence: 8 streams with 1500 notes each and one stream with 9000 notes (STREAM chunk over 64 KB) |
| `SYN_STRS.WRK` | String events: lyric-style text on every beat, a 127-byte text (Cakewalk Apprentice 1.04's maximum), MCI commands |
| `SYN_WAVE.WRK` | A Wave event with a small payload (format unverified; kept separate on purpose) |
| `ROWBOAT.WRK` | Row, Row, Row Your Boat (traditional, 19th c.) as a four-part round in 6/8 |
| `FRERE.WRK` | Frere Jacques (traditional) as a round made with identical streams and Time+ offsets |
| `ODETOJOY.WRK` | Ode to Joy theme (Beethoven, Symphony No. 9, 1824) with bass and chords |
| `JINGLE.WRK` | Jingle Bells, chorus (James Lord Pierpont, 1857) with bass |
| `SILENT.WRK` | Silent Night (Franz Xaver Gruber, 1818) in 6/8 with bass and SMPTE-locked markers |
| `GREENS.WRK` | Greensleeves (English traditional, 16th c.) in A minor, 6/8, with a ritardando |
| `AMAZING.WRK` | Amazing Grace (tune New Britain, 1829) in F major, 3/4, with a pickup |
| `TWINKLE.WRK` | Twinkle, Twinkle, Little Star (French traditional, 18th c.) with block chords and varied velocities |
| `FURELISE.WRK` | Fuer Elise, opening (Beethoven, WoO 59, 1810) in 3/8 for two hands |
| `QUANT.WRK` | Edit/Quantize test bed: every start offset in a half note, durations 1..75 and around multiples of a sixteenth, one event of every MIDI kind off the grid |
| `CLIP.WRK` | Cut/Copy/Paste test bed: a melody every beat and chords with controllers over nine measures, notes across region borders, tempo changes, a 3/4 stretch and key changes |
| `EFILT.WRK` | Event filter test bed: one stream per field, 60 events each at off-grid ticks (n * 240 + n) with the filtered value running over n |
| `XPOSE.WRK` | Edit/Transpose test bed: every key 0..127, key aftertouch and other kinds, a chromatic octave in C major, D major (measure 3) and E-flat major (measure 5) |


## Engine songs

| Files | Question |
|---|---|
| `ENG_ORDR`, `E2_KORD`, `E2_OVLP`, `ENG_MULT` | the order of messages at one tick, chords, overlapping notes, several streams |
| `ENG_PARM` | stream parameters: Key+, Vel+, Time+, mute, patch, volume, pan |
| `ENG_TMPO` | tempo changes |
| `ENG_CHAS`, `E2_CHAS` | patch, wheel and controller search-back when starting later |
| `ENG_LOOP` | stream loops |
| `ENG_SYSX` | sysex banks, auto-send |
| `ENG_SYNC`, `E2_SPP`, `E3_SPPD` | MIDI sync out, Song Position Pointer, locate delay |
| `ENG_METR`, `E2_METR`, `E3_MA` to `E3_MF` | the metronome: accent, durations |
| `E2_STOP`, `E3_DENS` | stopping and resuming, a dense sequence |
| `E3_SHTL` | auto-stop, auto-rewind and auto-restart |

## Copyright

The realistic sequences use works whose authors died more than 70 years ago or that are traditional:
Beethoven (d. 1827), Gruber (d. 1863), Pierpont (d. 1893), and traditional tunes of the 16th to
19th century. The arrangements themselves are part of libseq and are released under its MIT
license.
