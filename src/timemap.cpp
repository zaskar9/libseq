// libseq: the tempo map, the meter map and the time conversions (TTSSEQ 1.01).
#include "libseq/timemap.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>

namespace libseq {

namespace {

/// `num / den` rounded to the nearest integer, halves up.
std::uint64_t roundDiv(const std::uint64_t num, const std::uint64_t den) noexcept {
    return (2 * num + den) / (2 * den);
}

/// Up to `n` numbers of a string, any characters between them; missing ones are 0.
template <std::size_t N>
std::array<unsigned, N> numbers(const std::string& s) {
    std::array<unsigned, N> v{};
    std::size_t i = 0;
    for (std::size_t k = 0; k < N; ++k) {
        while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])) == 0) {
            ++i;
        }
        if (i == s.size()) {
            break;
        }
        while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])) != 0) {
            v[k] = v[k] * 10 + static_cast<unsigned>(s[i] - '0');
            ++i;
        }
    }
    return v;
}

} // namespace

// ---------------------------------------------------------------- TempoMap

TempoMap::TempoMap() : entries_{TempoEntry{}} {}

std::int64_t TempoMap::usPerQuarter(const std::uint16_t tempo) noexcept {
    const std::int64_t t = tempo == 0 ? 1 : tempo;
    return (6'000'000'000LL + t / 2) / t;
}

std::size_t TempoMap::indexAt(const Tick tick) const noexcept {
    std::size_t i = 0;
    while (i + 1 < entries_.size() && entries_[i + 1].tick <= tick) {
        ++i;
    }
    return i;
}

void TempoMap::refresh(const std::size_t i) {
    if (i > 0) {
        const TempoEntry& p = entries_[i - 1];
        entries_[i].elapsed = static_cast<std::int64_t>(entries_[i].tick - p.tick) * usPerQuarter(p.tempo);
    }
    for (std::size_t j = i + 1; j < entries_.size(); ++j) {
        const TempoEntry& p = entries_[j - 1];
        entries_[j].elapsed = p.elapsed + static_cast<std::int64_t>(entries_[j].tick - p.tick) * usPerQuarter(p.tempo);
    }
}

void TempoMap::set(const Tick tick, const std::uint16_t tempo) {
    const std::size_t i = indexAt(tick);
    entries_[i].tempo = tempo;
    refresh(i);
}

void TempoMap::insert(const Tick tick, const std::uint16_t tempo) {
    const auto it = std::ranges::find_if(entries_, [tick](const TempoEntry& e) { return e.tick >= tick; });
    if (it != entries_.end() && it->tick == tick) {
        it->tempo = tempo;
        refresh(static_cast<std::size_t>(it - entries_.begin()));
        return;
    }
    TempoEntry e;
    e.tick = tick;
    e.tempo = tempo;
    const auto at = entries_.insert(it, e);
    refresh(static_cast<std::size_t>(at - entries_.begin()));
}

void TempoMap::remove(const Tick tick) {
    if (entries_.size() > 1) {
        entries_.erase(entries_.begin() + static_cast<std::ptrdiff_t>(indexAt(tick)));
    }
}

std::size_t TempoMap::recalc() {
    entries_.front().tick = 0;
    entries_.front().elapsed = 0;
    refresh(0);
    return entries_.size();
}

void TempoMap::clear(const std::uint16_t tempo) {
    entries_.assign(1, TempoEntry{});
    entries_.front().tempo = tempo;
}

void TempoMap::assign(std::vector<TempoEntry> entries) {
    if (entries.empty()) {
        entries.emplace_back();
    }
    entries_ = std::move(entries);
    recalc();
}

void TempoMap::rescale(const std::uint16_t from, const std::uint16_t to) {
    for (TempoEntry& e : entries_) {
        e.tick = rescaleTime(e.tick, from, to);
    }
    recalc();
}

std::int64_t TempoMap::elapsedAt(const Tick tick) const noexcept {
    const TempoEntry& e = entries_[indexAt(tick)];
    return e.elapsed + (static_cast<std::int64_t>(tick) - static_cast<std::int64_t>(e.tick)) * usPerQuarter(e.tempo);
}

Tick TempoMap::tickAt(const std::uint64_t whole, const std::uint64_t part, const std::uint64_t parts,
                      const bool up) const noexcept {
    std::size_t i = 0;
    while (i + 1 < entries_.size() && entries_[i + 1].elapsed <= static_cast<std::int64_t>(whole)) {
        ++i;
    }
    const TempoEntry& e = entries_[i];
    if (static_cast<std::int64_t>(whole) < e.elapsed) {
        return e.tick;
    }
    const auto upq = static_cast<std::uint64_t>(usPerQuarter(e.tempo));
    const std::uint64_t d = whole - static_cast<std::uint64_t>(e.elapsed);
    const std::uint64_t num = (d % upq) * parts + part;
    const std::uint64_t den = upq * parts;
    const std::uint64_t ticks = d / upq + (up ? (num + den - 1) / den : roundDiv(num, den));
    return static_cast<Tick>(e.tick + ticks);
}

// ---------------------------------------------------------------- MeterMap

MeterMap::MeterMap() : entries_{MeterEntry{}} {}

Tick MeterMap::beatTicks(const MeterEntry& e, const std::uint16_t ppq) noexcept {
    return (Tick{ppq} * 4) >> (e.denominatorLog2 & 0x1fU);
}

Tick MeterMap::measureTicks(const MeterEntry& e, const std::uint16_t ppq) noexcept {
    return beatTicks(e, ppq) * e.numerator;
}

std::size_t MeterMap::indexAtMeasure(const int measure0) const noexcept {
    std::size_t i = 0;
    while (i + 1 < entries_.size() && entries_[i + 1].measure <= measure0) {
        ++i;
    }
    return i;
}

std::size_t MeterMap::indexAtTick(const Tick tick) const noexcept {
    std::size_t i = 0;
    while (i + 1 < entries_.size() && entries_[i + 1].tick <= tick) {
        ++i;
    }
    return i;
}

namespace {

void setMeterWord(MeterEntry& e, const std::int8_t key, const std::uint16_t meter) {
    e.key = key;
    e.numerator = static_cast<std::uint8_t>(meter & 0xffU);
    e.denominatorLog2 = static_cast<std::uint8_t>(meter >> 8);
}

} // namespace

void MeterMap::insert(const std::uint16_t measure, const std::int8_t key, const std::uint16_t meter,
                      const std::uint16_t ppq) {
    const auto m0 = static_cast<std::uint16_t>(measure - 1);
    const auto it = std::ranges::find_if(entries_, [m0](const MeterEntry& e) { return e.measure >= m0; });
    if (it != entries_.end() && it->measure == m0) {
        setMeterWord(*it, key, meter);
    } else {
        MeterEntry e;
        e.measure = m0;
        setMeterWord(e, key, meter);
        entries_.insert(it, e);
    }
    refresh(ppq);
}

void MeterMap::set(const Tick tick, const std::int8_t key, const std::uint16_t meter, const std::uint16_t ppq) {
    setMeterWord(entries_[indexAtTick(tick)], key, meter);
    refresh(ppq);
}

void MeterMap::remove(const std::uint16_t measure) {
    if (entries_.size() > 1) {
        const auto m0 = static_cast<std::int16_t>(measure - 1);
        entries_.erase(entries_.begin() + static_cast<std::ptrdiff_t>(indexAtMeasure(m0)));
    }
}

std::size_t MeterMap::recalc(const std::uint16_t ppq) {
    entries_.front().measure = 0;
    refresh(ppq);
    return entries_.size();
}

void MeterMap::refresh(const std::uint16_t ppq) {
    entries_.front().tick = 0;
    for (std::size_t j = 1; j < entries_.size(); ++j) {
        const MeterEntry& p = entries_[j - 1];
        entries_[j].tick =
            p.tick + Tick{static_cast<std::uint16_t>(entries_[j].measure - p.measure)} * measureTicks(p, ppq);
    }
}

void MeterMap::clear(const std::int8_t key, const std::uint16_t meter) {
    entries_.assign(1, MeterEntry{});
    setMeterWord(entries_.front(), key, meter);
}

void MeterMap::assign(std::vector<MeterEntry> entries, const std::uint16_t ppq) {
    if (entries.empty()) {
        entries.emplace_back();
    }
    entries_ = std::move(entries);
    recalc(ppq);
}

Tick MeterMap::toTick(const std::uint16_t measure, const std::uint16_t beat, const std::uint16_t tick,
                      const std::uint16_t ppq) const noexcept {
    const auto m0 = static_cast<std::uint16_t>(measure - 1);
    const MeterEntry& e = entries_[indexAtMeasure(static_cast<std::int16_t>(m0))];
    const auto measures = static_cast<std::uint16_t>(m0 - e.measure);
    const auto beats = static_cast<std::uint16_t>(beat - 1);
    return e.tick + Tick{measures} * measureTicks(e, ppq) + Tick{beats} * beatTicks(e, ppq) + tick;
}

MBT MeterMap::toMBT(const Tick tick, const std::uint16_t ppq) const noexcept {
    const MeterEntry& e = entries_[indexAtTick(tick)];
    const Tick mt = std::max<Tick>(measureTicks(e, ppq), 1);
    const Tick bt = std::max<Tick>(beatTicks(e, ppq), 1);
    const Tick rel = tick - e.tick;
    MBT p;
    p.measure = static_cast<std::uint16_t>(e.measure + rel / mt + 1);
    p.beat = static_cast<std::uint16_t>((rel % mt) / bt + 1);
    p.tick = static_cast<std::uint16_t>((rel % mt) % bt);
    return p;
}

// The neighbours of a tick go through its position, as the engine does: the position of the tick,
// then the tick of the neighbouring position. With stale entry ticks the two need not agree.

Tick MeterMap::nextMeasure(const Tick tick, const std::uint16_t ppq) const noexcept {
    return toTick(static_cast<std::uint16_t>(toMBT(tick, ppq).measure + 1), 1, 0, ppq);
}

Tick MeterMap::prevMeasure(const Tick tick, const std::uint16_t ppq) const noexcept {
    const MBT p = toMBT(tick, ppq);
    if (p.beat == 1 && p.tick == 0 && p.measure > 1) {
        return toTick(static_cast<std::uint16_t>(p.measure - 1), 1, 0, ppq);
    }
    return toTick(p.measure, 1, 0, ppq);
}

Tick MeterMap::nextBeat(const Tick tick, const std::uint16_t ppq) const noexcept {
    const MBT p = toMBT(tick, ppq);
    return toTick(p.measure, static_cast<std::uint16_t>(p.beat + 1), 0, ppq);
}

Tick MeterMap::prevBeat(const Tick tick, const std::uint16_t ppq) const noexcept {
    const MBT p = toMBT(tick, ppq);
    if (p.tick != 0) {
        return toTick(p.measure, p.beat, 0, ppq);
    }
    if (p.beat > 1) {
        return toTick(p.measure, static_cast<std::uint16_t>(p.beat - 1), 0, ppq);
    }
    if (p.measure > 1) {
        // the last beat of the measure before, by the meter in effect at its first tick
        const Tick start = toTick(static_cast<std::uint16_t>(p.measure - 1), 1, 0, ppq);
        const MeterEntry& e = entries_[indexAtTick(start)];
        return start + Tick{static_cast<std::uint8_t>(e.numerator - 1)} * beatTicks(e, ppq);
    }
    return toTick(1, 1, 0, ppq);
}

// ---------------------------------------------------------------- timecode

bool validFps(const std::uint16_t fps) noexcept {
    return fps == 24 || fps == 25 || fps == 29 || fps == 30;
}

HMSF framesToHMSF(std::uint32_t frames, const std::uint16_t fps) noexcept {
    const std::uint32_t nominal = fps == 29 ? 30 : std::max<std::uint16_t>(fps, 1);
    if (fps == 29) {
        // 17982 frames in ten minutes; two frame labels skipped in each minute but the tenth
        const std::uint32_t tens = frames / 17982;
        const std::uint32_t rest = frames % 17982;
        frames += 18 * tens + (rest >= 2 ? 2 * ((rest - 2) / 1798) : 0);
    }
    HMSF t;
    t.frames = static_cast<std::uint16_t>(frames % nominal);
    frames /= nominal;
    t.seconds = static_cast<std::uint16_t>(frames % 60);
    frames /= 60;
    t.minutes = static_cast<std::uint16_t>(frames % 60);
    t.hours = static_cast<std::uint16_t>(frames / 60);
    return t;
}

std::uint32_t hmsfToFrames(const HMSF& t, const std::uint16_t fps) noexcept {
    const std::uint32_t nominal = fps == 29 ? 30 : fps;
    const std::uint32_t minutes = std::uint32_t{t.hours} * 60 + t.minutes;
    std::uint32_t frames = (minutes * 60 + t.seconds) * nominal + t.frames;
    if (fps == 29) {
        frames -= 2 * (minutes - minutes / 10);
        // a label that drop frame skips (frames 0 and 1 of second 0) counts as the one 2 frames on
        if (minutes % 10 != 0 && t.seconds == 0 && t.frames < 2) {
            frames += 2;
        }
    }
    return frames;
}

std::string formatMBT(const MBT& p, const bool twoDigitBeat) {
    std::array<char, 32> buf{};
    std::snprintf(buf.data(), buf.size(), twoDigitBeat ? "%u:%02u:%03u" : "%u:%u:%03u", unsigned{p.measure},
                  unsigned{p.beat}, unsigned{p.tick});
    return buf.data();
}

MBT parseMBT(const std::string& s) {
    const auto v = numbers<3>(s);
    MBT p;
    p.measure = static_cast<std::uint16_t>(v[0] == 0 ? 1 : v[0]);
    p.beat = static_cast<std::uint16_t>(v[1] == 0 ? 1 : v[1]);
    p.tick = static_cast<std::uint16_t>(v[2]);
    return p;
}

std::string formatHMSF(const HMSF& t) {
    std::array<char, 32> buf{};
    std::snprintf(buf.data(), buf.size(), "%02u:%02u:%02u:%02u", unsigned{t.hours}, unsigned{t.minutes},
                  unsigned{t.seconds}, unsigned{t.frames});
    return buf.data();
}

HMSF parseHMSF(const std::string& s) {
    const auto v = numbers<4>(s);
    HMSF t;
    t.hours = static_cast<std::uint16_t>(v[0]);
    t.minutes = static_cast<std::uint16_t>(v[1]);
    t.seconds = static_cast<std::uint16_t>(v[2]);
    t.frames = static_cast<std::uint16_t>(v[3]);
    return t;
}

std::string tempoRatioString(const std::uint16_t ratio) {
    const unsigned hundredths = (ratio & 0xffU) * 100U / 64U;
    std::array<char, 16> buf{};
    std::snprintf(buf.data(), buf.size(), "%u.%02u", hundredths / 100, hundredths % 100);
    return buf.data();
}

Tick rescaleTime(const Tick time, const std::uint16_t from, const std::uint16_t to) noexcept {
    if (from == 0) {
        return time;
    }
    return static_cast<Tick>(std::uint64_t{time} * to / from);
}

} // namespace libseq
