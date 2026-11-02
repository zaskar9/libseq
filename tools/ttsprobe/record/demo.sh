#!/bin/bash
# demo.sh: drives Cakewalk Professional for Windows (the demo on disk CWDEMO1, TTSSEQ 1.01) under
# Wine, for tracing the engine calls the program makes. Source it, then call the functions:
#
#   . tools/ttsprobe/record/demo.sh
#   demo_start TAG INPUT    start WCWDEMO.EXE (installed in C:\CWDEMO of the prefix, with the
#                           TTSSEQ.INI of setup.sh) with the fake ports and dismiss the about box.
#                           INPUT is a fake input script; its blocks start when demo_go is called
#                           ("go" lines in the script wait for the next demo_go; see fakeseq.c).
#                           Logs: $TTSPROBE_WORK/demo/TAG.log (snoop) and TAG.fake (MIDI).
#                           WINCAKE.INI is put back as installed first (the program saves its
#                           settings, multi-take mode among them, there).
#   demo_take YES|NO        rewind, record, start the input after the count-in, stop after
#                           $TAKE_SECS (default 10.5) seconds and answer "Keep take?"
#   demo_punch IN OUT       Realtime > Punch-in values: enabled, IN and OUT as M:B:T
#   demo_shuttle FROM STOP  Realtime > Auto-shuttle: rewind to FROM, stop at STOP, restart
#   demo_calls [FROM_LINE]  the engine calls of the log so far (analysis/snoopcalls.py)
#   demo_shot NAME          a screenshot to $TTSPROBE_WORK/demo/NAME.png
#
# The snoop trace leaves out the program's own exports and the calls it polls with (SnoopExclude in
# the prefix's registry), which otherwise make gigabytes of log. The screen positions assume a
# 1024x768 display without a window manager (Xvfb :99). The demo's Punch-in and Auto-shuttle
# dialogs need Wine's user32 with user32-dlgproc.patch (see ../wine/build-wine16.sh).
. "$(dirname "${BASH_SOURCE[0]}")/../env.sh"
DEMO_OUT=$TTSPROBE_WORK/demo; mkdir -p "$DEMO_OUT"
export DISPLAY=${DISPLAY:-:99}
NOISE='SEQIDLE;SEQGETNOW;SEQTICK2MEAS;SEQGETTEMPORATIO;SEQGETEND;SEQWHICHTEMPORATIO;SEQTICK2MBTSTR;SEQTICK2HMSFSTR;SEQGETTEMPO;SEQGETPPQ;SEQGETMETER;SEQMETERIXBYTICK;SEQMBT2TICK;SEQMARKERFORTIME;SEQGETMARKERTIME'

demo_start() {
  local tag=$1 in; in=$(cd "$(dirname "$2")" && pwd)/$(basename "$2")
  wine reg add 'HKCU\Software\Wine\Debug' /v SnoopExclude /t REG_SZ \
    /d "WCWDEMO;$(echo "$NOISE" | sed 's/\([A-Z0-9]*\)/TTSSEQ.\1/g')" /f > /dev/null 2>&1; wineserver -w
  local d=$WINEPREFIX/drive_c/CWDEMO   # the program keeps settings such as multi-take mode here
  [ -f "$d/WINCAKE.INI.orig" ] || cp "$d/WINCAKE.INI" "$d/WINCAKE.INI.orig"; cp "$d/WINCAKE.INI.orig" "$d/WINCAKE.INI"
  rm -f "$DEMO_OUT/go"; DEMO_LOG=$DEMO_OUT/$tag.log
  FAKESEQ_LOG=$DEMO_OUT/$tag.fake FAKESEQ_INPUT=$in FAKESEQ_INPUT_GO=$DEMO_OUT/go TRACE=${TRACE:-+snoop} \
    setsid "$RUNCW" "$WINEPREFIX/drive_c/CWDEMO" WCWDEMO.EXE "$DEMO_LOG" 3000 > /dev/null 2>&1 < /dev/null &
  sleep 14; xdotool mousemove 656 421 click 1; sleep 1          # the about box's Continue
}
demo_go() { touch "$DEMO_OUT/go"; }
demo_take() {
  xdotool mousemove 173 84 click 1; sleep 1                       # REW
  xdotool mousemove 268 84 click 1; sleep 2.4; demo_go            # REC; the input after the count-in
  sleep "${TAKE_SECS:-10.5}"; xdotool mousemove 268 84 click 1; sleep 2   # REC again stops
  if [ "$1" = YES ]; then xdotool mousemove 483 417 click 1; else xdotool mousemove 541 417 click 1; fi; sleep 2
}
demo_field() { xdotool mousemove "$1" "$2" click 1; xdotool key End shift+Home BackSpace; xdotool type --delay 50 "$3"; }
demo_punch() {
  xdotool mousemove 58 40 click 1; sleep 0.5; xdotool mousemove 90 154 click 1; sleep 1.5
  xdotool mousemove 399 369 click 1; sleep 0.3                    # Enabled
  demo_field 490 387 "$1"; demo_field 490 409 "$2"; xdotool key Tab; sleep 0.3
  xdotool mousemove 590 371 click 1; sleep 1                      # OK
}
demo_shuttle() {
  xdotool mousemove 58 40 click 1; sleep 0.5; xdotool mousemove 85 170 click 1; sleep 1.5
  xdotool mousemove 394 369 click 1; xdotool mousemove 406 396 click 1; xdotool mousemove 394 420 click 1
  demo_field 520 369 "$1"; demo_field 520 396 "$2"; xdotool key Tab; sleep 0.3
  xdotool mousemove 613 371 click 1; sleep 1
}
demo_calls() { python3 "$TTSPROBE/analysis/snoopcalls.py" "$DEMO_LOG" "${1:-0}"; }
demo_shot() { import -window root "$DEMO_OUT/$1.png"; }
