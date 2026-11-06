// libseq: external sync (SeqSetSrcClock, and SeqPlayStart with MIDI sync or MIDI Time Code; TTSSEQ
// 1.01).
//
// Found with the probe and scripted input (suites sync and mtc; ttsseq-api.md, External sync).
//
// MIDI sync (source 1). The start sends the streams' parameters and the search-back for Now, then
// waits for Start or Continue, calling back (2, 0, 0) once and (5, 0, 0) over and over. Start
// plays from tick 0, Continue from the last Song Position that came in (kept until then, while
// stopped or playing), else from Now; a Song Position during the wait, and Continue, send the
// search-back for the new position. A Stop during the wait is kept: the playback ends at once.
// Each clock plays the next ppq / 24 ticks at the timer's next run (Player::scheduleClock); the
// tempo map plays no part. Stop ends the playback; SeqIdle reports it.
//
// MIDI Time Code (source 3). The start calls back (3, 0, 0) once and (5, 0, 0) while no code it
// takes comes in. It takes code of the sequence's frame rate once two labels have been read one
// after the other, unless its time after the chase would lie 4096 quarter notes or more after the
// SMPTE offset. Then the streams' parameters, the Song Position and the search-back go out for
// the code's time, and the callback gets (4, seconds * 256 + frames, hours * 256 + minutes) of the
// code until ChaseDelay of code time has gone by and the code has reached the offset; code that
// jumps during the chase is locked to again, with the output again (suite mtc). Now is then
// the first label of the run plus ChaseDelay plus about 0.3 s (kLockLead), through the tempo
// map, and rounded down to a MIDI beat while Song Position is on, as every start. A start that
// gives up sends the stop output. The playback follows the code at every quarter frame and runs
// on FlywheelAmount past the last one; after a jump it runs on kLockLead past where the code
// should have been and ends there.
#include "playback.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <cmath>

namespace libseq {

namespace {

/// Where the code locks, after the chase, in seconds past its first label and ChaseDelay; and
/// where the playback ends after a jump, past where the code should have been. The probes put it
/// at 0.25 to 0.37 s (with the start rounded down to a MIDI beat while Song Position is on).
constexpr double kLockLead = 0.3;

/// The frames a second the code's label counts, by the code's rate (0 to 3).
double nominal(const int rate) noexcept {
    switch (rate) {
    case 0:
        return 24.0;
    case 1:
        return 25.0;
    default:
        return 30.0;
    }
}

/// The code rate of a sequence's frame rate (SeqSetFPS).
int rateOf(const std::uint16_t fps) noexcept {
    switch (fps) {
    case 24:
        return 0;
    case 25:
        return 1;
    case 29:
        return 2;
    default:
        return 3;
    }
}

} // namespace

bool Engine::setSrcClock(const std::uint16_t source) {
    if (source != kSourceInternal && source != kSourceMidi && source != kSourceCode) {
        return false;
    }
    clockSource_ = source;
    return true;
}

double Engine::offsetSeconds(const Sequence& s) {
    const HMSF t = framesToHMSF(s.frameOffset, s.fps);
    return ((t.hours * 60.0 + t.minutes) * 60.0 + t.seconds) + t.frames / nominal(rateOf(s.fps));
}

Tick Engine::codeTick(const Sequence& s, const double seconds) {
    const double t = seconds - offsetSeconds(s);
    if (t <= 0) {
        return 0;
    }
    return s.tempoMap.tickAt(static_cast<std::uint64_t>(std::llround(t * 1e6 * s.ppq)), 0, 1, false);
}

// ---- MIDI sync

bool Engine::startOnClock(Sequence& s, const StartProc& callback) {
    if (!player_->start(s, false, Player::Drive::Clock)) {
        return false;
    }
    clockSync_.waiting = true;
    clockSync_.go = 0;
    clockSync_.stopKept = false;
    bool first = true;
    while (clockSync_.go == 0) {
        if (callback && callback(first ? startproc::kWaitSync : startproc::kWaiting, 0, 0) != 0) {
            clockSync_.waiting = false;
            player_->cancel();
            return false;
        }
        first = false;
        if (platform_ == nullptr) {
            break;
        }
        platform_->wait();
    }
    clockSync_.waiting = false;
    Tick from = 0;
    if (clockSync_.go == 2) {
        from = clockSync_.position.value_or(s.now);
        player_->searchBackFor(from);
    }
    clockSync_.position.reset();
    s.setParam(qp::kNowJumped, 1);
    player_->go(from);
    if (clockSync_.stopKept) {
        player_->externalStop();
    }
    return true;
}

void Engine::syncInput(const std::span<const std::uint8_t> message) {
    if (clockSource_ == kSourceCode && message[0] == 0xf1 && message.size() > 1) {
        quarterFrame(message[1]);
        return;
    }
    if (clockSource_ != kSourceMidi) {
        return; // on the internal clock, MIDI sync is ignored
    }
    Sequence* s = sequence(current_);
    switch (message[0]) {
    case 0xf8:
        player_->clock();
        break;
    case 0xfa:
    case 0xfb:
        if (clockSync_.waiting && clockSync_.go == 0) {
            clockSync_.go = message[0] == 0xfa ? 1 : 2;
        }
        break;
    case 0xfc:
        if (clockSync_.waiting) {
            clockSync_.stopKept = true;
        } else {
            player_->externalStop();
        }
        break;
    case 0xf2:
        if (message.size() > 2 && s != nullptr) {
            const Tick sixteenths = message[1] | (Tick{message[2]} << 7);
            clockSync_.position = sixteenths * (s->ppq / 4U);
            if (clockSync_.waiting) {
                player_->searchBackFor(*clockSync_.position);
            }
        }
        break;
    default:
        break;
    }
}

// ---- MIDI Time Code

void Engine::quarterFrame(const std::uint8_t data) {
    CodeSync& c = code_;
    const int piece = data >> 4;
    const auto value = static_cast<std::uint8_t>(data & 0x0fU);
    if (piece == 0) {
        c.expect = 0;
    }
    if (piece != c.expect) {
        c.expect = -1; // out of order: wait for the next group
        return;
    }
    c.pieces.at(static_cast<std::size_t>(piece)) = value;
    ++c.expect;
    if (piece == 7) {
        const auto& p = c.pieces;
        const int rate = (p[7] >> 1) & 3;
        const double label = ((((p[7] & 1) << 4) | p[6]) * 60.0 + ((p[5] << 4) | p[4])) * 60.0 + ((p[3] << 4) | p[2]) +
                             ((p[1] << 4) | p[0]) / nominal(rate);
        // the label this group should say: two frames after the last one
        const double next = c.label + 2.0 / nominal(c.rate);
        const bool expected = c.known && rate == c.rate && std::abs(label - next) < 0.5 / nominal(rate);
        c.jumped = c.known && !expected;
        if (c.jumped) {
            c.jumpFrom = next; // where the code should have been
        }
        if (expected) {
            ++c.run;
        } else {
            c.run = 1;
            c.first = label;
        }
        c.known = true;
        c.rate = rate;
        c.label = label;
    }
    if (!c.known) {
        return;
    }
    // the group's label is the last one's plus two frames until its eighth quarter frame says it
    const double base = piece == 7 ? c.label : c.label + 2.0 / nominal(c.rate);
    c.time = base + piece / (4.0 * nominal(c.rate));
    // following: every quarter frame moves the playback to the code; a jump ends it
    const Sequence* s = player_->sequence();
    if (!c.following || s == nullptr || !player_->playing()) {
        return;
    }
    if (c.jumped) {
        c.following = false;
        Tick at = codeTick(*s, c.jumpFrom + kLockLead);
        if (s->param(qp::kSendSpp) != 0 && s->ppq >= 4) {
            at -= at % (s->ppq / 4U);
        }
        player_->runOut(at);
        return;
    }
    player_->follow(codeTick(*s, c.time), codeTick(*s, c.time + flywheel_ / 10.0));
}

bool Engine::startOnCode(Sequence& s, const StartProc& callback) {
    if (!player_->start(s, false, Player::Drive::Code)) {
        return false;
    }
    code_ = CodeSync{};
    const double chase = chaseDelay_ / 10.0;
    const double offset = offsetSeconds(s);
    // code is taken once two labels in a row were read, of the sequence's rate, and not too far
    const auto takes = [&]() {
        if (code_.run < 2 || code_.rate != rateOf(s.fps)) {
            return false;
        }
        const Tick at = codeTick(s, code_.first + chase + kLockLead);
        return at / std::max<Tick>(s.ppq, 1) < 4096;
    };
    if (callback && callback(startproc::kWaitCode, 0, 0) != 0) {
        player_->cancel();
        return false;
    }
    bool locked = false;
    bool chasing = false; // locked once: the callback says so from then on
    double lockTime = 0;
    for (;;) {
        if (locked && code_.run < 2) {
            locked = false; // the code jumped during the chase: it locks again to the new code
        }
        if (!locked && takes()) {
            locked = true;
            chasing = true;
            lockTime = code_.time;
            player_->lockOutput(codeTick(s, code_.time));
        }
        if (locked && code_.time >= std::max(lockTime + chase, offset)) {
            break;
        }
        std::uint32_t r = 0;
        if (chasing) {
            const double t = code_.time;
            const auto whole = static_cast<unsigned>(t);
            const auto frames = static_cast<unsigned>((t - whole) * nominal(code_.rate));
            const unsigned h = whole / 3600;
            const unsigned m = whole / 60 % 60;
            r = callback ? callback(startproc::kLocking, static_cast<std::uint16_t>((whole % 60) * 256 + frames),
                                    static_cast<std::uint16_t>(h * 256 + m))
                         : 0;
        } else if (callback) {
            r = callback(startproc::kWaiting, 0, 0);
        }
        if (r != 0) {
            player_->cancel();
            return false;
        }
        if (platform_ == nullptr) {
            player_->cancel();
            return false;
        }
        platform_->wait();
    }
    s.setParam(qp::kNowJumped, 1);
    Tick at = codeTick(s, code_.first + chase + kLockLead);
    if (s.param(qp::kSendSpp) != 0 && s.ppq >= 4) {
        at -= at % (s.ppq / 4U); // as every start with Song Position, on a MIDI beat
    }
    player_->go(at);
    code_.following = true;
    player_->follow(s.now, codeTick(s, code_.time + flywheel_ / 10.0));
    return true;
}

} // namespace libseq
