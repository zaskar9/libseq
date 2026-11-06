// libseq: MIDI the program sends itself (SeqTxMidiMsg, TTSSEQ 1.01).
//
// Found with the probe (suite midiout): the channel replaces the status's low nibble, port 0xFFFF
// and channel 0xFF mean all of them, and the engine keeps the notes on it sent, per port, channel
// and key. A note off (0x8n, or 0x9n with velocity 0) goes out only for a note that is on, as it
// was given; a second note on first ends the note with a note on of velocity 0. Pitch wheel is
// refused, as are statuses outside 0x80 to 0xEF, channels above 15 and data bytes above 0x7F. A
// port that does not exist sends nothing, but the call succeeds. The thru keeps its notes in the
// same table (suite thru): a note the thru turned on can be ended with SeqTxMidiMsg, and the
// other way round.
#include "libseq/engine.hpp"

#include <array>

namespace libseq {

void Engine::sendTracked(const int port, const std::span<const std::uint8_t> message) {
    if (platform_ == nullptr || port < 0 || port >= platform_->outputPorts() || message.size() < 2) {
        return;
    }
    const unsigned kind = message[0] & 0xf0U;
    const unsigned ch = message[0] & 0x0fU;
    const bool noteOff = kind == 0x80U || (kind == 0x90U && message.size() > 2 && message[2] == 0);
    const bool noteOn = kind == 0x90U && !noteOff;
    const auto key = std::make_tuple(port, static_cast<int>(ch), static_cast<int>(message[1]));
    if (noteOff && notes_.erase(key) == 0) {
        return;
    }
    if (noteOn && !notes_.insert(key).second) {
        const std::array<std::uint8_t, 3> end{static_cast<std::uint8_t>(0x90U | ch), message[1], 0};
        send(port, end);
    }
    send(port, message);
}

bool Engine::txMidiMsg(const std::uint16_t port, const std::uint16_t channel, const std::uint16_t status,
                       const std::uint16_t data1, const std::uint16_t data2) {
    const unsigned kind = status & 0xf0U;
    if (status < 0x80 || status > 0xef || kind == 0xe0U || (channel > 15 && channel != 0xff) || data1 > 0x7f ||
        data2 > 0x7f) {
        return false;
    }
    const int ports = platform_ != nullptr ? platform_->outputPorts() : 0;
    const int firstPort = port == 0xffff ? 0 : port;
    const int lastPort = port == 0xffff ? ports - 1 : port;
    const unsigned firstCh = channel == 0xff ? 0 : channel;
    const unsigned lastCh = channel == 0xff ? 15 : channel;
    const std::size_t size = kind == 0xc0U || kind == 0xd0U ? 2 : 3;
    for (int p = firstPort; p <= lastPort; ++p) {
        for (unsigned ch = firstCh; ch <= lastCh; ++ch) {
            const std::array<std::uint8_t, 3> m{static_cast<std::uint8_t>(kind | ch), static_cast<std::uint8_t>(data1),
                                                static_cast<std::uint8_t>(data2)};
            sendTracked(p, std::span<const std::uint8_t>(m.data(), size));
        }
    }
    return true;
}

} // namespace libseq
