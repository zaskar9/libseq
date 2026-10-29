#!/bin/bash
# setup.sh DLLDIR: prepares $TTSPROBE_WORK (see env.sh) for observing the three TTSSEQ versions.
#
# DLLDIR holds the engines from your own Cakewalk disks, renamed by version:
#   TTSSEQ101.DLL  TTSSEQ.DLL of the Cakewalk Professional for Windows demo (1992), disk CWDEMO1
#   TTSSEQ210.DLL  TTSSEQ.DLL of Cakewalk Apprentice for Windows 1.04
#   TTSSEQ300.DLL  TTSSEQ30.DLL of Cakewalk Professional for Windows 2.00, disk 1
# On the disks they are compressed (TTSSEQ.DL_, TTSSEQ30.DL_): copy them off the images with
# mcopy and expand them with msexpand from libmspack. They are not part of this repository.
#
# Needs: Wine with 32-bit support (wine32:i386), gcc with -m32, Python 3, and Open Watcom v2 for
# the probe (WATCOM set, its binl64 on PATH). Run the programs on an X display (Xvfb :99).
#
# What it does: creates the Wine prefix, turns off Wine's crash debugger (a crashing call should
# end the run, not wait for a debugger), copies the DLLs to C:\PROBE\V101, V210 and V300 with a
# TTSSEQ.INI naming the fake MIDI ports, copies the test songs to C:\PROBE, builds libfakeseq.so and ttsprobe.exe, and checks that
# Wine's ALSA driver reports a usable manufacturer ID (wine/patch-winealsa.py; needs root to
# install, see the message).
set -e
. "$(dirname "$0")/env.sh"
src=${1:?usage: setup.sh DLLDIR}
mkdir -p "$TTSPROBE_WORK"
if [ ! -d "$WINEPREFIX/drive_c" ]; then WINEARCH=win32 wineboot -i > /dev/null 2>&1; wineserver -w; fi
wine reg add 'HKLM\Software\Microsoft\Windows NT\CurrentVersion\AeDebug' /v Auto /t REG_SZ /d 1 /f > /dev/null 2>&1
wine reg add 'HKLM\Software\Microsoft\Windows NT\CurrentVersion\AeDebug' /v Debugger /t REG_SZ /d '' /f > /dev/null 2>&1
wineserver -w
for v in 101 210 300; do
  d=$PROBE_C/V$v; mkdir -p "$d"
  cp "$src/TTSSEQ$v.DLL" "$d/$(ttsseq_dll $v)"
  printf '[Options]\r\nSyncOutputPort=1\r\n[MIDI Output Devices]\r\n0=Fake - MIDI Out\r\n[MIDI Input Devices]\r\n0=Fake - MIDI In\r\n' > "$d/TTSSEQ.INI"
done
cp "$SONGS"/*/*.WRK "$PROBE_C"/   # the test songs, for the sweep scripts (the oracle suites copy their own)
gcc -m32 -shared -fPIC -O2 -pthread -o "$FAKESEQ_LIB" "$TTSPROBE/wine/fakeseq.c"
if command -v wcl > /dev/null; then
  (cd "$TTSPROBE_WORK" && wcl -q -bt=windows -l=windows -ml -zW -fe=ttsprobe.exe "$TTSPROBE/probe/ttsprobe.c")
  for v in 101 210 300; do cp "$TTSPROBE_WORK/ttsprobe.exe" "$PROBE_C/V$v/"; done
else
  echo "Open Watcom (wcl) not found: build probe/ttsprobe.c and copy ttsprobe.exe to $PROBE_C/V101, V210, V300"
fi
alsa=$(dirname "$(command -v wine)")/../lib/i386-linux-gnu/wine/i386-unix/winealsa.so
[ -f "$alsa" ] || alsa=/usr/lib/i386-linux-gnu/wine/i386-unix/winealsa.so
if python3 -c "import sys; b=open(sys.argv[1],'rb').read(); sys.exit(0 if b.find(bytes.fromhex('c74722ff000100'))>=0 else 1)" "$alsa"; then
  echo "Wine's $alsa reports manufacturer 255, which 2.10 and 3.00 reject. Patch a copy and install it:"
  echo "  python3 $TTSPROBE/wine/patch-winealsa.py $alsa $TTSPROBE_WORK/winealsa.so"
  echo "  sudo cp $alsa $alsa.orig && sudo cp $TTSPROBE_WORK/winealsa.so $alsa"
fi
echo "ready: $TTSPROBE_WORK"
