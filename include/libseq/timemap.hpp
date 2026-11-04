// libseq: the tempo map, the meter map and the time conversions of a sequence (TTSSEQ 1.01).
//
// The engine keeps for each tempo entry the time elapsed up to it, in microseconds times the ppq
// (ticks times microseconds per quarter note), and for each meter entry the tick it starts at.
// Those cached values are only brought up to date where the engine does it: an edit refreshes
// the entries from the one it touched on, a deletion refreshes nothing, and SeqRecalcTempoMap or
// SeqRecalcMeterMap recompute the whole map. The conversions read the cached values, so after an
// edit they answer as the engine does, stale values included (docs/reverse-engineering/
// ttsseq-api.md, "Time, tempo, meter").
#pragma once

#include "types.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace libseq {

inline constexpr std::uint16_t kMaxPpq = 480;     // SeqGetMaxPPQ
inline constexpr std::uint16_t kMinTempo = 1000;  // SeqGetMinTempo: 10.00 bpm
inline constexpr std::uint16_t kMaxTempo = 25000; // SeqGetMaxTempo: 250.00 bpm

/// A time with its tempo: from `tick` on, `tempo` beats per minute times 100.
struct TempoEntry {
    Tick tick = 0;
    std::uint16_t tempo = 10000;
    /// Elapsed time up to the entry in microseconds times the ppq, as the engine last computed it.
    std::int64_t elapsed = 0;
};

/// The tempo map: entries in tick order. It is never empty.
class TempoMap {
public:
    TempoMap();

    /// Microseconds per quarter note at a tempo, rounded to the nearest microsecond.
    [[nodiscard]] static std::int64_t usPerQuarter(std::uint16_t tempo) noexcept;

    [[nodiscard]] const std::vector<TempoEntry>& entries() const noexcept { return entries_; }
    [[nodiscard]] std::size_t size() const noexcept { return entries_.size(); }

    /// The entry in effect at a tick: the last one at or before it (the first when none is).
    [[nodiscard]] std::size_t indexAt(Tick tick) const noexcept;
    [[nodiscard]] std::uint16_t tempoAt(Tick tick) const noexcept { return entries_[indexAt(tick)].tempo; }

    /// SeqSetTempo: changes the tempo of the entry in effect at the tick.
    void set(Tick tick, std::uint16_t tempo);
    /// SeqInsTempo: a new entry at the tick, or a new tempo for the entry already there.
    void insert(Tick tick, std::uint16_t tempo);
    /// SeqDelTempo: removes the entry in effect at the tick (also the first one, while another is left).
    void remove(Tick tick);
    /// SeqRecalcTempoMap: moves the first entry to tick 0 and recomputes the elapsed times.
    std::size_t recalc();
    /// SeqClearTempoMap: one entry at tick 0.
    void clear(std::uint16_t tempo);
    /// The entries of a file, recomputed.
    void assign(std::vector<TempoEntry> entries);
    /// The ticks of the entries scaled to another ppq (SeqSetPPQ), recomputed.
    void rescale(std::uint16_t from, std::uint16_t to);

    /// Elapsed time at a tick, in microseconds times the ppq.
    [[nodiscard]] std::int64_t elapsedAt(Tick tick) const noexcept;
    /// The tick at an elapsed time `whole + part / parts` (microseconds times the ppq): rounded to
    /// the nearest tick, or up to the next whole tick when `up`.
    [[nodiscard]] Tick tickAt(std::uint64_t whole, std::uint64_t part, std::uint64_t parts, bool up) const noexcept;

private:
    /// What the engine does after changing entry `i`: entry `i` gets the time from the entry before
    /// it to itself (but not the time before that entry), and the entries after it follow on.
    void refresh(std::size_t i);
    std::vector<TempoEntry> entries_;
};

/// A meter and key from a measure on.
struct MeterEntry {
    std::uint16_t measure = 0; // 0-based
    std::uint8_t numerator = 4;
    std::uint8_t denominatorLog2 = 2;
    std::int8_t key = 0; // sharps (positive) or flats (negative)
    /// The tick the measure starts at, as the engine last computed it.
    Tick tick = 0;

    /// The meter word: the power of two of the denominator in the high byte, the numerator in the low.
    [[nodiscard]] std::uint16_t meter() const noexcept {
        return static_cast<std::uint16_t>((denominatorLog2 << 8) | numerator);
    }
};

/// A position: measure and beat count from 1, the tick within the beat from 0.
struct MBT {
    std::uint16_t measure = 1;
    std::uint16_t beat = 1;
    std::uint16_t tick = 0;
};

/// The meter map: entries in measure order. It is never empty.
class MeterMap {
public:
    MeterMap();

    [[nodiscard]] const std::vector<MeterEntry>& entries() const noexcept { return entries_; }
    [[nodiscard]] std::size_t size() const noexcept { return entries_.size(); }

    [[nodiscard]] std::size_t indexAtMeasure(int measure0) const noexcept;
    [[nodiscard]] std::size_t indexAtTick(Tick tick) const noexcept;

    /// SeqInsMeter: an entry at the measure (1-based), or new values for the entry already there.
    void insert(std::uint16_t measure, std::int8_t key, std::uint16_t meter, std::uint16_t ppq);
    /// SeqSetMeter: new values for the entry in effect at the tick.
    void set(Tick tick, std::int8_t key, std::uint16_t meter, std::uint16_t ppq);
    /// SeqDelMeter: removes the entry in effect at the measure (1-based).
    void remove(std::uint16_t measure);
    /// SeqRecalcMeterMap: moves the first entry to measure 1 and recomputes the ticks the entries
    /// start at.
    std::size_t recalc(std::uint16_t ppq);
    /// SeqClearMeterMap: one entry at measure 1.
    void clear(std::int8_t key, std::uint16_t meter);
    /// The entries of a file, recomputed.
    void assign(std::vector<MeterEntry> entries, std::uint16_t ppq);

    [[nodiscard]] static Tick beatTicks(const MeterEntry& e, std::uint16_t ppq) noexcept;
    [[nodiscard]] static Tick measureTicks(const MeterEntry& e, std::uint16_t ppq) noexcept;

    [[nodiscard]] Tick toTick(std::uint16_t measure, std::uint16_t beat, std::uint16_t tick,
                              std::uint16_t ppq) const noexcept;
    [[nodiscard]] MBT toMBT(Tick tick, std::uint16_t ppq) const noexcept;
    [[nodiscard]] Tick nextMeasure(Tick tick, std::uint16_t ppq) const noexcept;
    [[nodiscard]] Tick prevMeasure(Tick tick, std::uint16_t ppq) const noexcept;
    [[nodiscard]] Tick nextBeat(Tick tick, std::uint16_t ppq) const noexcept;
    [[nodiscard]] Tick prevBeat(Tick tick, std::uint16_t ppq) const noexcept;

private:
    /// Recomputes the ticks the entries start at from the first entry's measure on.
    void refresh(std::uint16_t ppq);
    std::vector<MeterEntry> entries_;
};

/// Hours, minutes, seconds and frames.
struct HMSF {
    std::uint16_t hours = 0;
    std::uint16_t minutes = 0;
    std::uint16_t seconds = 0;
    std::uint16_t frames = 0;
};

/// The frame rates the engine takes (SeqSetFPS): 24, 25, 29 (29.97, drop frame) and 30.
[[nodiscard]] bool validFps(std::uint16_t fps) noexcept;
/// Frame numbers to labels and back; at 29 fps the labels skip frames 0 and 1 of each minute
/// that is not a tenth.
[[nodiscard]] HMSF framesToHMSF(std::uint32_t frames, std::uint16_t fps) noexcept;
[[nodiscard]] std::uint32_t hmsfToFrames(const HMSF& t, std::uint16_t fps) noexcept;

/// "2:1:039", or with `twoDigitBeat` "2:01:039" (SeqTick2MBTStr).
[[nodiscard]] std::string formatMBT(const MBT& p, bool twoDigitBeat);
/// SeqMBTStr2MBT: up to three numbers, any characters between; a missing or 0 measure or beat is 1.
[[nodiscard]] MBT parseMBT(const std::string& s);
/// "00:00:02:00" (SeqTick2HMSFStr).
[[nodiscard]] std::string formatHMSF(const HMSF& t);
/// SeqHMSFStr2HMSF: up to four numbers, any characters between; missing ones are 0.
[[nodiscard]] HMSF parseHMSF(const std::string& s);

/// SeqTempoRatio2Str: a ratio in 64ths (8 bits of it) as "1.25", the hundredths cut off.
[[nodiscard]] std::string tempoRatioString(std::uint16_t ratio);

/// SeqRescaleTime: a time at one ppq at another, rounded down.
[[nodiscard]] Tick rescaleTime(Tick time, std::uint16_t from, std::uint16_t to) noexcept;

} // namespace libseq
