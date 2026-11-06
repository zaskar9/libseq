// libseq: what the engine needs from the system it runs on.
#pragma once

#include <cstdint>
#include <span>

namespace libseq {

/// The MIDI output ports and the clock, which 1.01 has from Windows (`midiOutShortMsg`,
/// `midiOutLongMsg`, `timeGetTime`). While the engine is open the host calls Engine::timer()
/// every Engine::timerPeriod() milliseconds, as 1.01's multimedia timer does, and the engine
/// sends what is due at that moment.
class Platform {
public:
    Platform() = default;
    virtual ~Platform() = default;
    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;
    Platform(Platform&&) = delete;
    Platform& operator=(Platform&&) = delete;

    /// The number of output ports; port numbers run from 0.
    [[nodiscard]] virtual int outputPorts() const = 0;
    /// Sends a message at once: a short message of 1 to 3 bytes, or a sysex message F0 ... F7.
    virtual void send(int port, std::span<const std::uint8_t> message) = 0;
    /// A clock in milliseconds that only goes forward.
    [[nodiscard]] virtual std::uint32_t milliseconds() const = 0;
};

} // namespace libseq
