// The engine interfaces the script runner can put behind the probe's calls.
#pragma once

#include "probe.hpp"

namespace libseq {
class Engine;
}

#include <cstring>
#include <string>

namespace ttsrun {

/// TTSSEQ.DLL 1.01.
void defineApi101(Probe& p, libseq::Engine& e);
/// Its event cursor and edit primitives (api101_events.cpp).
void defineEvents101(Probe& p, libseq::Engine& e);
/// Its time base, tempo and meter maps and timecode functions (api101_time.cpp).
void defineTime101(Probe& p, libseq::Engine& e);

/// Writes a little-endian word or double word into the probe's memory, if the pointer leads there.
inline void putWord(Probe& p, const FarPtr at, const std::uint16_t v) {
    if (std::uint8_t* b = p.mem.at(at, 2)) {
        b[0] = static_cast<std::uint8_t>(v & 0xffU);
        b[1] = static_cast<std::uint8_t>(v >> 8);
    }
}
inline void putDword(Probe& p, const FarPtr at, const std::uint32_t v) {
    if (std::uint8_t* b = p.mem.at(at, 4)) {
        for (int i = 0; i < 4; ++i) {
            b[i] = static_cast<std::uint8_t>((v >> (8 * i)) & 0xffU);
        }
    }
}
/// Writes a string and its terminating 0.
inline void putString(Probe& p, const FarPtr at, const std::string& s) {
    if (std::uint8_t* b = p.mem.at(at, s.size() + 1)) {
        std::memcpy(b, s.c_str(), s.size() + 1);
    }
}
/// A far pointer as a function result: the segment in DX, the offset in AX.
inline std::uint32_t farResult(const FarPtr at) {
    return (std::uint32_t{at.seg} << 16) | at.off;
}

} // namespace ttsrun
