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
| `wine/` | `fakeseq.c` (a fake ALSA sequencer with one MIDI output and one scripted MIDI input), `patch-winealsa.py` (Wine's own driver: the manufacturer ID of its ports), `mmsystem16-midiin.patch` (Wine's 16-bit MMSYSTEM: MIDI input buffers, for sysex), `krnl386-irq.patch` (Wine's KRNL386: callbacks at interrupt time, for external sync), `user32-dlgproc.patch` (Wine's user32: 16-bit dialog procedures, for Cakewalk Professional's dialogs) and `build-wine16.sh` (builds all three), `runcw.sh` (runs a 16-bit program with the fake ports), `caps16.c` (lists the MIDI devices a 16-bit program sees) |
| `sweep/` | calling every export to measure argument sizes, finding pointer layouts, mapping file bytes to parameters, playing songs, the SPP test, working the device dialog (`configmidi.sh`), single external-sync experiments (`syncexp.py`, and `mtcexp.py` for time code with frame rates and offsets), what one `TTSSEQ.INI` option changes (`options.py`), one recording with chosen `SeqRecordStart` arguments (`recstart.py`) |
| `analysis/` | turning traces and outputs into tables: argument sizes, the API catalogue, parameter maps, `.WRK` dumps, the one-byte song variants; `snoopcalls.py` lists a program's engine calls with their arguments |
| `record/` | recording with the programs themselves, from scripted MIDI input: Cakewalk Professional 2.00 (`prorec.sh`) and the Professional demo on 1.01 (`demo.sh`) |
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
   3.00 skip MIDI ports whose manufacturer ID is Wine's placeholder 255) and how to build Wine's
   16-bit MMSYSTEM with MIDI input buffers and its KRNL386 with interrupt-time callbacks (below).
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
input port delivers after Wine connects to it (or after the file `$FAKESEQ_INPUT_GO` appears):
channel messages, sysex, MIDI sync (`f8` Clock, `fa` Start, `fb` Continue, `fc` Stop, `f2 lsb msb`
Song Position Pointer) and MIDI Time Code quarter frames (`f1 nn`). With `$FAKESEQ_INPUT_GO`, the script
starts when that file appears, and a line `go` waits for it again, so one script can feed
several takes of a program.

Two things to know: Wine 9.0 passes Song Position Pointer and other system common messages to ALSA
in a buffer that has gone out of scope, so the fake logs them as `sysex 00 00 00` (Wine's `+midi`
channel has the real bytes); and under Wine only one thread runs 16-bit code at a time, so a
function that waits in a loop (the SPP locate delay, the count-in) blocks the engine's timer unless
the loop yields. The probe's `cbyield 1` makes its callbacks yield; for a loop without a callback
(the locate delay, the wait for MIDI Start) see interrupt-time callbacks below.

Sysex input needs one more change to Wine. A 16-bit program receives system exclusive messages
into buffers it hands to the driver (`midiInPrepareHeader`, `midiInAddBuffer`); Wine 9.0 does not
translate those buffers between 16 and 32 bits and returns an error, which `SeqReceiveSysxBank`
reports as -4 and Cakewalk Professional 2.00 as "Out of memory". `wine/mmsystem16-midiin.patch`
translates them the way Wine already translates the buffers of MIDI output, and
`wine/build-wine16.sh` builds the patched `mmsystem.dll16` for the installed Wine version. Like
the ALSA driver patch, it changes Wine, not Cakewalk's files.

External sync needs a change to how Wine runs callbacks. Windows 3.1 calls the callbacks of
multimedia timers and MIDI drivers at interrupt time: they interrupt the program wherever it is.
Wine calls them from threads of its own, and only when no thread runs 16-bit code (the Win16
lock). TTSSEQ 1.01 waits for MIDI Start in a loop inside `SeqPlayStart` that only its MIDI input
callback can end, so under Wine it waits for ever and blocks the other threads too.
`wine/krnl386-irq.patch` adds `WINE_WIN16_IRQ_CALLBACKS`: set to `serial`, callbacks on threads
that are not Win16 tasks run at once without the Win16 lock, one at a time in the order they
come, while the program's 16-bit code stands still (it is suspended if it is in 16-bit code, as
an interrupt stops a program), and 16-bit `cli` ... `sti` (which Wine otherwise ignores) holds
them off, as disabling interrupts does. Unset, Wine behaves as before. `build-wine16.sh` builds
this `krnl386.exe16` too; a suite asks for the mode with `#! irq`, and `runcw.sh` passes the
variable on; `WINE_IRQ_TIMING=1` reports callbacks that waited or ran 5 ms or more. Letting the
program run on beside the callbacks was not enough: it made a Continue after a Song Position
leave the engines deaf to the clock in about one run in five (see `ttsseq-api.md`).
`sweep/syncexp.py VER SONG SPEC...` runs one sync experiment from the command line: input
messages, clocks and probe steps in one list, and a time line of what came back.

Virtual time takes the host's clock out of the measurements. On the host's clock the engine's
timer and the MIDI input under Wine came a few milliseconds early or late, differently on every
run. `wine/mmsystem16-vtime.patch` (on top of the MIDI input patch; `build-wine16.sh` applies
both) adds `WINE_VTIME=1`: the 16-bit multimedia timer, `timeGetTime` and the MIDI devices then
run on a millisecond clock that moves only when the program calls `VTIMEADVANCE` (MMSYSTEM.2000).
The probe calls it where it waits: once per millisecond of `sleep` and `idle`, and once in each
callback with `cbyield 1`. A call into the DLL takes no time; at each millisecond the timers due
run first, then the input due. MIDI input comes from `$VTIME_INPUT` (the fake port's script
format, times from `midiInOpen`), and what goes out, and the input delivered, is written to
`$VTIME_LOG` in the fake port's log format. The probe's `time` prints the virtual clock. Every
run gives the same output to the millisecond, so an experiment can place a call or an input at
any millisecond. A wait that calls nothing (the wait for MIDI Start without a callback) would
stop the clock; `VTIME_WATCHDOG=MS` moves it on from a thread of its own while the program has
not touched it for MS ms of real time (with `WINE_WIN16_IRQ_CALLBACKS=serial`).
`oracles/mkoracles.sh` runs every suite in virtual time (`VTIME=0` for the host's clock); `#! irq`
and `#! watchdog` turn the watchdog on. The script runner (`tests/runner`) keeps time the same way, so libseq is compared with
the DLL at the exact millisecond (`ttsseq-api.md`, Timing).

For functions that wait in a loop until their callback says stop (`SeqReceiveSysxBank`), the
probe's `cbretafter MS HEX` switches the callbacks' result after MS milliseconds, and `cbmask HEX`
hides callback words that differ from run to run (the free memory the engine reports); with
`cbmask HEX merge`, repeated calls that differ only in those words count as one.

Some functions only work inside a callback (`SeqIORead` while the engine loads a file). `cbdo cbN
W0 LINE` makes callback cbN run LINE when its first word is W0 (`*` for any); in LINE, `%wN` is
the callback's word N, `letw N K` keeps word K in variable N, `%bN+OFF` passes a word read from a
buffer, and `ret HEX` sets what that call of the callback returns. `dumpcb K OFF LEN` and
`fillcb K OFF HEX...` read and write the bytes at the far pointer in the callback's words K and
K+1: an event procedure's event, which the engine passes by address. `win` makes a hidden window
for functions that report to one (`SeqStepRecSetup`): the argument `hwnd` passes it, its messages
print like callbacks, and `cbdo msg 0464 ...` answers them; `set` and `add` change a variable.

`FAKESEQ_OUTPUTS=2` gives the fake a second output port, "MIDI Out 2"; the log then names the
port of every message (`short@2 ...`). A suite asks for it with `#! outputs 2`, and with
`#! ini FILE` for its own `TTSSEQ.INI` (from `tests/data/ttsseq/input`, put back afterwards).
`sweep/options.py VER TEMPLATE KEY=VALUE...` runs a template once with the usual `TTSSEQ.INI` and
once per setting, and prints what changed: the probe's output, the MIDI, and with `--relay` the
engine's timer, MIDI buffer and memory calls.
`sweep/recstart.py VER WORDS...` records once like the `record` suite with the given leading
arguments of `SeqRecordStart` and prints the take, the streams asked for and the MIDI; options
add probe lines before the start, during the take and after it.
`record/demo.sh` drives Cakewalk Professional for Windows, the demo on 1.01, which records like
the full program but cannot save (copy it from disk `CWDEMO1` into `C:\CWDEMO` of the prefix with
the `TTSSEQ.INI` of setup.sh): takes, punch-in, auto-shuttle, with the snoop trace limited to the
engine's calls. `analysis/snoopcalls.py LOG` lists those calls with their arguments.
`sweep/configmidi.sh VER` works `SeqConfigMidi`'s dialog with `xdotool` and reports what each way
of working it changed.

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
