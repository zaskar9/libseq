# ttsprobe: observing TTSSEQ through its interface

Tools for studying the three versions of Twelve Tone's sequencer engine by running them: tracing
which engine functions the Cakewalk programs call, and calling the functions directly from a
script. Nothing here disassembles or changes Cakewalk's files. The method and the results are in
`docs/reverse-engineering/`.

The DLLs are not part of this repository; they come from your own Cakewalk disks (see `setup.sh`).

## Folders

| Folder | Content |
|---|---|
| `probe/` | `ttsprobe.c`, a 16-bit Windows program that loads a DLL and calls its functions from a script; `first.txt`, the first script |
| `wine/` | `fakeseq.c` (a fake ALSA sequencer with one MIDI output and one scripted MIDI input), `patch-winealsa.py` (Wine's own driver: the manufacturer ID of its ports), `runcw.sh` (runs a 16-bit program with both), `caps16.c` (lists the MIDI devices a 16-bit program sees) |
| `sweep/` | calling every export to measure argument sizes, finding pointer layouts, mapping file bytes to parameters, playing songs, the SPP test |
| `analysis/` | turning traces and outputs into tables: argument sizes, the API catalogue, parameter maps, `.WRK` dumps, the one-byte song variants |
| `record/` | recording with Cakewalk Professional 2.00 itself, from scripted MIDI input |
| `oracles/` | the oracle suites and their tools (below) |

`env.sh` holds the settings all scripts share: the work folder `$TTSPROBE_WORK` (default
`~/ttsprobe-work`) with the Wine prefix, the built helpers and scratch results.

## Setup (Linux x86-64)

1. Wine with 32-bit support (`wine32:i386`), `gcc` with `-m32`, Python 3, `Xvfb`, `xdotool`.
2. Open Watcom v2 for the probe: `WATCOM=<dir>`, `PATH=$WATCOM/binl64:$PATH`,
   `INCLUDE=$WATCOM/h:$WATCOM/h/win`. The probe needs `-zW` (callback thunks):
   `wcl -q -bt=windows -l=windows -ml -zW -fe=ttsprobe.exe probe/ttsprobe.c`.
3. Copy the three engines from your disks (`mcopy` from the images, `msexpand` from libmspack for
   the compressed `*.DL_`) as `TTSSEQ101.DLL`, `TTSSEQ210.DLL`, `TTSSEQ300.DLL` into one folder
   and run `./setup.sh FOLDER`. It creates the prefix, puts each DLL in `C:\PROBE\V101`, `V210` or
   `V300` with a `TTSSEQ.INI` naming the fake ports, copies the test songs, builds
   `libfakeseq.so` and `ttsprobe.exe`, and tells you how to patch Wine's ALSA driver (2.10 and
   3.00 skip MIDI ports whose manufacturer ID is Wine's placeholder 255).
4. Start a display: `Xvfb :99 &`.

## The probe

`ttsprobe SCRIPT OUTPUT` loads a DLL and runs a script of calls; the commands are described at the
top of `ttsprobe.c`. Arguments are words in the order Wine's tracer prints them: `1a2b` a word,
`d:HEX` a dword, `@N` a far pointer to buffer N, `"text"` a far pointer to a string, `$r` the last
result, `%N` a kept result, `cbN`/`ccN` a callback taking N words (Pascal or C convention). The
probe restores the stack pointer and saves SI, DI and DS around each call (3.00 does not preserve
them). `mode oracle` leaves out what differs between runs; typed calls (`call.w`, `call.d`,
`call.p`, `call.v`) print the result as its type.

`wine/runcw.sh DIR EXE LOG SECONDS [ARGS]` runs it (or a Cakewalk program) with the fake ports and
`WINEDEBUG=+snoop`, which logs every call into a 16-bit DLL with its arguments and, after the
first return, how many argument words it took. `analysis/snoopsum.py LOG MODULE EXPORTS` condenses
such a log.

## MIDI under Wine without a sound card

`wine/fakeseq.c` replaces libasound's sequencer API (`LD_PRELOAD`). Wine then sees "Fake - MIDI
Out" and "Fake - MIDI In". Everything sent to the output is written to `$FAKESEQ_LOG` with a
millisecond timestamp; `analysis/fake2cap.py` turns that into the capture format of
`tests/data/cakewalk/engine/`. `$FAKESEQ_INPUT` names a script of `<ms> <hex bytes>` lines that the
input port delivers after Wine connects to it (or after the file `$FAKESEQ_INPUT_GO` appears).

Two things to know: Wine 9.0 passes Song Position Pointer and other system common messages to ALSA
in a buffer that has gone out of scope, so the fake logs them as `sysex 00 00 00` (Wine's `+midi`
channel has the real bytes); and under Wine only one thread runs 16-bit code at a time, so a
function that waits in a loop (the SPP locate delay, the count-in) blocks the engine's timer unless
the loop yields. The probe's `cbyield 1` makes its callbacks yield.

## Oracle suites (`oracles/`)

Version-independent scripts that call the engine systematically, and the tools to run them on
every version and compare the results. The results are in `tests/data/ttsseq/`.

- `gen.py` writes the suites (`suites/*.tpl`); `render.py` turns a suite into the script of one
  version (`@open`, `@load`, `@save`, `@play` ... become that version's calls).
- `mkoracles.sh VER [SUITE...]` runs suites on one version into `tests/data/ttsseq/VER/`;
  `runall.sh` regenerates all three sets, repeats 1.01 and writes the reports (about 90 minutes).
- `cmporacles.py A B` compares two result folders; `diffversions.py A B` lists where two versions
  answer differently; `coverage.py VER` lists the exports the suites do not call.
- `try.sh VER < TEMPLATE`, `layouts.sh` and `widths.py` are for exploring: running a template,
  finding a call layout that returns, telling word from dword results.
