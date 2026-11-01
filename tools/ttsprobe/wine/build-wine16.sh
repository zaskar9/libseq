#!/bin/bash
# build-wine16.sh OUTDIR: builds two of Wine's 16-bit modules with this folder's patches, for the
# Wine version installed, and writes them to OUTDIR:
#
# - mmsystem.dll16 with mmsystem16-midiin.patch. Wine 9.0 does not map the long buffers of 16-bit
#   MIDI input (midiInPrepareHeader, midiInAddBuffer and midiInUnprepareHeader return an error), so
#   a 16-bit program cannot receive system exclusive messages. The patch maps them the way Wine
#   already maps those of MIDI output. TTSSEQ needs it for SeqReceiveSysxBank (the sysexin suite).
# - krnl386.exe16 with krnl386-irq.patch. Windows 3.1 runs the callbacks of multimedia timers and
#   MIDI drivers at interrupt time, while the program runs; Wine runs them only when no thread is in
#   16-bit code. A program that waits in a loop for such a callback (TTSSEQ 1.01 waiting for MIDI
#   Start) then never sees it. With WINE_WIN16_IRQ_CALLBACKS=serial the patched module runs them
#   at once, one at a time, and lets 16-bit cli ... sti keep them out, as disabling interrupts
#   does (the sync suite, "#! irq"). Unset, Wine behaves as before.
#
# Both change Wine only. Needs: git, flex, bison, gcc and the MinGW cross compiler for i686
# (i686-w64-mingw32-gcc). Install the results over Wine's own modules (keep the originals):
#   W=/usr/lib/i386-linux-gnu/wine/i386-windows
#   sudo cp $W/mmsystem.dll16 $W/mmsystem.dll16.orig; sudo cp OUTDIR/mmsystem.dll16 $W/
#   sudo cp $W/krnl386.exe16 $W/krnl386.exe16.orig; sudo cp OUTDIR/krnl386.exe16 $W/
set -e
out=$(mkdir -p "${1:?usage: build-wine16.sh OUTDIR}" && cd "$1" && pwd)
here=$(cd "$(dirname "$0")" && pwd)
ver=$(wine --version | sed 's/^wine-\([0-9.]*\).*/\1/')
work=$out/wine-build; mkdir -p "$work"; cd "$work"
[ -d src ] || git clone --depth 1 --branch "wine-$ver" https://github.com/wine-mirror/wine.git src
for p in mmsystem16-midiin.patch krnl386-irq.patch; do
  git -C src apply --check "$here/$p" 2>/dev/null && git -C src apply "$here/$p"
done
mkdir -p build; cd build
[ -f Makefile ] || ../src/configure --enable-archs=i386 --disable-tests --without-x --without-freetype \
  --without-alsa --without-pulse --without-gstreamer --without-vulkan --without-opengl --without-gnutls \
  --without-cups --without-dbus --without-sane --without-usb --without-v4l2 --without-krb5 \
  --without-netapi --without-pcap --without-sdl --without-udev --without-unwind --without-oss \
  --without-capi --without-gphoto --without-inotify --without-pcsclite --without-wayland \
  --without-osmesa --without-xcomposite > configure.log
make -j"$(nproc)" dlls/mmsystem.dll16/i386-windows/mmsystem.dll16 dlls/krnl386.exe16/i386-windows/krnl386.exe16 > make.log
cp dlls/mmsystem.dll16/i386-windows/mmsystem.dll16 dlls/krnl386.exe16/i386-windows/krnl386.exe16 "$out/"
echo "built: $out/mmsystem.dll16 and $out/krnl386.exe16 (Wine $ver with this folder's patches)"
