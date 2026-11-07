#!/bin/bash
# runcw.sh DIR EXE LOG SECONDS [ARGS...]: runs a 16-bit Windows program in DIR under Wine with the
# fake MIDI ports and Wine's call trace, in a minimal environment (the 16-bit C run-times of the
# Cakewalk programs reject a large one). The trace goes to LOG.
#   TRACE           WINEDEBUG channels (default +snoop: every call into a 16-bit DLL)
#   FAKESEQ_LOG     where the fake output port writes the MIDI it receives (default /dev/null)
#   FAKESEQ_INPUT   a script of timed messages for the fake input port; FAKESEQ_INPUT_GO a file
#                   whose appearance starts it (see fakeseq.c)
#   FAKESEQ_OUTPUTS 2 gives the fake a second output port, and the log the port of each message
#   WINE_WIN16_IRQ_CALLBACKS  "serial": timer and MIDI callbacks run at once, as at interrupt time
#                   on Windows 3.1 (needs Wine's krnl386.exe16 with krnl386-irq.patch);
#                   WINE_IRQ_TIMING=1 reports callbacks that waited or ran 5 ms or more
#   WINE_VTIME      1: virtual time (needs Wine's mmsystem.dll16 with mmsystem16-vtime.patch): the
#                   16-bit timer, clock and MIDI devices run on a clock the probe moves on where it
#                   waits; VTIME_LOG gets the MIDI out and in, VTIME_INPUT is the input script;
#                   VTIME_WATCHDOG=MS moves the clock on while the program has not used it for MS
#                   ms of real time (waits that call nothing; with WINE_WIN16_IRQ_CALLBACKS)
#   DISPLAY         the X display (default :99, for example Xvfb)
. "$(dirname "$0")/../env.sh"
dir=$1; exe=$2; log=$3; secs=$4; shift 4
cd "$dir" || exit 1
env -i HOME="$HOME" PATH=/usr/bin:/bin DISPLAY="${DISPLAY:-:99}" WINEPREFIX="$WINEPREFIX" \
    LD_PRELOAD="$FAKESEQ_LIB" FAKESEQ_LOG="${FAKESEQ_LOG:-/dev/null}" \
    ${FAKESEQ_INPUT:+FAKESEQ_INPUT=$FAKESEQ_INPUT} ${FAKESEQ_INPUT_GO:+FAKESEQ_INPUT_GO=$FAKESEQ_INPUT_GO} ${FAKESEQ_OUTPUTS:+FAKESEQ_OUTPUTS=$FAKESEQ_OUTPUTS} \
    ${WINE_WIN16_IRQ_CALLBACKS:+WINE_WIN16_IRQ_CALLBACKS=$WINE_WIN16_IRQ_CALLBACKS} ${WINE_IRQ_TIMING:+WINE_IRQ_TIMING=1} \
    ${WINE_VTIME:+WINE_VTIME=$WINE_VTIME} ${VTIME_LOG:+VTIME_LOG=$VTIME_LOG} ${VTIME_INPUT:+VTIME_INPUT=$VTIME_INPUT} ${VTIME_WATCHDOG:+VTIME_WATCHDOG=$VTIME_WATCHDOG} \
    WINEDEBUG="${TRACE:-+snoop}" timeout "$secs" wine "$exe" "$@" > "$log" 2>&1
