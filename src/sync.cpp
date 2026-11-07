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
// MIDI Time Code (source 3; the timing in virtual time, ttsseq-api.md, Timing; suites mtc and
// mtcbreaks). The start calls back (3, 0, 0) once and (5, 0, 0) while no code it takes comes in.
// Input is taken at the timer's runs. The decoder counts the code in two-frame groups: its time
// in quarter frames is 8 * G + 4 * b + piece, where G is the group it expects and b the low bit
// of the frames of the last label read, taken over at piece 1. G goes up by one at a piece 0 that
// follows a piece 7; a piece 0 inside a group moves G to the nearest group start (by one from
// piece 4 on) from its piece 1 on, the piece 0 itself counting one quarter frame on. A label
// (piece 7) is compared with G at group resolution only. The code is taken once two labels in a
// row were read, of the sequence's frame rate, unless its time would lie 4096 quarter notes or
// more past the SMPTE offset. At the run that takes the next quarter frame the streams'
// parameters, the Song Position of the start ahead and its search-back go out, and the callback
// gets (4, seconds * 256 + frames, hours * 256 + minutes) of the code from then on. The chase ends
// at the run whose first quarter frame lies ChaseDelay of code after the second label, once the
// code has reached the offset (code that jumps meanwhile is taken again); the start call returns
// then, with Now one MIDI beat past the code's time rounded down to a MIDI beat. Following: at
// each run that takes quarter frames, the first of them sets the position (in ticks, rounded to
// the nearest); after the run the clock runs on from the last one, and the playback stops when it
// passes the last code's time plus FlywheelAmount. A label of another group is a jump: the decoder
// goes on with the group it expects, and the playback stops at the run that takes the 24th
// quarter frame after the jump group began, after what that run plays; while following, the
// label of a group that began inside another is not read. A start that gives up sends the stop
// output.
#include "playback.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <cmath>

namespace libseq {

namespace {

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

// ---- MIDI sync

bool Engine::startOnClock(Sequence& s, const StartProc& callback) {
    if (!player_->start(s, false, Player::Drive::Clock)) {
        return false;
    }
    clockSync_.waiting = true;
    clockSync_.go = 0;
    clockSync_.held = 0;
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
    player_->clocks(clockSync_.held, true);
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
        if (clockSync_.waiting) {
            if (clockSync_.go != 0) {
                ++clockSync_.held; // taken with the Start: they play with the next clock
            }
        } else {
            player_->clocks(1, false);
        }
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

double Engine::codeTicks(const Sequence& s, const double seconds) {
    const double t = seconds - offsetSeconds(s);
    if (t <= 0) {
        return 0;
    }
    const double units = t * 1e6 * s.ppq;
    const Tick base = s.tempoMap.tickAt(static_cast<std::uint64_t>(units), 0, 1, false);
    const double tick = 6e9 / std::max<double>(s.tempoMap.tempoAt(base), 1.0);
    return static_cast<double>(base) + (units - static_cast<double>(s.tempoMap.elapsedAt(base))) / tick;
}

void Engine::quarterFrame(const std::uint8_t data) {
    CodeSync& c = code_;
    const int piece = data >> 4;
    std::int64_t counted = -1; // a piece 0's time: one quarter frame after the last
    if (piece == 0) {
        // a group begins; one that begins inside another (after its pieces 0 to expect - 1) moves
        // the group on to the nearest group start (by one from piece 4 on), from its piece 1 on
        c.inside = c.expect >= 1 && c.expect <= 7;
        if (c.known && c.expect >= 1) {
            counted = 8 * c.group + 4 * std::int64_t{c.bit} + c.expect;
        }
        if (c.known && (!c.inside || c.expect >= 4)) {
            ++c.group;
        }
        c.expect = 0;
    }
    if (piece != c.expect) {
        c.expect = -1; // out of order: wait for the next group
        return;
    }
    c.pieces.at(static_cast<std::size_t>(piece)) = static_cast<std::uint8_t>(data & 0x0fU);
    ++c.expect;
    if (c.known && piece == 1) {
        c.bit = c.parity;
    }
    if (piece == 7) {
        const auto& p = c.pieces;
        const int rate = (p[7] >> 1) & 3;
        const auto fps = static_cast<std::int64_t>(nominal(rate));
        const std::int64_t frames =
            ((((p[7] & 1) << 4 | p[6]) * 60LL + (p[5] << 4 | p[4])) * 60 + (p[3] << 4 | p[2])) * fps +
            (p[1] << 4 | p[0]);
        if (!c.known || rate != c.rate) {
            c.known = true;
            c.rate = rate;
            c.group = frames >> 1;
            c.parity = c.bit = static_cast<int>(frames & 1);
            c.run = 1;
        } else if (c.jumpAt >= 0 || (c.following && c.inside)) {
            // after a jump the decoder goes on with the group it expected; while following, the
            // label of a group that began inside another is not read
        } else if (frames >> 1 == c.group) {
            c.parity = static_cast<int>(frames & 1);
            ++c.run;
        } else {
            c.parity = static_cast<int>(frames & 1);
            if (c.following) {
                c.jumpAt = c.count - 7; // the jump group's piece 0
            } else {
                c.group = frames >> 1; // during the chase it is taken again
                c.run = 1;
            }
        }
    }
    const std::int64_t qf = c.count++;
    if (!c.known) {
        return;
    }
    c.time = static_cast<double>(counted >= 0 ? counted : 8 * c.group + 4 * std::int64_t{c.bit} + piece) /
             (4.0 * nominal(c.rate));
    if (piece == 7) {
        c.label = c.time;
        c.labelAt = qf;
    }
    if (!c.inRun) {
        c.inRun = true;
        c.first = c.time;
    }
    const Sequence* s = player_->sequence();
    if (!c.following || s == nullptr || !player_->playing()) {
        return;
    }
    // the first quarter frame of the run moves the playback; the clock goes on from the last
    const double at = codeTicks(*s, c.time);
    player_->follow(static_cast<Tick>(std::floor(at + 0.5)), codeTicks(*s, c.time + flywheel_ / 10.0), !codeTaken_, at);
    codeTaken_ = true;
    if (c.jumpAt >= 0 && qf == c.jumpAt + 24) {
        c.following = false;
        player_->codeStop(); // the run still plays up to where the code stands
    }
}

bool Engine::startOnCode(Sequence& s, const StartProc& callback) {
    if (!player_->start(s, false, Player::Drive::Code)) {
        return false;
    }
    code_ = CodeSync{};
    const double chase = chaseDelay_ / 10.0;
    const double offset = offsetSeconds(s);
    const auto beat = static_cast<Tick>(std::max<std::uint16_t>(s.ppq / 4, 1));
    // the start position for the code's time: one MIDI beat past it, rounded down to a MIDI beat
    // (code at or before the offset starts at the beginning)
    const auto startAt = [&](const double seconds) -> Tick {
        if (seconds <= offset) {
            return 0;
        }
        const auto t = static_cast<Tick>(std::floor(codeTicks(s, seconds)));
        return (t / beat + 1) * beat;
    };
    // code is taken once two labels in a row were read, of the sequence's rate, and not too far
    const auto takes = [&]() {
        if (code_.run < 2 || code_.rate != rateOf(s.fps)) {
            return false;
        }
        return startAt(code_.time + chase) / std::max<Tick>(s.ppq, 1) < 4096;
    };
    if (callback && callback(startproc::kWaitCode, 0, 0) != 0) {
        player_->cancel();
        return false;
    }
    const auto quarter = [&]() { return 1.0 / (4.0 * nominal(code_.rate)); };
    bool locked = false;
    bool chasing = false; // locked once: the callback says so from then on
    bool lockSent = false;
    std::int64_t lockAt = 0;
    double lockTime = 0;
    for (;;) {
        if (locked && code_.run < 2) {
            locked = false; // the code jumped during the chase: it locks again to the new code
        }
        if (!locked && takes()) {
            // the second label is complete (its last quarter frame was taken at this run)
            locked = true;
            lockSent = false;
            lockAt = code_.labelAt;
            lockTime = code_.label;
        }
        if (locked && !lockSent && code_.count > lockAt + 1) {
            // at the run that takes the next quarter frame (piece 0 of the next group), the patches
            // and the song position of the start ahead go out
            lockSent = true;
            chasing = true;
            player_->lockOutput(startAt(lockTime + chase));
        }
        // the chase ends at the run whose first quarter frame is ChaseDelay of code past the lock
        // and the code has reached the offset: the next quarter frame would be at or after it
        if (locked && lockSent && code_.first >= lockTime + chase - 1e-9 && code_.time + quarter() >= offset - 1e-9) {
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
    player_->go(startAt(code_.first));
    code_.following = true;

    return true;
}

} // namespace libseq
