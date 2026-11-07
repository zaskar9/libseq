// libseq: MIDI input (TTSSEQ 1.01): the thru, and sysex in and out of banks.
//
// Found with the probe (suite thru, with scripted input on the fake port):
//  - the input is echoed with the engine's thru settings (parameters 7 to 12, Engine::thru_): those
//    of a new sequence from SeqOpen on, all of the playing sequence's from SeqSetPlayingSeq on
//    (none with 0), and each one set on the current sequence. They stay when that sequence is
//    destroyed and when a new one becomes current. Mode 0 echoes nothing, 1 (manual) and 2 (auto)
//    every channel message, whether a sequence plays or not, before any sequence exists too;
//  - the port is parameter 8 (a port that does not exist gets nothing); a channel of 0 or more
//    (parameter 9) is ORed into the status's high nibble, so 16 turns a program change into
//    channel pressure, and a status of F0 or above is not sent;
//  - Key+ (10) moves the keys of notes and key pressure, folded into 0 to 127 by octaves; Vel+
//    (11) moves the velocity of note ons, kept within 1 to 127; note offs keep theirs;
//  - nothing goes to the Local On port (12) on the channel the message came in on: the keyboard
//    there plays itself;
//  - the notes go through the table SeqTxMidiMsg keeps (Engine::sendTracked);
//  - sysex and system messages are not echoed; the record filter (28) does not apply.
//
// SeqReceiveSysxBank and SeqSendSysxBank (suites sysexin and misc): ttsseq-api.md, Sysex banks.
#include "libseq/engine.hpp"

#include <algorithm>
#include <array>

namespace libseq {

namespace {

std::uint8_t foldKey(int key) noexcept {
    while (key > 127) {
        key -= 12;
    }
    while (key < 0) {
        key += 12;
    }
    return static_cast<std::uint8_t>(key);
}

} // namespace

void Engine::receive(const std::span<const std::uint8_t> message) {
    if (message.empty() || message[0] < 0x80) {
        return;
    }
    // 1.01 takes the input at its timer's next run: the thru's echoes all lie on the runs, 3 to
    // 13 ms after the input, and a take's ticks move in steps of a period (the probes)
    const std::lock_guard<std::mutex> lock(inboxMutex_);
    inbox_.emplace_back(message.begin(), message.end());
}

void Engine::takeInput() {
    std::vector<std::vector<std::uint8_t>> in;
    {
        const std::lock_guard<std::mutex> lock(inboxMutex_);
        in.swap(inbox_);
    }
    for (std::vector<std::uint8_t>& m : in) {
        if (!m.empty() && m[0] < 0xf0) {
            channelIn_.push_back(std::move(m)); // after the run's playback (takeChannelInput)
        } else if (!m.empty()) {
            input(m);
        }
    }
}

void Engine::takeChannelInput() {
    std::vector<std::vector<std::uint8_t>> in;
    in.swap(channelIn_);
    for (const std::vector<std::uint8_t>& m : in) {
        input(m);
    }
}

void Engine::input(const std::span<const std::uint8_t> message) {
    if (message[0] == 0xf0) {
        receiveSysx(message);
        return;
    }
    if (message[0] > 0xf0) {
        syncInput(message);
        return;
    }
    thru(message);
    record(message);
}

void Engine::receiveSysx(const std::span<const std::uint8_t> message) {
    Sequence* s = sequence(seqOf(sysxReceive_));
    SysxBank* bank = s != nullptr ? s->sysxBank(sysxReceive_) : nullptr;
    if (bank == nullptr || (message.size() > 1 && (message[1] == 0x7e || message[1] == 0x7f))) {
        return; // universal messages are left out
    }
    bank->data.insert(bank->data.end(), message.begin(), message.end());
    sysxReceived_ += static_cast<std::uint32_t>(message.size());
}

std::int16_t Engine::receiveSysxBank(const StreamHandle bank, const SysxReceiveProc& callback) {
    Sequence* s = sequence(seqOf(bank));
    if (s == nullptr || s->sysxBank(bank) == nullptr) {
        return 0;
    }
    sysxReceive_ = bank;
    sysxReceived_ = 0;
    constexpr std::uint32_t kFree = 0x01000000; // GetFreeSpace: what libseq says it has
    for (;;) {
        if (!callback || callback(sysxReceived_, kFree) != 0) {
            break;
        }
        if (platform_ == nullptr) {
            break;
        }
        platform_->wait();
    }
    sysxReceive_ = 0;
    const SysxBank* b = s->sysxBank(bank);
    return b != nullptr && !b->data.empty() ? std::int16_t{1} : std::int16_t{-2};
}

bool Engine::sendSysxBank(const StreamHandle bank) {
    Sequence* s = sequence(seqOf(bank));
    const SysxBank* b = s != nullptr ? s->sysxBank(bank) : nullptr;
    if (b == nullptr) {
        return false;
    }
    constexpr std::size_t kPacket = 1024; // SysxSendPacketSize
    constexpr std::uint32_t kGapMs = 65;
    const std::vector<std::uint8_t> data = b->data;
    for (std::size_t at = 0; at < data.size(); at += kPacket) {
        if (at > 0 && platform_ != nullptr) {
            const std::uint32_t until = platform_->milliseconds() + kGapMs;
            while (platform_->milliseconds() < until) {
                platform_->wait();
            }
        }
        send(0, std::span<const std::uint8_t>(data).subspan(at, std::min(kPacket, data.size() - at)));
    }
    return true;
}

void Engine::thru(const std::span<const std::uint8_t> message) {
    const auto param = [this](const int p) { return thru_[static_cast<std::size_t>(p - qp::kThruMode)]; };
    if (param(qp::kThruMode) == 0) {
        return;
    }
    std::array<std::uint8_t, 3> m{};
    const std::size_t size = std::min<std::size_t>(message.size(), 3);
    std::copy_n(message.begin(), size, m.begin());
    const unsigned kind = m[0] & 0xf0U;
    if (const int ch = param(qp::kThruChannel); ch >= 0) {
        const unsigned status = kind | static_cast<unsigned>(ch);
        if (status >= 0xf0) {
            // channel 16 turns pitch wheel into F0: 1.01 hands that to the driver as a short
            // message, which logs only its status byte (seen in virtual time; Wine's ALSA driver
            // drops it)
            const std::array<std::uint8_t, 1> f{static_cast<std::uint8_t>(status)};
            send(param(qp::kThruPort), f);
            return;
        }
        m[0] = static_cast<std::uint8_t>(status);
    }
    const int port = param(qp::kThruPort);
    if (port == param(qp::kLocalOnPort) && (m[0] & 0x0fU) == (message[0] & 0x0fU)) {
        return; // the keyboard there plays itself
    }
    if (size >= 2 && (kind == 0x80U || kind == 0x90U || kind == 0xa0U)) {
        m[1] = foldKey(m[1] + param(qp::kThruKeyPlus));
    }
    if (size >= 3 && kind == 0x90U && m[2] != 0) {
        m[2] = static_cast<std::uint8_t>(std::clamp(m[2] + param(qp::kThruVelPlus), 1, 127));
    }
    sendTracked(port, std::span<const std::uint8_t>(m.data(), size));
}

} // namespace libseq
