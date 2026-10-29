#!/bin/bash
# build-mmsystem16.sh OUTDIR: builds Wine's 16-bit MMSYSTEM (mmsystem.dll16) with
# mmsystem16-midiin.patch applied, for the Wine version installed, and writes it to OUTDIR.
#
# Wine 9.0 does not map the long buffers of 16-bit MIDI input (midiInPrepareHeader,
# midiInAddBuffer and midiInUnprepareHeader return an error), so a 16-bit program cannot receive
# system exclusive messages. The patch maps them the way Wine already maps those of MIDI output.
# It changes Wine only; TTSSEQ needs it for SeqReceiveSysxBank (the sysexin suite).
#
# Needs: git, flex, bison, gcc and the MinGW cross compiler for i686 (i686-w64-mingw32-gcc).
# Install the result over Wine's own module (keep the original):
#   sudo cp /usr/lib/i386-linux-gnu/wine/i386-windows/mmsystem.dll16{,.orig}
#   sudo cp OUTDIR/mmsystem.dll16 /usr/lib/i386-linux-gnu/wine/i386-windows/mmsystem.dll16
set -e
out=$(mkdir -p "${1:?usage: build-mmsystem16.sh OUTDIR}" && cd "$1" && pwd)
here=$(cd "$(dirname "$0")" && pwd)
ver=$(wine --version | sed 's/^wine-\([0-9.]*\).*/\1/')
work=$out/wine-build; mkdir -p "$work"; cd "$work"
[ -d src ] || git clone --depth 1 --branch "wine-$ver" https://github.com/wine-mirror/wine.git src
git -C src apply --check "$here/mmsystem16-midiin.patch" 2>/dev/null && git -C src apply "$here/mmsystem16-midiin.patch"
mkdir -p build; cd build
[ -f Makefile ] || ../src/configure --enable-archs=i386 --disable-tests --without-x --without-freetype \
  --without-alsa --without-pulse --without-gstreamer --without-vulkan --without-opengl --without-gnutls \
  --without-cups --without-dbus --without-sane --without-usb --without-v4l2 --without-krb5 \
  --without-netapi --without-pcap --without-sdl --without-udev --without-unwind --without-oss \
  --without-capi --without-gphoto --without-inotify --without-pcsclite --without-wayland \
  --without-osmesa --without-xcomposite > configure.log
make -j"$(nproc)" dlls/mmsystem.dll16/i386-windows/mmsystem.dll16 > make.log
cp dlls/mmsystem.dll16/i386-windows/mmsystem.dll16 "$out/"
echo "built: $out/mmsystem.dll16 (Wine $ver with mmsystem16-midiin.patch)"
