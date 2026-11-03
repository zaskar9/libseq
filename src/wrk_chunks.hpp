// libseq: the chunk types of a Cakewalk `.WRK` file (not installed).
#pragma once

#include <cstdint>

namespace libseq::wrk {

enum ChunkType : std::uint8_t {
    TRACK = 1,
    STREAM = 2,
    VARS = 3,
    TEMPO = 4,
    METER = 5,
    SYSEX = 6,
    MEMRGN = 7,
    COMMENTS = 8,
    TRKOFFS = 9,
    TIMEBASE = 10,
    TIMEFMT = 11,
    TRKREPS = 12,
    TRKPATCH = 14,
    NTEMPO = 15,
    THRU = 16,
    METRONOME = 17,
    STRSTREAM = 18,
    TRKVOL = 19,
    SYSEX2 = 20,
    MARKERS = 21,
    STRTAB = 22,
    METERKEY = 23,
    TRKNAME = 24,
    VARIABLE = 26,
    NTRKOFS = 27,
    TRKBANK = 30,
    NTRACK = 36,
    NSYSEX = 44,
    NSTREAM = 45,
    SGMNT = 49,
    SOFTVER = 74,
    END = 255,
};

} // namespace libseq::wrk
