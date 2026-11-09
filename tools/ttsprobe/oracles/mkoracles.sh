#!/bin/bash
# mkoracles.sh VER [SUITE...]: runs the oracle suites (suites/*.tpl, or the ones named) on one
# TTSSEQ version through ttsprobe under Wine and stores the results in $TTSSEQ_DATA/VER/:
#   SUITE.txt   the rendered script          SUITE.out   the probe's output (oracle mode)
#   SUITE.midi  the MIDI output (fake port, capture format), when there was any
#   SUITE/      files the suite saved (C:\PROBE\ORC\OUT)
# The raw logs (Wine trace, fake port log) go to $TTSPROBE_WORK/oracles/VER/.
# Suite header lines: "#! input FILE" feeds $INPUTS/FILE (tests/data/ttsseq/input) to the fake MIDI input;
# "#! secs N" sets the time limit (default 90); "#! outputs 2" gives the fake a second output port;
# "#! ini FILE" runs the suite with $INPUTS/FILE as TTSSEQ.INI (the usual one is put back after);
# "#! irq" runs timer and MIDI callbacks without the Win16 lock, as at interrupt time on Windows 3.1
# (needs Wine's krnl386.exe16 built with wine/krnl386-irq.patch; see wine/build-wine16.sh); in
# virtual time it also turns on the watchdog, for the waits that call nothing ("#! watchdog" turns
# on only that, in virtual time).
# The suites run in virtual time (Wine's 16-bit MMSYSTEM built with wine/mmsystem16-vtime.patch):
# the DLL's timer, its clock and the MIDI devices run on a millisecond clock that moves only where
# the probe waits, so every run gives the same output to the millisecond. VTIME=0 runs them on the
# host's clock through the fake MIDI port instead (fakeseq), as before.
# OUTDIR=name writes to $TTSSEQ_DATA/name instead.
here=$(cd "$(dirname "$0")" && pwd)
. "$here/../env.sh"
ver=$1; shift
V=$PROBE_C/V$ver; ORC=$PROBE_C/ORC
out=$TTSSEQ_DATA/${OUTDIR:-$ver}; raw=$TTSPROBE_WORK/oracles/${OUTDIR:-$ver}; mkdir -p "$out" "$raw" "$ORC"
cp "$SONGS"/*/*.WRK "$ORC"/
suites=("$@"); [ ${#suites[@]} -eq 0 ] && suites=($(cd "$here/suites" && ls *.tpl | sed 's/\.tpl$//'))
for s in "${suites[@]}"; do
  tpl=$here/suites/$s.tpl
  secs=$(sed -n 's/^#! secs //p' "$tpl"); secs=${secs:-90}
  input=$(sed -n 's/^#! input //p' "$tpl")
  outputs=$(sed -n 's/^#! outputs //p' "$tpl")
  ini=$(sed -n 's/^#! ini //p' "$tpl")
  irq=$(grep -q '^#! irq' "$tpl" && echo 1)
  watch=$(grep -q '^#! \(irq\|watchdog\)' "$tpl" && echo 1)
  [ -n "$ini" ] && { cp "$V/TTSSEQ.INI" "$raw/TTSSEQ.INI.saved"; cp "$INPUTS/$ini" "$V/TTSSEQ.INI"; }
  rm -rf "$ORC/OUT"; mkdir -p "$ORC/OUT"
  python3 "$here/render.py" $ver "$tpl" > "$out/$s.txt"
  cp "$out/$s.txt" "$V/orc.txt"; rm -f "$V/orc.out"
  : > "$raw/$s.fake"
  if [ "${VTIME:-1}" = 1 ]; then
    WINE_VTIME=1 VTIME_LOG="$raw/$s.fake" VTIME_INPUT=${input:+$INPUTS/$input} FAKESEQ_OUTPUTS=$outputs \
    VTIME_WATCHDOG=${watch:+200} WINE_WIN16_IRQ_CALLBACKS=${watch:+serial} TRACE=-all \
        "$RUNCW" "$V" ttsprobe.exe "$raw/$s.log" $secs orc.txt orc.out || true
  else
    FAKESEQ_LOG="$raw/$s.fake" FAKESEQ_INPUT=${input:+$INPUTS/$input} FAKESEQ_OUTPUTS=$outputs WINE_WIN16_IRQ_CALLBACKS=${irq:+serial} TRACE=-all \
        "$RUNCW" "$V" ttsprobe.exe "$raw/$s.log" $secs orc.txt orc.out || true
  fi
  [ -n "$ini" ] && cp "$raw/TTSSEQ.INI.saved" "$V/TTSSEQ.INI"
  tr -d '\r' < "$V/orc.out" > "$out/$s.out" 2>/dev/null || echo "(no output)" > "$out/$s.out"
  if grep -q "short\|sysex" "$raw/$s.fake"; then python3 "$TTSPROBE/analysis/fake2cap.py" "$raw/$s.fake" "$out/$s.midi"; else rm -f "$out/$s.midi"; fi
  rm -rf "${out:?}/$s"; if [ -n "$(ls -A "$ORC/OUT")" ]; then mkdir -p "$out/$s"; cp "$ORC"/OUT/* "$out/$s/"; fi
  echo "$ver $s: $(wc -l < "$out/$s.out") lines$( [ -f "$out/$s.midi" ] && echo ", $(wc -l < "$out/$s.midi") MIDI")$( [ -d "$out/$s" ] && echo ", $(ls "$out/$s" | wc -l) files")"
done
