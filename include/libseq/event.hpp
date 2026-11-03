// libseq: the event record.
#pragma once

#include "types.hpp"

#include <array>
#include <cstdint>

namespace libseq {

/// One event as the engine keeps it: the 8-byte record of a `.WRK` stream. A 24-bit time, a status
/// byte (a MIDI status with its channel, 0xF0 for a sysex bank event, or a string event's kind),
/// two data bytes and a 16-bit duration (notes; other kinds keep whatever the field holds).
struct Event {
    Tick time = 0; // 24 bits are kept
    std::uint8_t status = 0;
    std::uint8_t data1 = 0;
    std::uint8_t data2 = 0;
    std::uint16_t duration = 0;

    static constexpr std::size_t kSize = 8;
    static constexpr Tick kMaxTime = 0xffffff;

    [[nodiscard]] std::array<std::uint8_t, kSize> bytes() const noexcept {
        return {static_cast<std::uint8_t>(time & 0xffU),
                static_cast<std::uint8_t>((time >> 8) & 0xffU),
                static_cast<std::uint8_t>((time >> 16) & 0xffU),
                status,
                data1,
                data2,
                static_cast<std::uint8_t>(duration & 0xffU),
                static_cast<std::uint8_t>(duration >> 8)};
    }

    [[nodiscard]] static Event fromBytes(const std::uint8_t* b) noexcept {
        Event e;
        e.time = Tick{b[0]} | (Tick{b[1]} << 8) | (Tick{b[2]} << 16);
        e.status = b[3];
        e.data1 = b[4];
        e.data2 = b[5];
        e.duration = static_cast<std::uint16_t>(b[6] | (b[7] << 8));
        return e;
    }

    [[nodiscard]] bool isNote() const noexcept { return (status & 0xf0U) == 0x90; }

    friend bool operator==(const Event&, const Event&) = default;
};

} // namespace libseq
