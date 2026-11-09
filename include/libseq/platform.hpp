// libseq: what the engine needs from the system it runs on.
#pragma once

#include <cstdint>
#include <span>
#include <string>

namespace libseq {

class Engine;

/// The [Options] of TTSSEQ.INI that 1.01 reads, as the host keeps them (docs/reverse-engineering/
/// ttsseq-api.md, TTSSEQ.INI). The defaults are 1.01's. The engine asks for them at SeqOpen, and
/// for the timer's period also at SeqSetPlayingSeq and every load, as 1.01 reads its INI file.
struct Options {
    /// SyncOutputPort: the port (from 1) the Song Position goes to. 1.01 sends no other sync.
    int syncOutputPort = 1;
    /// SendLocalOff: Local Control Off on every channel of every port at SeqOpen (0: none).
    int sendLocalOff = 1;
    /// Timebase: 1.01 reads it, and its new sequences stay at 120 ppq all the same.
    int timebase = 120;
    /// TimerPeriod and TimerResolution, in milliseconds (0 counts as 1).
    unsigned timerPeriod = 10;
    unsigned timerResolution = 5;
    /// SysxSendPacketSize: SeqSendSysxBank sends the bank in packets of this size (0: one byte).
    unsigned sysxSendPacketSize = 1024;
    /// SysxReceiveBuffers and SysxReceiveBuffersSize: the input buffers a sysex receive uses.
    unsigned sysxReceiveBuffers = 128;
    unsigned sysxReceiveBuffersSize = 512;
    /// ChaseDelay and FlywheelAmount: MIDI Time Code's chase and run-on, in tenths of a second.
    unsigned chaseDelay = 20;
    unsigned flywheelAmount = 5;
};

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

    /// The number of output ports; port numbers run from 0. They are the devices in the order the
    /// user put them (TTSSEQ.INI's [MIDI Output Devices]; SeqConfigMidi's dialog).
    [[nodiscard]] virtual int outputPorts() const = 0;
    /// The name of the device behind an output port (SeqGetPortName).
    [[nodiscard]] virtual std::string outputName(int port) const {
        static_cast<void>(port);
        return {};
    }
    /// TTSSEQ.INI's [Options].
    [[nodiscard]] virtual Options options() const { return {}; }
    /// Sends a message at once: a short message of 1 to 3 bytes, or a sysex message F0 ... F7.
    virtual void send(int port, std::span<const std::uint8_t> message) = 0;
    /// A clock in milliseconds that only goes forward.
    [[nodiscard]] virtual std::uint32_t milliseconds() const = 0;
    /// SeqOpen and SeqClose: the host starts and stops its input (and may start and stop its
    /// timer here).
    virtual void opened(Engine& engine) { static_cast<void>(engine); }
    virtual void closed() {}
    /// The engine (re)starts its timer: from now on the host calls Engine::timer every
    /// `periodMs` milliseconds, the first time `periodMs` from now (1.01 restarts it at SeqOpen,
    /// SeqSetPlayingSeq and every load). stopTimer: at SeqClose.
    virtual void startTimer(unsigned periodMs) { static_cast<void>(periodMs); }
    virtual void stopTimer() {}
    /// The engine waits in a loop (the count-in, the wait for MIDI sync, a sysex receive) and
    /// calls this on every turn: the host lets a little time pass, during which its timer and
    /// input run.
    virtual void wait() {}
};

} // namespace libseq
