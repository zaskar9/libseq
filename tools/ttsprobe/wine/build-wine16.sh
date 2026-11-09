#!/bin/bash
# build-wine16.sh OUTDIR: builds three of Wine's modules with this folder's patches, for the Wine
# version installed, and writes them to OUTDIR:
#
# - mmsystem.dll16 with mmsystem16-midiin.patch. Wine 9.0 does not map the long buffers of 16-bit
#   MIDI input (midiInPrepareHeader, midiInAddBuffer and midiInUnprepareHeader return an error), so
#   a 16-bit program cannot receive system exclusive messages. The patch maps them the way Wine
#   already maps those of MIDI output. TTSSEQ needs it for SeqReceiveSysxBank (the sysexin suite).
# - and mmsystem16-vtime.patch on top: virtual time (WINE_VTIME=1). The module's multimedia timer,
#   clock and MIDI devices then run on a millisecond clock that moves only where the probe waits
#   (VTIMEADVANCE, MMSYSTEM.2000), MIDI input comes from a script at its virtual time, and what
#   goes out is logged in the fake port's format. A run gives the same output to the millisecond
#   every time, which is what the timing of libseq is checked against (docs/ttsseq-api.md,
#   Timing). VTIME_WATCHDOG moves the clock on in waits that call nothing (needs the irq patch).
#   Unset, the module behaves as before.
# - krnl386.exe16 with krnl386-irq.patch. Windows 3.1 runs the callbacks of multimedia timers and
#   MIDI drivers at interrupt time, while the program runs; Wine runs them only when no thread is in
#   16-bit code. A program that waits in a loop for such a callback (TTSSEQ 1.01 waiting for MIDI
#   Start) then never sees it. With WINE_WIN16_IRQ_CALLBACKS=serial the patched module runs them
#   at once, one at a time, while the program's 16-bit code stands still, and lets 16-bit
#   cli ... sti keep them out, as disabling interrupts does (the sync suite, "#! irq"). Unset,
#   Wine behaves as before.
# - user32.dll with user32-dlgproc.patch. When a 16-bit program sets DWL_DLGPROC to 0 and back
#   (Cakewalk Professional's dialogs do it while handling WM_SETFONT), Wine 9.0 hands it back a
#   value that stands for "some 16-bit procedure" instead of the procedure, and the next message
#   to the dialog jumps to a bad address. The demo's Punch-in and Auto-shuttle dialogs crashed.
#
# All three change Wine only. Needs: git, flex, bison, gcc and the MinGW cross compiler for i686
# (i686-w64-mingw32-gcc). Install the results over Wine's own modules (keep the originals):
#   W=/usr/lib/i386-linux-gnu/wine/i386-windows
#   sudo cp $W/mmsystem.dll16 $W/mmsystem.dll16.orig; sudo cp OUTDIR/mmsystem.dll16 $W/
#   sudo cp $W/krnl386.exe16 $W/krnl386.exe16.orig; sudo cp OUTDIR/krnl386.exe16 $W/
#   sudo cp $W/user32.dll $W/user32.dll.orig; sudo cp OUTDIR/user32.dll $W/
set -e
out=$(mkdir -p "${1:?usage: build-wine16.sh OUTDIR}" && cd "$1" && pwd)
here=$(cd "$(dirname "$0")" && pwd)
ver=$(wine --version | sed 's/^wine-\([0-9.]*\).*/\1/')
work=$out/wine-build; mkdir -p "$work"; cd "$work"
[ -d src ] || git clone --depth 1 --branch "wine-$ver" https://github.com/wine-mirror/wine.git src
for p in mmsystem16-midiin.patch mmsystem16-vtime.patch krnl386-irq.patch user32-dlgproc.patch; do
  git -C src apply --check "$here/$p" 2>/dev/null && git -C src apply "$here/$p"
done
mkdir -p build; cd build
[ -f Makefile ] || ../src/configure --enable-archs=i386 --disable-tests --without-x --without-freetype \
  --without-alsa --without-pulse --without-gstreamer --without-vulkan --without-opengl --without-gnutls \
  --without-cups --without-dbus --without-sane --without-usb --without-v4l2 --without-krb5 \
  --without-netapi --without-pcap --without-sdl --without-udev --without-unwind --without-oss \
  --without-capi --without-gphoto --without-inotify --without-pcsclite --without-wayland \
  --without-osmesa --without-xcomposite > configure.log
make -j"$(nproc)" dlls/mmsystem.dll16/i386-windows/mmsystem.dll16 dlls/krnl386.exe16/i386-windows/krnl386.exe16 \
  dlls/user32/i386-windows/user32.dll > make.log
cp dlls/mmsystem.dll16/i386-windows/mmsystem.dll16 dlls/krnl386.exe16/i386-windows/krnl386.exe16 \
  dlls/user32/i386-windows/user32.dll "$out/"
echo "built: $out/mmsystem.dll16, krnl386.exe16 and user32.dll (Wine $ver with this folder's patches)"
