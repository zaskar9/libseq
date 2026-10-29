# env.sh: settings shared by the ttsprobe scripts; sourced, not run.
#   TTSPROBE        this folder (tools/ttsprobe)
#   TTSPROBE_WORK   where the Wine prefix, the built helpers and the scratch results live
#                   (default ~/ttsprobe-work); setup.sh creates it
#   WINEPREFIX      the Wine prefix (default $TTSPROBE_WORK/prefix); the DLLs are in
#                   C:\PROBE\V101, V210 and V300 inside it ($PROBE_C/V101 ...)
#   FAKESEQ_LIB     the fake ALSA sequencer (default $TTSPROBE_WORK/libfakeseq.so)
#   TTSSEQ_DATA     the oracle sets (default tests/data/ttsseq of this repository)
#   SONGS           the test songs (default tests/data/songs); INPUTS the scripted MIDI input
TTSPROBE=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO=$(cd "$TTSPROBE/../.." && pwd)
TTSPROBE_WORK=${TTSPROBE_WORK:-$HOME/ttsprobe-work}
export WINEPREFIX=${WINEPREFIX:-$TTSPROBE_WORK/prefix}
PROBE_C=$WINEPREFIX/drive_c/PROBE
FAKESEQ_LIB=${FAKESEQ_LIB:-$TTSPROBE_WORK/libfakeseq.so}
TTSSEQ_DATA=${TTSSEQ_DATA:-$REPO/tests/data/ttsseq}
SONGS=${SONGS:-$REPO/tests/data/songs}
INPUTS=$REPO/tests/data/ttsseq/input     # scripted MIDI input for the suites that record
EXPORTS=$REPO/docs/reverse-engineering/exports
RUNCW=$TTSPROBE/wine/runcw.sh
# the DLL file name and module name of a version (101, 210, 300)
ttsseq_dll() { [ "$1" = 300 ] && echo TTSSEQ30.DLL || echo TTSSEQ.DLL; }
ttsseq_mod() { [ "$1" = 300 ] && echo TTSSEQ30 || echo TTSSEQ; }
