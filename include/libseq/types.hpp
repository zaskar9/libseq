// libseq: types shared by the whole library.
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace libseq {

/// Time in ticks. The engine keeps 32-bit times; an event record holds 24 bits of them.
using Tick = std::uint32_t;

/// A sequence handle: a small number, the lowest free one (1 for the first sequence).
using SeqHandle = std::uint16_t;

/// A stream handle: the sequence in the high byte, the stream's id in the low byte
/// (0x0100 is stream 0 of sequence 1). The id is an allocation number, not the track position.
using StreamHandle = std::uint16_t;

[[nodiscard]] constexpr StreamHandle makeStreamHandle(const SeqHandle seq, const std::uint8_t id) noexcept {
    return static_cast<StreamHandle>((seq << 8) | id);
}
[[nodiscard]] constexpr SeqHandle seqOf(const StreamHandle stream) noexcept {
    return static_cast<SeqHandle>(stream >> 8);
}
[[nodiscard]] constexpr std::uint8_t idOf(const StreamHandle stream) noexcept {
    return static_cast<std::uint8_t>(stream & 0xffU);
}

/// Error raised for malformed input and invalid operations.
class Error : public std::runtime_error {
public:
    explicit Error(const std::string& what) : std::runtime_error(what) {}
};

} // namespace libseq
