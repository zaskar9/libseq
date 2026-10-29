# MIDI output captures

The MIDI output of Cakewalk Apprentice 1.04 (engine 2.10) playing the engine test songs
(`../../songs/engine/`). The same songs played by the two other engine versions are in `pro/` and
`demo/` (see "Other engines" below). Step 3 in `docs/reverse-engineering/README.md`.

## How they were made

Cakewalk Apprentice 1.04 ran on Windows 3.1 (MS-DOS 6.22) in the 86Box emulator (i486DX/33,
Sound Blaster AWE32 with its Windows 3.1 drivers). Cakewalk's output port was the AWE32's MPU-401
port; 86Box forwarded that port to the host as "System MIDI", where a small CoreMIDI program
received every message and wrote it with a host timestamp. The sequences went into the emulator
on a floppy image. Only the program's output was observed; nothing was decompiled.

The songs are in `../../songs/engine/` (120 ppq, 4/4, so one measure is 480 ticks); `FURELISE` is
the corpus song `../../songs/corpus/FURELISE.WRK`.

## Capture format

One line per MIDI message: milliseconds since the first message of the take, then the bytes in
hex. Lines starting with `#` are comments. Note-offs appear as Note On with velocity 0 (that is
what the engine sends). `f8` is MIDI Clock, `fa` Start, `fb` Continue, `fc` Stop. The engine's
timer runs at about 10 ms, so times scatter by up to about 12 ms around the exact tick times.

## Takes

| Capture | Sequence | What was done |
|---|---|---|
| `ENG_ORDR-open` | `ENG_ORDR` | File > Open |
| `ENG_ORDR-play` | `ENG_ORDR` | Play from 1:1:000 to the end |
| `ENG_ORDR-stop` | `ENG_ORDR` | Play, Stop at about 2:1:105 (tick 585) while the chord sounds |
| `ENG_ORDR-resume` | `ENG_ORDR` | Play again from there, Stop at about tick 1176 |
| `ENG_MULT-open` | `ENG_MULT` | File > Open (taken from the end of the `ENG_METR-play` take) |
| `ENG_MULT-play` | `ENG_MULT` | Play to the end |
| `ENG_PARM-play` | `ENG_PARM` | Play to the end |
| `ENG_TMPO-play` | `ENG_TMPO` | Play to the end |
| `ENG_CHAS-play` | `ENG_CHAS` | Open (Now = 3:1:000), Play to the end; search-back off |
| `ENG_LOOP-play` | `ENG_LOOP` | Play to the end |
| `ENG_SYSX-open` | `ENG_SYSX` | File > Open: empty, the auto-send bank is not sent |
| `ENG_SYSX-play` | `ENG_SYSX` | Play to the end |
| `ENG_SYNC-open` | `ENG_SYNC` | File > Open, then stopped: Clock keeps running |
| `ENG_SYNC-play` | `ENG_SYNC` | Play, Stop near tick 610, Play (Continue) to the end, Play again from the start |
| `ENG_METR-play` | `ENG_METR` | Play to the end, metronome on |
| `E2_KORD-play` | `E2_KORD` | Play to the end |
| `E2_OVLP-play` | `E2_OVLP` | Play to the end |
| `E2_STOP-play` | `E2_STOP` | Play, Stop at about tick 390 |
| `E2_CHAS-open` | `E2_CHAS` | File > Open |
| `E2_CHAS-play` | `E2_CHAS` | Play from 3:1:000 to the end; search-back and zero controllers on |
| `E2_METR-play` | `E2_METR` | Play to the end, metronome on |
| `E2_SPP-play` | `E2_SPP` | Play from 2:1:000 to the end, then Rewind |
| `E2_SPP-locate` | `E2_SPP` | GoTo next measure while stopped, Play, Stop |
| `FURELISE-open` | `FURELISE` | File > Open (the clocks come from the sync of the file before) |
| `FURELISE-play` | `FURELISE` | Play to the end |
| `E3_MA-play` .. `E3_MD-play` | `E3_MA` .. `E3_MD` | Play to the end: metronome durations 1, 20 (60 bpm), 10, 40 (120 bpm) |
| `E3_ME-play`, `E3_MF-play` | `E3_ME`, `E3_MF` | `E3_MD` with the duration set to 2 and 24 in the Metronome dialog (the songs have those durations) |
| `E3_DENS-stop` | `E3_DENS` | Play, Stop at about tick 561 (shown as 2:1:080) |
| `E3_DENS-resume1..3` | `E3_DENS` | Play again from where it stopped, Stop; three times (Now shown as 2:4:115, 3:3:025, 3:4:080 after them) |
| `E3_SPPD-play` | `E3_SPPD` | Play from 2:1:000 with "Send MIDI SPP" on and a locate delay of 5 |
| `E3_SHTL-play` | `E3_SHTL` | Play: the VARS auto-stop, auto-rewind and auto-restart bytes are set |

## What the captures show

- Loading a file sends the patch, volume and pan of every stream with events, in stream-number
  order, muted streams included. Play sends them again, muted streams left out.
- At one tick: due note-offs first; then the streams in stream-number order, each stream's
  events in stored order (a patch, controller or note keeps its place); sysex after the short
  messages.
- Pending note-offs sit in a binary min-heap on time. Equal times come out in the heap's order:
  a new entry moves up past parents with an equal time, the last entry moves down after a removal
  only past strictly earlier children. This reproduces every chord and every multi-stream
  release in the captures (for example notes started on keys 60, 62, 64, 65, 72 end as 72, 64,
  60, 62, 65).
- A note-on for a key already sounding on that channel sends a note-off first. The first note-off
  due for a key ends it; later ones for that key are dropped (`E2_OVLP`).
- Stop and the end of the sequence: MIDI Stop (with sync on); the metronome click's note-off; then
  for channels 1 to 16 the sounding notes' offs in key order, each channel followed by All Notes
  Off (CC 123). With "Zero continuous controllers", then for every channel: pitch wheel centre,
  CC 1 = 0, CC 64 = 0, CC 121.
- Patch/wheel/controller search-back: for each stream that still has events at the start
  position, the last patch, pitch wheel, CC 1, CC 64 and CC 7 before it, in that order, after the
  stream parameters. A stream whose events all lie before the start is not chased. Pan, other
  controllers, aftertouch and held notes are not chased. With search-back off, nothing is.
- Key+ outside 0..127 folds by octaves (120 + 12 plays 120); Vel+ clamps to 1..127; Time+ that
  moves an event before the start plays it at tick 0 with its full duration.
- MIDI sync: Start when playback begins at tick 0, Continue otherwise; Clock every 1/24 quarter
  note, before the events of its tick; Stop on stop. Clock also runs while stopped.
- No Song Position Pointer appears in these captures. That was 86Box, not the engine: its MIDI
  output does not pass `F2` on. Called directly under Wine, every version sends an SPP at each
  start when the file asks for it (step 5; `docs/reverse-engineering/ttsseq-api.md`).
- Metronome: the MIDI click has the same velocity on every beat, accent on or off (engine 3.00
  accents, see below); a click is
  played on the end beat. The duration counts MIDI clocks (1/24 quarter note; the dialog accepts
  1 to 24 and says so when given 25). The click ends two clocks before that count (duration 10:
  8 clocks, 20: 18, 24: 22, 2: at once), duration 1 wraps round to 23 clocks, and never later than
  one clock before the next measure. A click still sounding when the next one comes (duration 40,
  set through the file) ends right before it. On stop the click's note-off goes first.
- Resuming after a stop does not start exactly where it stopped: it restarted 5 to 25 ticks
  earlier in the takes (`ENG_ORDR-resume`, `ENG_SYNC-play`, `E3_DENS-resume1..3`, where the last
  note before the stop plays again). The amount varies from take to take and looks like the lag of
  the position the program reads back on stop.
- The auto-stop, auto-rewind and auto-restart bytes of VARS, which Cakewalk Apprentice 1.04 has no dialog for,
  change nothing when a file sets them (`E3_SHTL-play` plays straight through). Cakewalk Apprentice 1.04
  has no play loop (its help lists auto-shuttle as a Cakewalk Professional feature). Cakewalk
  Professional, on the 1.01 and 3.00 engines, does loop (below). The 2.10 engine can too: called
  directly, it loops (step 5). The program sets the shuttle parameters to 0 after loading.

## Other engines

| Folder | Program | Engine |
|---|---|---|
| `pro/` | Cakewalk Professional 2.00 | TTSSEQ30.DLL 3.00 |
| `demo/` | Cakewalk Professional for Windows demo (README of 12 May 1992; loads and plays, cannot save) | TTSSEQ.DLL 1.01 |

Same emulator, port, capture tool and sequences as above, and the same take names. The takes
stopped by hand stopped at slightly different positions. `pro/idle.txt` is Professional 2.00 stopped with sync on. There is no
`E3_SPPD` take for the demo.

The three engines share everything listed above, except:

| Behaviour | 1.01 | 2.10 | 3.00 |
|---|---|---|---|
| Search-back | as 2.10 | patch, wheel, CC 1, 64, 7; streams with events after Now | also CC 10 (pan, last), and streams whose events all lie before Now (`E2_CHAS`) |
| Metronome click length | d - 2 clocks (1 wraps to 23) | d - 2 clocks | d - 1 clocks (1 ends at once) |
| Metronome accent | no effect | no effect | first beat of the measure at velocity 127 (`ENG_METR`, velocity 110) |
| Time+ moving an event before 0 | the event does not play (`ENG_PARM`) | plays at tick 0 | plays at tick 0 |
| MIDI sync out | none, also with the flag on (`ENG_SYNC`) | yes | yes, Clock also while stopped |
| Sysex timing | on time | on time | 20 to 40 ms after its tick |
| Timer jitter | about 10 ms | about 10 ms | 1 to 4 ms |
| Song Position Pointer | not passed on by 86Box | not passed on by 86Box | not passed on by 86Box |

What Cakewalk Professional does that Cakewalk Apprentice 1.04 does not, on both engines:

- Auto-shuttle (`E3_SHTL`: auto-stop at 960, auto-rewind to 480, auto-restart): play from Now to
  the stop time, then continue at the rewind time without stopping. The events at the stop tick
  never play, there is no All Notes Off at the wrap, and the wrap costs a few ms (3 to 5). The loop button shows on after loading such a file. Only this combination was
  captured.
- Sending the auto-send sysex banks on File > Open (`ENG_SYSX-open`).

At the end of a sequence whose last output is not a sounding note-off (`ENG_PARM`, `E2_OVLP`),
the 1.01 engine stops 120 to 620 ms late, apparently when a slower poll notices.
