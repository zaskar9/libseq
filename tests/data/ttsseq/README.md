# TTSSEQ oracle sets

What the original engine answers, version by version: the suites of `tools/ttsprobe/oracles/`
run through `ttsprobe` under Wine on `TTSSEQ.DLL` 1.01, 2.10 and `TTSSEQ30.DLL` 3.00 (step 6 in
`docs/reverse-engineering/README.md`). libseq's tests run the same scripts against the library
and compare.

| Folder or file | Content |
|---|---|
| `101/`, `210/`, `300/` | per suite: the script as run on that version (`.txt`), the probe's output (`.out`), the MIDI output in capture format (`.midi`), the files the suite saved (`SUITE/`) |
| `input/IN3.TXT` | the scripted MIDI input of the `record` suite (milliseconds after the engine opens the input, then the bytes) |
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
- 1.01 is covered for 149 of its 158 API exports. Not covered: `SeqMergeStreams`,
  `SeqInsertRange`, `SeqTxMidiMsg`, `SeqSysxAsc2Bin` and `SeqReceiveSysxBank` (no working layout
  found yet), `SeqIOSeek/Read/Write` (valid only inside the file callbacks) and `SeqConfigMidi` (a
  dialog). The functions new in 2.10 and 3.00 have no suites yet.
