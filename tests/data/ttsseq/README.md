# TTSSEQ oracle sets

What the original engine answers, version by version: the suites of `tools/ttsprobe/oracles/`
run through `ttsprobe` under Wine on `TTSSEQ.DLL` 1.01, 2.10 and `TTSSEQ30.DLL` 3.00 (step 6 in
`docs/reverse-engineering/README.md`). libseq's tests run the same scripts against the library
and compare.

| Folder or file | Content |
|---|---|
| `101/`, `210/`, `300/` | per suite: the script as run on that version (`.txt`), the probe's output (`.out`), the MIDI output in capture format (`.midi`), the files the suite saved (`SUITE/`) |
| `input/IN3.TXT`, `input/SYSX.TXT` | the scripted MIDI input of the `record` and `sysexin` suites (milliseconds after the engine opens the input, then the bytes) |
| `input/PORTS.INI` | the `TTSSEQ.INI` of the `ports` suite: two output devices in reverse order, sync to port 2 |
| `repeat-101.txt` | 1.01 run a second time and compared: whether the results repeat |
| `coverage.txt` | the exports each version's suites call and do not call |
| `diff-101-210.txt`, `diff-210-300.txt` | where two versions answer the same calls differently |

## Reading the outputs

Each line of a `.out` file is a probe command and what came back: `SEQGETTEMPO(0001,d:000001e0)
-> 00001770` is `SeqGetTempo(1, 480)` returning 6000 (60.00 bpm). Words are hexadecimal; `d:` is
a 32-bit value; `@N` a pointer to the probe's buffer N, whose contents follow in `dump` lines;
`%N` a handle the engine returned earlier; `cbN(...)` a call the engine made back into the probe.
`signatures.md` in `docs/reverse-engineering/` gives the layout of every call.

## How exact they are

- Outputs and saved files repeat byte for byte (`repeat-101.txt`).
- MIDI repeats in content and order. In the suites that play, the times repeat within 30 ms; a
  few messages of the densest song are up to 115 ms off, when Wine's timer stalls. Times outside
  playback only say how long the calls took and are not compared.
- The `record` suite records live input, so its event times may differ by a few ticks between
  runs.
- The `ports` suite runs with two fake output ports; its MIDI lines name the fake port (`@0`,
  `@2`). The `fileio` suite masks the file handle and the pointers in its callbacks.
- The `sysexin` suite receives sysex from scripted input; it needs Wine's 16-bit MMSYSTEM with
  `tools/ttsprobe/wine/mmsystem16-midiin.patch`. Its callback words with the free memory are
  masked, as they differ from run to run.
- Coverage (`coverage.txt`) counts the exports that at least one suite calls. It does not say
  whether the meaning of every argument is known. For the 158 API exports of 1.01:
  - 157 are called, and their layouts are known (`docs/reverse-engineering/signatures.md`).
    `SeqIOSeek/Read/Write` are called inside the load and save callbacks (`fileio`).
  - 1 is not called by any suite: `SeqConfigMidi`, which opens a dialog. It was observed with the
    dialog worked by `xdotool` (`docs/reverse-engineering/devices/`), and it is not ported; the
    `ports` suite covers its effect, the port assignment in `TTSSEQ.INI`.

  So `coverage.txt` reports 157 of 158 as covered.
- Of the functions new in 2.10 and 3.00 only `SeqReceiveSysxBankEx` has a suite so far.
