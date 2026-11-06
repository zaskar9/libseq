// libseq: what the engine needs from the system it runs on.
#pragma once

#include <cstdint>
#include <span>

namespace libseq {

class Engine;

/// The MIDI ports and the clock, which 1.01 has from Windows (`midiOutShortMsg`,
/// `midiOutLongMsg`, `midiInOpen`, `timeGetTime`). While the engine is open the host calls
/// Engine::timer() every Engine::timerPeriod() milliseconds, as 1.01's multimedia timer does, and
/// the engine sends what is due at that moment; and it hands every message that comes in to
/// Engine::receive(), as 1.01's MIDI input callback gets it. Both may interrupt the program
/// wherever it is (on Windows 3.1 they run at interrupt time); the engine does not lock.
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
    /// SeqOpen and SeqClose: the host starts and stops its input (and may start and stop its
    /// timer here).
    virtual void opened(Engine& engine) { static_cast<void>(engine); }
    virtual void closed() {}
    /// The engine waits in a loop (the count-in, the wait for MIDI sync, a sysex receive) and
    /// calls this on every turn: the host lets a little time pass, during which its timer and
    /// input run.
    virtual void wait() {}
};

} // namespace libseq
