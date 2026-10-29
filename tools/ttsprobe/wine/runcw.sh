#!/bin/bash
# runcw.sh DIR EXE LOG SECONDS [ARGS...]: runs a 16-bit Windows program in DIR under Wine with the
# fake MIDI ports and Wine's call trace, in a minimal environment (the 16-bit C run-times of the
# Cakewalk programs reject a large one). The trace goes to LOG.
#   TRACE           WINEDEBUG channels (default +snoop: every call into a 16-bit DLL)
#   FAKESEQ_LOG     where the fake output port writes the MIDI it receives (default /dev/null)
#   FAKESEQ_INPUT   a script of timed messages for the fake input port; FAKESEQ_INPUT_GO a file
#                   whose appearance starts it (see fakeseq.c)
#   DISPLAY         the X display (default :99, for example Xvfb)
. "$(dirname "$0")/../env.sh"
dir=$1; exe=$2; log=$3; secs=$4; shift 4
cd "$dir" || exit 1
env -i HOME="$HOME" PATH=/usr/bin:/bin DISPLAY="${DISPLAY:-:99}" WINEPREFIX="$WINEPREFIX" \
    LD_PRELOAD="$FAKESEQ_LIB" FAKESEQ_LOG="${FAKESEQ_LOG:-/dev/null}" \
    ${FAKESEQ_INPUT:+FAKESEQ_INPUT=$FAKESEQ_INPUT} ${FAKESEQ_INPUT_GO:+FAKESEQ_INPUT_GO=$FAKESEQ_INPUT_GO} \
    WINEDEBUG="${TRACE:-+snoop}" timeout "$secs" wine "$exe" "$@" > "$log" 2>&1
