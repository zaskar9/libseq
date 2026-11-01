#!/bin/bash
# configmidi.sh VER [CASE...]: observes SeqConfigMidi's "Select MIDI Devices" dialog. For each case
# the probe opens the engine, reads the port names, calls SeqConfigMidi, and xdotool works the
# dialog on the X display; afterwards the probe reads the port names again, sends a note to ports 1
# and 2, reopens the engine and does the same. Printed per case: the probe's output (port names as
# text), TTSSEQ.INI after the dialog, and the MIDI each fake port got (@0 the fake's first output,
# @2 its second). Needs the fake with two outputs (FAKESEQ_OUTPUTS=2, set here) and Xvfb on :99.
# TTSSEQ.INI is set to the usual one before each case and put back at the end.
# Cases: both only2 order selorder cancel sync (all versions), low medium (3.00); default: all.
here=$(cd "$(dirname "$0")" && pwd)
. "$here/../env.sh"
ver=${1:?usage: configmidi.sh VER [CASE...]}; shift
V=$PROBE_C/V$ver; W=$TTSPROBE_WORK/configmidi; mkdir -p "$W"
INI='[Options]\r\nSyncOutputPort=1\r\n[MIDI Output Devices]\r\n0=Fake - MIDI Out\r\n[MIDI Input Devices]\r\n0=Fake - MIDI In\r\n'
# where the dialog's controls are (the list rows: the fake's first output, Wine's MIDI mapper, the second output)
case $ver in
  101) OUT=302; R1=73; R2=85; R3=97; MOVE="343 271"; SYNC="377 297"; OK="472 64"; CANCEL="472 90" ;;
  210) OUT=280; R1=73; R2=85; R3=97; MOVE="335 271"; SYNC="377 297"; OK="472 64"; CANCEL="472 90" ;;
  300) OUT=520; R1=291; R2=303; R3=315; MOVE="580 430"; SYNC="463 430"; OK="726 267"; CANCEL="726 292"
       LOW="287 481"; MEDIUM="287 501" ;;
esac
toggle() { echo "mousemove $OUT $1 keydown ctrl click 1 keyup ctrl"; }
syncport() { echo "mousemove $SYNC click --repeat 2 1"; echo "key ctrl+a"; echo "type $1"; }
cases=("$@"); [ ${#cases[@]} -eq 0 ] && cases=(both only2 order selorder cancel sync $( [ $ver = 300 ] && echo low medium ))
for c in "${cases[@]}"; do
  button=$OK
  case $c in
    both) acts=("$(toggle $R3)") ;;                                            # add the second output
    only2) acts=("$(toggle $R1)" "$(toggle $R3)") ;;                           # only the second output
    order) acts=("$(toggle $R1)" "$(toggle $R3)" "mousemove $MOVE click 1" "$(toggle $R2)") ;;  # second output moved to the top, then the first added
    selorder) acts=("$(toggle $R1)" "$(toggle $R3)" "$(toggle $R1)") ;;        # second selected before the first, no move
    cancel) acts=("$(toggle $R3)"); mapfile -t -O 1 acts < <(syncport 2); button=$CANCEL ;;
    sync) acts=("$(toggle $R3)"); mapfile -t -O 1 acts < <(syncport 2) ;;
    low) acts=("mousemove $LOW click 1") ;;
    medium) acts=("mousemove $MEDIUM click 1") ;;
    *) echo "unknown case $c"; continue ;;
  esac
  printf "$INI" > "$V/TTSSEQ.INI"
  { echo @open; echo 'buf 0 32'
    names() { for p in 0000 0001 0002; do echo "fill 0 0 $(printf 'ee %.0s' $(seq 24))"; echo "call.w SEQGETPORTNAME $p @0"; echo 'dump 0 0 24'; done; }
    names; echo 'sleep 500'; echo 'call.w SEQCONFIGMIDI 0000'; names
    echo 'echo == a note to port 1, then to port 2'
    echo 'call.w SEQTXMIDIMSG 0000 0000 0090 003c 0064'; echo 'call.w SEQTXMIDIMSG 0001 0000 0090 003d 0064'; echo 'sleep 300'
    echo 'echo == close and open again'; echo 'call.w SEQCLOSE'; echo 'call.w SEQOPEN 0000 0000'; names
    echo 'call.w SEQTXMIDIMSG 0000 0000 0090 003e 0064'; echo 'call.w SEQTXMIDIMSG 0001 0000 0090 003f 0064'; echo 'sleep 300'; echo @close
  } | RAW=1 python3 "$here/../oracles/render.py" $ver /dev/stdin > "$V/cfg.txt"
  rm -f "$V/cfg.out"; : > "$W/$c.fake"
  (FAKESEQ_OUTPUTS=2 FAKESEQ_LOG="$W/$c.fake" TRACE=-all "$RUNCW" "$V" ttsprobe.exe "$W/$c.log" 90 cfg.txt cfg.out &)
  sleep 12                                   # the dialog is open by then
  for a in "${acts[@]}"; do DISPLAY=${DISPLAY:-:99} xdotool $a; sleep 0.7; done
  DISPLAY=${DISPLAY:-:99} import -window root "$W/$c.png" 2>/dev/null
  DISPLAY=${DISPLAY:-:99} xdotool mousemove $button click 1
  for i in $(seq 1 60); do sleep 0.5; grep -q "^SEQCLOSE" "$V/cfg.out" 2>/dev/null && [ "$(grep -c '^SEQCLOSE' "$V/cfg.out")" -ge 2 ] && break; done
  sleep 1
  echo "##### $ver $c"
  tr -d '\r' < "$V/cfg.out" | python3 -c '
import sys
lines = sys.stdin.read().splitlines(); i = 0
while i < len(lines):
    l = lines[i]
    if l.startswith("SEQGETPORTNAME") and i + 3 < len(lines):
        b = bytes.fromhex(" ".join(lines[i + 2:i + 4]))
        print(l.split(" ->")[0], "->", repr(b.split(b"\0")[0].decode("latin1"))); i += 4; continue
    print(l); i += 1'
  echo "--- TTSSEQ.INI"; tr -d '\r' < "$V/TTSSEQ.INI"
  echo "--- MIDI"; grep -E "^ *[0-9.]+ short@" "$W/$c.fake" | grep -v " 7a 00\| 78 00\| 40 00" | sed 's/^ *[0-9.]* //'
done
printf "$INI" > "$V/TTSSEQ.INI"
