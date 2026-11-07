// libseq: the engine's clock (TTSSEQ 1.01).
//
// Found in virtual time (tools/ttsprobe/wine/mmsystem16-vtime.patch): the DLL's timer, its clock
// and the MIDI devices run on a millisecond clock that moves only where the probe waits, so a run
// gives the same output to the millisecond every time. Each rule below reproduces the DLL in
// every case tried (ttsseq-api.md, Timing, lists the cases and what each rule rules out).
//
// The engine keeps one tick counter, at the tempo of the sequence where it stands: the playback
// position while playing, a free-running clock while stopped. It starts at 0 at the first
// SeqOpen and carries over SeqClose and the next SeqOpen. At every timer run it adds the whole
// ticks the time since the last run makes, and keeps the rest. When its timer starts (at SeqOpen,
// SeqSetPlayingSeq and every load), the next run counts from one tick before that moment; a stop
// counts on from one tick before the last run (unless no run came since the start), a playback
// start from the last run itself, the end of a count-in from one tick before it, at -1. Its MIDI
// clocks (ppq / 24 ticks) begin the count-in and set its clicks, and 2.10 sends them out.
#pragma once

#include <cstdint>

namespace libseq {

class TickClock {
public:
    /// Time is kept in units of 1 / (1000 * ppq * the tempo ratio) of a millisecond (a tempo ratio
    /// of 64 is the song's tempo): a millisecond is `perMs` = 1000 * ppq * ratio of them, a tick
    /// at a tempo the microseconds of its quarter note (rounded, TempoMap::usPerQuarter) * 64.

    [[nodiscard]] std::int64_t value() const noexcept { return value_; }
    void setValue(std::int64_t v) noexcept { value_ = v; }

    /// A timer run at `now`: `tickAt(t)` is the size of tick t, `nextChange(t)` the next tick
    /// where the size changes after t (-1 for none).
    template <class TickAt, class NextChange>
    void run(std::uint32_t now, std::int64_t perMs, TickAt tickAt, NextChange nextChange) noexcept {
        rest_ += static_cast<std::int64_t>(now - last_) * perMs;
        last_ = now;
        for (;;) {
            const std::int64_t size = tickAt(value_);
            const std::int64_t n = rest_ / size;
            const std::int64_t change = nextChange(value_);
            if (change < 0 || value_ + n < change) {
                rest_ -= n * size;
                value_ += n;
                return;
            }
            rest_ -= (change - value_) * size; // up to the change, the rest at its tick size
            value_ = change;
        }
    }
    /// Counting on from one tick (of size `size`) before `at` (a timer start, a stop at its last
    /// run, the end of a count-in): the rest so far is dropped.
    void tickBefore(std::uint32_t at, std::int64_t size) noexcept {
        last_ = at;
        rest_ = size;
    }
    /// Counting on from `at` exactly (a playback start, at the last run), with `rest` units of
    /// the next tick gone by already.
    void from(std::uint32_t at, std::int64_t rest = 0) noexcept {
        last_ = at;
        rest_ = rest;
    }

private:
    std::int64_t value_ = 0;
    std::int64_t rest_ = 0;
    std::uint32_t last_ = 0;
};

} // namespace libseq
