// libseq: playback as TTSSEQ 1.01 does it.
//
// The rules come from the 86Box captures of the original engine (tests/data/cakewalk/engine/) and
// from playing the engine test songs through the probe (the play_* suites): the order of what goes
// out at a tick, the note-off queue, the start and stop output, loops and the metronome.
//  - Start: the Song Position Pointer (parameter 4), then the patch, volume and pan of every
//    audible stream with events, in track order; then, with search-back (parameter 1), for every
//    audible stream that still has events at or after the start, the last patch, pitch wheel,
//    modulation (1), sustain (64) and volume (7) before it, in that order.
//  - At one tick: the note-offs due first, then the streams in track order, each in stored order;
//    sysex after the channel messages.
//  - A note-on for a key that is sounding on the same port and channel first ends it; a note-off
//    for a key that is not sounding is dropped, so the first end due for a key ends it.
//  - Stop and the end: for each channel the sounding notes' ends in key order and All Notes Off
//    (123); then, with parameter 2, pitch wheel centre, modulation 0, sustain 0 and controller 121.
//  - Key+ outside 0 to 127 folds by octaves into range; Vel+ keeps 1 to 127; an event that Time+
//    moves before tick 0 does not play.
//  - Timing (found in virtual time; clock.hpp has the clock, docs/ttsseq-api.md "Timing" the
//    experiments behind each rule): the engine's timer runs every TimerPeriod milliseconds. At a
//    run the clock first moves on by the whole ticks since the last run; then the sync input of
//    the run is taken (MIDI clock, time code), then what is due goes out (events up to the clock,
//    the metronome, the count-in), and then the run's channel input (thru and the take).
//  - Now is the tick of the last thing played; while nothing plays it moves on in whole MIDI
//    clocks, also past the end until SeqIdle sees the end.
//  - A count-in waits for the clock's next MIDI clock; its clicks fall on Now's beats, its beats
//    left count down from its begin, and at its end the clock starts again at -1: the take's
//    position is the clock plus the start.
#include "playback.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

namespace libseq {

namespace {

constexpr Tick kNever = std::numeric_limits<Tick>::max();
constexpr int kLoopForever = 9999;

/// Division rounding down (the clock is -1 for a moment at the end of a count-in).
std::int64_t floorDiv(const std::int64_t a, const std::int64_t b) noexcept {
    return a >= 0 ? a / b : -((-a + b - 1) / b);
}

std::uint8_t foldKey(int key) noexcept {
    while (key > 127) {
        key -= 12;
    }
    while (key < 0) {
        key += 12;
    }
    return static_cast<std::uint8_t>(key);
}

std::uint8_t byte(const int v) noexcept {
    return static_cast<std::uint8_t>(v & 0x7f);
}

Tick measureStart(const MeterMap& m, const std::uint16_t measure, const std::uint16_t ppq) {
    return m.toTick(measure, 1, 0, ppq);
}

/// The beat at or after a tick, and whether it begins a measure.
std::pair<Tick, bool> beatAtOrAfter(const MeterMap& meters, const Tick tick, const std::uint16_t ppq) {
    const MBT mbt = meters.toMBT(tick, ppq);
    const MeterEntry& e = meters.entries()[meters.indexAtTick(tick)];
    const Tick start = measureStart(meters, mbt.measure, ppq);
    const Tick bt = MeterMap::beatTicks(e, ppq);
    const Tick k = bt > 0 ? (tick - start + bt - 1) / bt : 0;
    if (bt == 0 || k >= e.numerator) {
        return {measureStart(meters, static_cast<std::uint16_t>(mbt.measure + 1), ppq), true};
    }
    return {start + k * bt, k == 0};
}

/// The start of the measure after the one a tick is in.
Tick nextMeasureStart(const MeterMap& meters, const Tick tick, const std::uint16_t ppq) {
    return measureStart(meters, static_cast<std::uint16_t>(meters.toMBT(tick, ppq).measure + 1), ppq);
}

} // namespace

// ---- the note-off queue

void Player::OffHeap::push(const Off& off) {
    heap_.push_back(off);
    std::size_t i = heap_.size() - 1;
    while (i > 0) {
        const std::size_t parent = (i - 1) / 2;
        if (heap_[parent].tick < heap_[i].tick) {
            break;
        }
        std::swap(heap_[parent], heap_[i]);
        i = parent;
    }
}

Player::Off Player::OffHeap::pop() {
    const Off out = heap_.front();
    heap_.front() = heap_.back();
    heap_.pop_back();
    std::size_t i = 0;
    for (;;) {
        const std::size_t left = 2 * i + 1;
        if (left >= heap_.size()) {
            break;
        }
        std::size_t child = left;
        if (left + 1 < heap_.size() && heap_[left + 1].tick < heap_[left].tick) {
            child = left + 1;
        }
        if (!(heap_[child].tick < heap_[i].tick)) {
            break;
        }
        std::swap(heap_[child], heap_[i]);
        i = child;
    }
    return out;
}

// ---- what plays

bool Player::audible(const Stream& stream) {
    return stream.param(sp::kMuted) == 0 && stream.param(sp::kArchived) == 0;
}

void Player::build() {
    items_.clear();
    end_ = 0;
    std::vector<const Stream*> streams;
    for (const Stream* st : seq_->streams()) {
        streams.push_back(st);
    }
    std::stable_sort(streams.begin(), streams.end(), [](const Stream* a, const Stream* b) {
        return a->param(sp::kPosition) < b->param(sp::kPosition);
    });
    // the end of the streams that do not loop: where endless loops stop
    Tick plainEnd = 0;
    for (const Stream* st : streams) {
        if (st->param(sp::kLoopCount) <= 1) {
            const std::int64_t e = static_cast<std::int64_t>(st->duration()) + st->param(sp::kTimePlus);
            plainEnd = std::max(plainEnd, static_cast<Tick>(std::max<std::int64_t>(e, 0)));
        }
    }
    const std::uint16_t ppq = seq_->ppq;
    for (std::size_t rank = 0; rank < streams.size(); ++rank) {
        const Stream& st = *streams[rank];
        int reps = std::max(1, static_cast<int>(st.param(sp::kLoopCount)));
        const std::int64_t offset = st.param(sp::kTimePlus);
        // a loop repeats the stream rounded up to whole measures
        const Tick length = st.duration();
        const Tick period = reps > 1 && length > 0 ? nextMeasureStart(seq_->meterMap, length - 1, ppq) : 0;
        Tick clip = kNever;
        if (reps == kLoopForever) {
            reps = 1;
            if (period > 0 && static_cast<std::int64_t>(plainEnd) > offset) {
                clip = plainEnd;
                reps = static_cast<int>((static_cast<std::int64_t>(plainEnd) - offset + period - 1) / period);
            }
        }
        const int forced = st.param(sp::kChannel);
        const int port = st.param(sp::kPort);
        const int keyPlus = st.param(sp::kKeyPlus);
        const int velPlus = st.param(sp::kVelPlus);
        int index = 0;
        for (int rep = 0; rep < reps; ++rep) {
            const std::int64_t shift = offset + static_cast<std::int64_t>(rep) * period;
            for (std::size_t i = 0; i < st.events().size(); ++i) {
                const Event& e = st.events()[i];
                const std::int64_t at = static_cast<std::int64_t>(e.time) + shift;
                ++index;
                if (at < 0 || static_cast<Tick>(at) >= clip) {
                    continue; // 1.01 drops an event that Time+ moves before tick 0
                }
                Item it;
                it.tick = static_cast<Tick>(at);
                it.rank = static_cast<int>(rank);
                it.audible = audible(st);
                it.index = index;
                it.port = port;
                const unsigned kind = e.status & 0xf0U;
                const auto status = static_cast<std::uint8_t>(forced >= 0 && e.status >= 0x80 && e.status < 0xf0
                                                                  ? kind | (static_cast<unsigned>(forced) & 0xfU)
                                                                  : e.status);
                if (e.status < 0x80) {
                    it.kind = Kind::String;
                    it.stream = st.handle();
                    it.event = i;
                } else if (e.status == 0xf0) {
                    it.kind = Kind::Sysex;
                    it.bank = e.data1;
                } else if (kind == 0x90) {
                    it.kind = Kind::Note;
                    it.size = 3;
                    it.data = {status, foldKey(e.data1 + keyPlus),
                               static_cast<std::uint8_t>(std::clamp(e.data2 + velPlus, 1, 127))};
                    it.off = it.tick + std::max<Tick>(1, e.duration);
                    it.off = std::min(it.off, clip);
                } else if (kind == 0xa0) {
                    it.size = 3;
                    it.data = {status, foldKey(e.data1 + keyPlus), byte(e.data2)};
                } else if (kind == 0xc0 || kind == 0xd0) {
                    it.size = 2;
                    it.data = {status, byte(e.data1), 0};
                } else if (kind >= 0x80 && kind < 0xf0) {
                    it.size = 3;
                    it.data = {status, byte(e.data1), byte(e.data2)};
                } else {
                    continue;
                }
                end_ = std::max(end_, it.kind == Kind::Note ? it.off : it.tick);
                items_.push_back(it);
            }
        }
    }
    std::stable_sort(items_.begin(), items_.end(), [](const Item& a, const Item& b) {
        const bool sa = a.kind == Kind::Sysex;
        const bool sb = b.kind == Kind::Sysex;
        return std::tie(a.tick, sa, a.rank, a.index) < std::tie(b.tick, sb, b.rank, b.index);
    });
}

// ---- time

const Sequence* Player::followed() const {
    // the sequence the clock follows: the one playing, else the playing sequence
    return seq_ != nullptr && playing_ ? seq_ : engine_->sequence(engine_->playingSeq());
}

std::int64_t Player::perMs() const {
    const Sequence* s = followed();
    if (s == nullptr) {
        return 1000LL * 120 * 64;
    }
    const std::uint16_t ratio = s->tempoRatio < s->tempoRatios.size() ? s->tempoRatios.at(s->tempoRatio) : 64;
    return 1000LL * s->ppq * (ratio > 0 ? ratio : 64);
}

std::int64_t Player::tickSize(const std::int64_t tick) const {
    // a tick: the microseconds of a quarter note at the tempo there, rounded as the tempo map
    // does (so 70 bpm runs a little slow); 100 bpm without a sequence
    const Sequence* s = followed();
    if (s == nullptr) {
        return TempoMap::usPerQuarter(10000) * 64;
    }
    const Tick at = playing_ ? static_cast<Tick>(std::max<std::int64_t>(tick + base_, 0)) : s->now;
    return TempoMap::usPerQuarter(s->tempoMap.tempoAt(at)) * 64;
}

void Player::tickBefore(const std::uint32_t at) {
    clock_.tickBefore(at, tickSize(clock_.value()));
}

void Player::timerStarted(const std::uint32_t now) {
    tickBefore(now);
}

std::int64_t Player::clockTicks() const {
    const Sequence* s = seq_ != nullptr && playing_ ? seq_ : engine_->sequence(engine_->playingSeq());
    return std::max<std::int64_t>((s != nullptr ? s->ppq : 120) / 24, 1);
}

void Player::send(const int port, const std::initializer_list<std::uint8_t> bytes) {
    const Platform* p = engine_->platform();
    if (p == nullptr || port < 0 || port >= p->outputPorts()) {
        return; // 1.01 sends nothing to a port that is not there
    }
    engine_->send(port, std::span<const std::uint8_t>(bytes.begin(), bytes.size()));
}

std::set<int> Player::portsInUse() const {
    std::set<int> ports;
    for (const Stream* st : seq_->streams()) {
        if (!st->events().empty()) {
            ports.insert(st->param(sp::kPort));
        }
    }
    if (ports.empty()) {
        ports.insert(0);
    }
    return ports;
}

void Player::noteOn(const Item& it) {
    if (!engine_->notes_.insert(Engine::NoteKey{it.port, it.data[0] & 0x0f, it.data[1]}).second) {
        send(it.port, {it.data[0], it.data[1], 0}); // the key is sounding: it ends first
    }
    send(it.port, {it.data[0], it.data[1], it.data[2]});
    offs_.push({it.off, it.port, it.data[0], it.data[1]});
}

void Player::noteOff(const Off& off) {
    if (engine_->notes_.erase({off.port, off.status & 0x0f, off.key}) > 0) {
        send(off.port, {off.status, off.key, 0});
    }
}

void Player::emit(const Item& it) {
    switch (it.kind) {
    case Kind::Note:
        noteOn(it);
        break;
    case Kind::Short:
        if (it.size == 2) {
            send(it.port, {it.data[0], it.data[1]});
        } else {
            send(it.port, {it.data[0], it.data[1], it.data[2]});
        }
        break;
    case Kind::Sysex:
        for (const auto& [id, bank] : seq_->sysxBanks) {
            if (bank.number == it.bank) {
                const Platform* p = engine_->platform();
                if (p != nullptr && it.port < p->outputPorts() && !bank.data.empty()) {
                    engine_->send(it.port, bank.data);
                }
                break;
            }
        }
        break;
    case Kind::String:
        if (const Stream* st = seq_->stream(it.stream); st != nullptr && it.event < st->events().size()) {
            engine_->playStringEvent(st->events()[it.event]);
        }
        break;
    }
}

void Player::searchBack(const Tick start) {
    std::vector<const Stream*> streams;
    for (const Stream* st : seq_->streams()) {
        streams.push_back(st);
    }
    std::stable_sort(streams.begin(), streams.end(), [](const Stream* a, const Stream* b) {
        return a->param(sp::kPosition) < b->param(sp::kPosition);
    });
    for (std::size_t rank = 0; rank < streams.size(); ++rank) {
        if (!audible(*streams[rank])) {
            continue;
        }
        const int r = static_cast<int>(rank);
        const bool continues =
            std::any_of(items_.begin(), items_.end(), [&](const Item& it) { return it.rank == r && it.tick >= start; });
        if (!continues) {
            continue;
        }
        // the patch, wheel, controllers 1, 64 and 7: the latest of each before the start, per channel
        std::map<std::pair<int, std::uint8_t>, std::array<const Item*, 5>> latest;
        for (const Item& it : items_) {
            if (it.tick >= start) {
                break;
            }
            if (it.rank != r || it.kind != Kind::Short) {
                continue;
            }
            const unsigned kind = it.data[0] & 0xf0U;
            int slot = -1;
            if (kind == 0xc0) {
                slot = 0;
            } else if (kind == 0xe0) {
                slot = 1;
            } else if (kind == 0xb0 && it.data[1] == 1) {
                slot = 2;
            } else if (kind == 0xb0 && it.data[1] == 64) {
                slot = 3;
            } else if (kind == 0xb0 && it.data[1] == 7) {
                slot = 4;
            }
            if (slot >= 0) {
                auto& slots = latest[{it.port, static_cast<std::uint8_t>(it.data[0] & 0x0fU)}];
                slots.at(static_cast<std::size_t>(slot)) = &it;
            }
        }
        for (const auto& [key, slots] : latest) {
            for (const Item* it : slots) {
                if (it != nullptr) {
                    emit(*it);
                }
            }
        }
    }
}

void Player::stopOutput(const bool zeroControllers) {
    std::set<int> ports = portsInUse();
    if (clickOn_) {
        send(click_.port, {click_.status, click_.key, 0});
        clickOn_ = false;
    }
    std::map<int, std::array<std::set<std::uint8_t>, 16>> byChannel;
    for (const auto& [port, channel, key] : engine_->notes_) {
        byChannel[port].at(static_cast<std::size_t>(channel)).insert(static_cast<std::uint8_t>(key));
        ports.insert(port);
    }
    for (const int port : ports) {
        for (unsigned ch = 0; ch < 16; ++ch) {
            if (const auto found = byChannel.find(port); found != byChannel.end()) {
                for (const std::uint8_t key : found->second.at(ch)) {
                    send(port, {static_cast<std::uint8_t>(0x90U | ch), key, 0});
                }
            }
            send(port, {static_cast<std::uint8_t>(0xb0U | ch), 123, 0});
        }
    }
    if (zeroControllers) {
        for (const int port : ports) {
            for (unsigned ch = 0; ch < 16; ++ch) {
                const auto cc = static_cast<std::uint8_t>(0xb0U | ch);
                send(port, {static_cast<std::uint8_t>(0xe0U | ch), 0, 64});
                send(port, {cc, 1, 0});
                send(port, {cc, 64, 0});
                send(port, {cc, 121, 0});
            }
        }
    }
    engine_->notes_.clear();
    offs_.clear();
}

void Player::silence() {
    std::map<int, std::array<std::set<std::uint8_t>, 16>> byChannel;
    for (const auto& [port, channel, key] : engine_->notes_) {
        byChannel[port].at(static_cast<std::size_t>(channel)).insert(static_cast<std::uint8_t>(key));
    }
    if (clickOn_) {
        byChannel[click_.port].at(click_.status & 0x0fU).insert(click_.key);
        clickOn_ = false;
    }
    for (const auto& [port, channels] : byChannel) {
        for (unsigned ch = 0; ch < 16; ++ch) {
            for (const std::uint8_t key : channels.at(ch)) {
                send(port, {static_cast<std::uint8_t>(0x90U | ch), key, 0});
            }
        }
    }
    engine_->notes_.clear();
    offs_.clear();
}

// ---- the metronome

Tick Player::clickTicks() const {
    // the duration counts MIDI clocks (24 a quarter note); 1.01 ends the click two clocks before
    // that count, and a duration of 1 wraps round to 23 clocks
    const int d = seq_->param(qp::kMetroDuration);
    const int clocks = d >= 2 ? d - 2 : 24 + d - 2;
    return static_cast<Tick>(clocks) * seq_->ppq / 24;
}

void Player::click(const bool downbeat) {
    (void)downbeat; // 1.01's MIDI click is the same on every beat
    const int port = seq_->param(qp::kMetroPort);
    const auto status =
        static_cast<std::uint8_t>(0x90U | (static_cast<unsigned>(seq_->param(qp::kMetroChannel)) & 0xfU));
    const std::uint8_t key = byte(seq_->param(qp::kMetroKey));
    if (clickOn_) {
        send(click_.port, {click_.status, click_.key, 0});
    }
    send(port, {status, key, byte(seq_->param(qp::kMetroVelocity))});
    click_ = {0, port, status, key};
    clickOn_ = true;
}

void Player::clicks(const Tick upTo) {
    const bool on = seq_->param(recording_ ? qp::kMetroRecord : qp::kMetroPlay) != 0 &&
                    seq_->param(qp::kMetroVelocity) > 0 && seq_->param(qp::kMetroDuration) > 0;
    const std::uint16_t ppq = seq_->ppq;
    for (;;) {
        // the click's end, when it comes before the next beat
        const bool beatDue = on && clickTick_ <= upTo;
        if (clickOn_ && clickOffTick_ <= upTo && (!beatDue || clickOffTick_ <= clickTick_)) {
            send(click_.port, {click_.status, click_.key, 0});
            clickOn_ = false;
            continue;
        }
        if (!beatDue) {
            return;
        }
        const auto [tick, downbeat] = beatAtOrAfter(seq_->meterMap, clickTick_, ppq);
        if (tick > upTo) {
            clickTick_ = tick;
            return;
        }
        click(downbeat);
        const Tick last = nextMeasureStart(seq_->meterMap, tick, ppq) - ppq / 24;
        clickOffTick_ = std::min(tick + clickTicks(), last);
        clickTick_ = beatAtOrAfter(seq_->meterMap, tick + 1, ppq).first;
    }
}

/// A timer run during the count-in. It begins at the first run after the start call where the
/// clock enters a new MIDI clock, and clicks on the song's beats, counted in MIDI clocks from the
/// start position: at the runs where the clock enters a MIDI clock that is a whole number of
/// beats from it. A click ends after the metronome's duration. The count-in ends at the first run
/// where the position counter reaches the target the start set; the take begins there, with
/// the clock one tick before the start position.
void Player::countInRun() {
    const std::int64_t clk = clockTicks();
    const std::int64_t index = floorDiv(clock_.value(), clk);
    const auto beat = std::max<std::int64_t>(static_cast<std::int64_t>(countInBeatTicks_) / clk, 1);
    const auto fromStart = static_cast<std::int64_t>(seq_->now) / clk;
    bool clickNow = false;
    if (countInWaits_) {
        if (!crossed_) {
            return;
        }
        countInWaits_ = false;
        countInBegin_ = index;
        clickNow = fromStart % beat == 0;
    } else if (crossed_) {
        clickNow = (index - countInBegin_ + fromStart) % beat == 0;
    }
    // the beats left count down from the begin, a beat at a time, whatever Now does to the clicks
    countInDone_ = static_cast<std::uint16_t>(1 + (index - countInBegin_) / beat);
    if (crossed_ && clickOn_ && index >= countInOff_) {
        send(click_.port, {click_.status, click_.key, 0});
        clickOn_ = false;
    }
    if (clickNow) {
        click(true);
        countInOff_ = index + static_cast<std::int64_t>(clickTicks()) / clk;
    }
    if (clock_.value() + pOffset_ < countInTarget_) {
        return;
    }
    countIn_ = false;
    // the clock starts again at -1, whatever the start: the take's position is the clock plus
    // the start (Now 0 to 1000, with and without Song Position)
    base_ = static_cast<std::int64_t>(recordTick_);
    clock_.setValue(-1);
    tickBefore(lastRunMs_);
    pOffset_ = 0;
    lastIndex_ = -1;
    if (clickOn_) {
        // the click goes off in the take, as many MIDI clocks after its start as it had left past
        // the next one (Now 1000, with and without Song Position)
        clickOffTick_ = recordTick_ + static_cast<Tick>(std::max<std::int64_t>(countInOff_ - index - 1, 0) * clk);
    }
    clickTick_ = beatAtOrAfter(seq_->meterMap, recordTick_ + (clickNow ? 1 : 0), seq_->ppq).first;
}

// ---- the transport

void Player::songPositionOnly(Sequence& sequence) {
    // a start that goes no further (SeqRecordStart without a stream left for the take): its
    // Song Position still goes out
    const std::uint16_t ppq = sequence.ppq;
    if (sequence.param(qp::kSendSpp) != 0 && ppq >= 4) {
        Tick tick = sequence.now;
        tick -= tick % (ppq / 4U);
        sequence.now = tick;
        const Tick beats = tick / (ppq / 4U);
        send(0, {0xf2, byte(static_cast<int>(beats & 0x7fU)), byte(static_cast<int>((beats >> 7) & 0x7fU))});
    }
}

bool Player::start(Sequence& sequence, const bool recording, const Drive drive) {
    if (playing_ || waiting_) {
        return false;
    }
    seq_ = &sequence;
    recording_ = recording;
    drive_ = drive;
    const std::uint16_t ppq = seq_->ppq;
    Tick tick = seq_->now;
    const bool spp = drive == Drive::Timer && seq_->param(qp::kSendSpp) != 0;
    if (spp && ppq >= 4) {
        tick -= tick % (ppq / 4U); // a start with Song Position begins on a MIDI beat
    }
    seq_->now = tick;
    seq_->setParam(qp::kNowJumped, 1);
    build();
    // the auto-shuttle, when the start lies before its stop
    loop_ = false;
    stopAt_ = false;
    const Tick rewind = static_cast<Tick>(seq_->param(qp::kRewindTime));
    const Tick stopTime = static_cast<Tick>(seq_->param(qp::kStopTime));
    const bool autoStop = seq_->param(qp::kAutoStop) != 0;
    if (seq_->param(qp::kAutoRewind) != 0 && seq_->param(qp::kAutoRestart) != 0) {
        const Tick thru = autoStop ? stopTime : lastTick();
        if (thru > rewind && tick < thru) {
            loop_ = true;
            loopFrom_ = rewind;
            loopThru_ = thru;
        }
    } else if (autoStop && tick < stopTime) {
        stopAt_ = true;
        stopTick_ = stopTime;
    }
    // a recording's count-in: measures of the meter at the start; it ends one MIDI clock short of
    // them, counted on the position counter from the MIDI clock after it (rounded toward zero)
    countInBeats_ = 0;
    countInDone_ = 0;
    const MeterEntry& meter = seq_->meterMap.entries()[seq_->meterMap.indexAtTick(tick)];
    countInNumerator_ = std::max<std::uint8_t>(meter.numerator, 1);
    countInBeatTicks_ = MeterMap::beatTicks(meter, ppq);
    const auto clk = static_cast<std::int64_t>(std::max<std::uint16_t>(ppq / 24, 1));
    if (recording && seq_->param(qp::kCountIn) > 0 && countInBeatTicks_ > 0) {
        const std::int64_t length = static_cast<std::int64_t>(seq_->param(qp::kCountIn)) * countInNumerator_ *
                                    static_cast<std::int64_t>(countInBeatTicks_);
        const std::int64_t position = clock_.value() + pOffset_;
        countInTarget_ = (position / clk + 1) * clk + length - clk;
        countInBeats_ = static_cast<std::uint16_t>(seq_->param(qp::kCountIn) * countInNumerator_);
        countInWaits_ = true;
        countIn_ = true;
    } else {
        countIn_ = false;
    }
    recordTick_ = tick;
    next_ = static_cast<std::size_t>(
        std::lower_bound(items_.begin(), items_.end(), tick, [](const Item& it, Tick t) { return it.tick < t; }) -
        items_.begin());
    clickTick_ = beatAtOrAfter(seq_->meterMap, tick, ppq).first;
    clickOn_ = false;
    ended_ = false;
    offs_.clear();
    if (spp && ppq >= 4) {
        const Tick beats = tick / (ppq / 4U);
        send(0, {0xf2, byte(static_cast<int>(beats & 0x7fU)), byte(static_cast<int>((beats >> 7) & 0x7fU))});
    }
    std::vector<const Stream*> streams;
    for (const Stream* st : seq_->streams()) {
        streams.push_back(st);
    }
    std::stable_sort(streams.begin(), streams.end(), [](const Stream* a, const Stream* b) {
        return a->param(sp::kPosition) < b->param(sp::kPosition);
    });
    for (const Stream* st : streams) {
        if (!st->events().empty() && audible(*st)) {
            for (const int p : {sp::kPatch, sp::kVolume, sp::kPan}) {
                engine_->sendStreamParam(*st, p, true);
            }
        }
    }
    if (seq_->param(qp::kSearchBack) != 0) {
        searchBack(tick);
    }
    ranSince_ = false;
    atEnd_ = false;
    base_ = 0;
    if (drive != Drive::Timer) {
        waiting_ = true; // until the sync comes
        return true;
    }
    if (!countIn_) {
        // the clock is the playback position, counted from the timer's last run
        clock_.setValue(static_cast<std::int64_t>(tick));
        clock_.from(lastRunMs_);
        pOffset_ = 0; // (the next run compares its MIDI clock with the last run's, before the start)
    }
    playing_ = true; // the timer plays the events from the start on
    return true;
}

// ---- external sync

void Player::go(const Tick from) {
    waiting_ = false;
    seq_->now = from;
    next_ = static_cast<std::size_t>(
        std::lower_bound(items_.begin(), items_.end(), from, [](const Item& it, Tick t) { return it.tick < t; }) -
        items_.begin());
    clickTick_ = beatAtOrAfter(seq_->meterMap, from, seq_->ppq).first;
    spanTick_ = from;
    clocks_ = 0;
    firstClock_ = true;
    deadline_ = std::numeric_limits<double>::infinity();
    codeStop_ = false;
    codeLast_ = -1;
    clock_.setValue(static_cast<std::int64_t>(from));
    clock_.from(lastRunMs_);
    pOffset_ = 0;
    ended_ = false;
    playing_ = true;
}

void Player::cancel() {
    if (waiting_) {
        waiting_ = false;
        stopOutput(seq_->param(qp::kZeroControllers) != 0);
    }
}

void Player::lockOutput(const Tick at) {
    for (const Stream* st : seq_->streams()) {
        if (!st->events().empty() && audible(*st)) {
            for (const int p : {sp::kPatch, sp::kVolume, sp::kPan}) {
                engine_->sendStreamParam(*st, p, true);
            }
        }
    }
    if (seq_->param(qp::kSendSpp) != 0 && seq_->ppq >= 4) {
        const Tick beats = at / (seq_->ppq / 4U);
        send(0, {0xf2, byte(static_cast<int>(beats & 0x7fU)), byte(static_cast<int>((beats >> 7) & 0x7fU))});
    }
    searchBackFor(at);
}

void Player::searchBackFor(const Tick at) {
    if (seq_ != nullptr && seq_->param(qp::kSearchBack) != 0) {
        searchBack(at);
    }
}

void Player::clocks(const unsigned count, const bool held) {
    if (!playing_ || ended_ || drive_ != Drive::Clock || count == 0) {
        return;
    }
    clocks_ += count;
    if (!held) {
        scheduleClock();
    }
}

void Player::externalStop() {
    if (playing_) {
        ended_ = true;
    }
}

void Player::follow(const Tick tick, const double deadline, const bool first, const double exact) {
    if (!playing_ || ended_ || drive_ != Drive::Code) {
        return;
    }
    if (first) {
        clock_.setValue(static_cast<std::int64_t>(tick));
        clock_.from(lastRunMs_);
    }
    codeLast_ = exact;
    deadline_ = deadline;
}

/// MIDI clock: each clock plays the ticks of its span, ppq / 24 of them; Now is then the tick of
/// the last thing it played (a note's end too), or else ppq / 24 more than before, but the first
/// clock after Start or Continue does not move it (suite sync).
void Player::scheduleClock() {
    const Tick span = std::max<Tick>(seq_->ppq / 24U, 1);
    for (; clocks_ > 0 && !ended_; --clocks_) {
        const Tick end = spanTick_ + span;
        bool played = false;
        Tick last = 0;
        for (;;) {
            const Tick itemTick = next_ < items_.size() ? items_[next_].tick : kNever;
            const Tick offTick = offs_.empty() ? kNever : offs_.top().tick;
            const Tick content = std::min(itemTick, offTick);
            if (content >= end) {
                break;
            }
            played = true;
            last = content;
            if (offTick <= itemTick) {
                noteOff(offs_.pop());
                continue;
            }
            if (items_[next_].audible) {
                emit(items_[next_]);
            }
            ++next_;
        }
        if (played) {
            seq_->now = last;
        } else if (!firstClock_) {
            seq_->now += span;
        }
        firstClock_ = false;
        spanTick_ = end;
    }
}

Tick Player::limit() const noexcept {
    if (loop_) {
        return loopThru_;
    }
    // a recording, and a playback that follows time code, play on past the end
    const Tick end = recording_ || drive_ == Drive::Code ? kNever : lastTick();
    return stopAt_ ? std::min(stopTick_, end) : end;
}

/// The timer's run while playing: everything up to the clock goes out, in time order with the
/// metronome. Now is the time of the last thing played; while nothing plays it moves on in whole
/// MIDI clocks from there.
void Player::schedule() {
    const auto at = [this]() { return static_cast<Tick>(std::max<std::int64_t>(position(), 0)); };
    bool played = false;
    Tick last = 0;
    while (!ended_) {
        const Tick e = position() < 0 ? 0 : at();
        const Tick itemTick = next_ < items_.size() ? items_[next_].tick : kNever;
        const Tick offTick = offs_.empty() ? kNever : offs_.top().tick;
        const Tick content = std::min(itemTick, offTick);
        if (position() < 0) {
            break;
        }
        if (loop_ && content >= loopThru_) {
            // the auto-shuttle: at its stop, on at its rewind time without a stop
            if (e < loopThru_) {
                clicks(e);
                break;
            }
            // the wrap: the clock goes back by the loop's length and one tick more, keeping what it
            // ran past the stop (so passes come 0 or one run late, by turns, at 120 bpm)
            clicks(loopThru_ - 1);
            silence();
            clock_.setValue(clock_.value() - static_cast<std::int64_t>(loopThru_ - loopFrom_) - 1);
            seq_->setParam(qp::kNowJumped, 1);
            seq_->now = loopFrom_;
            next_ = static_cast<std::size_t>(std::lower_bound(items_.begin(), items_.end(), loopFrom_,
                                                              [](const Item& it, Tick t) { return it.tick < t; }) -
                                             items_.begin());
            clickTick_ = beatAtOrAfter(seq_->meterMap, loopFrom_, seq_->ppq).first;
            continue;
        }
        const Tick stop = stopAt_ ? stopTick_ : kNever;
        if ((recording_ || drive_ == Drive::Code) && content == kNever && !stopAt_) {
            clicks(e);
            break;
        }
        if (content >= stop || content == kNever) {
            // the end, or the auto-stop time, whose events do not play
            const Tick end = content == kNever ? lastTick() : stopTick_;
            clicks(end == stopTick_ && stopAt_ ? std::min(e, stopTick_ - 1) : std::min(e, end));
            if (end <= e) {
                ended_ = true;
                atEnd_ = true;
                seq_->now = end;
            }
            return;
        }
        if (content > e) {
            clicks(std::min(e, limit()));
            break;
        }
        clicks(content);
        played = true;
        last = content;
        if (offTick <= itemTick) {
            noteOff(offs_.pop());
            continue;
        }
        if (items_[next_].audible) {
            emit(items_[next_]);
        }
        ++next_;
    }
    if (ended_) {
        return;
    }
    const auto clk = static_cast<Tick>(clockTicks());
    if (played) {
        seq_->now = last;
    } else if (position() >= 0 && at() >= seq_->now + clk) {
        seq_->now += (at() - seq_->now) / clk * clk;
    }
    if (drive_ == Drive::Code && codeLast_ >= 0) {
        // on from the last quarter frame of the run, with its fraction of a tick
        const auto whole = static_cast<std::int64_t>(std::floor(codeLast_ + 1e-9));
        const auto size = tickSize(whole);
        const double part = std::max(codeLast_ - static_cast<double>(whole), 0.0);
        clock_.setValue(whole);
        clock_.from(lastRunMs_, static_cast<std::int64_t>(part * static_cast<double>(size)));
        codeLast_ = -1;
    }
    if (drive_ == Drive::Code && (codeStop_ || static_cast<double>(clock_.value()) > deadline_ + 1e-6)) {
        // the code jumped, or stopped and the clock ran on FlywheelAmount past it: the playback
        // ends after this run
        codeStop_ = false;
        ended_ = true;
    }
}

void Player::opened(const std::uint32_t now) {
    // the clock and the position counter start at the first SeqOpen and carry over a SeqClose
    // and the next SeqOpen (a hundred close and open cycles, at every 1 ms phase); the timer's
    // start counts from one tick before
    if (!everOpened_) {
        everOpened_ = true;
        clock_.setValue(0);
        lastIndex_ = 0;
        pStarted_ = false;
        pOffset_ = 0;
    }
    tickBefore(now);
    lastRunMs_ = now;
}

void Player::created() noexcept {
    if (!pStarted_) {
        pStarted_ = true;
        pOffset_ = -1 - clock_.value();
    }
}

void Player::advance(const std::uint32_t now) {
    // while playing the clock is the position and goes through the tempo map; while stopped (and
    // counting in) it runs at the tempo at Now
    const bool map = playing_ && seq_ != nullptr && drive_ == Drive::Timer && !countIn_;
    clock_.run(
        now, perMs(), [this](const std::int64_t t) { return tickSize(t); },
        [this, map](const std::int64_t t) -> std::int64_t {
            if (!map) {
                return -1;
            }
            const TempoMap& m = seq_->tempoMap;
            const std::size_t i = m.indexAt(static_cast<Tick>(std::max<std::int64_t>(t + base_, 0))) + 1;
            return i < m.size() ? static_cast<std::int64_t>(m.entries()[i].tick) - base_ : -1;
        });
    lastRunMs_ = now;
    ranSince_ = true;
    const std::int64_t index = floorDiv(clock_.value(), clockTicks());
    crossed_ = index > lastIndex_;
    lastIndex_ = index;
}

void Player::timer() {
    if (playing_ && ended_ && atEnd_ && seq_ != nullptr) {
        // past the end, until SeqIdle sees it, Now moves on in whole MIDI clocks
        const auto clk = clockTicks();
        if (position() >= static_cast<std::int64_t>(seq_->now) + clk) {
            seq_->now += static_cast<Tick>((position() - static_cast<std::int64_t>(seq_->now)) / clk * clk);
        }
        return;
    }
    if (!playing_ || ended_ || seq_ == nullptr || engine_->platform() == nullptr) {
        return;
    }
    if (countIn_) {
        countInRun();
        return;
    }
    if (drive_ == Drive::Clock) {
        return; // the clocks play what they span when they come in
    }
    schedule();
}

bool Player::countingIn() const {
    return playing_ && recording_ && countIn_;
}

Tick Player::recordTick() const {
    if (countIn_ || position() <= static_cast<std::int64_t>(recordTick_)) {
        return recordTick_;
    }
    return static_cast<Tick>(position());
}

std::uint16_t Player::idle() {
    if (!playing_ || !ended_) {
        return 0;
    }
    playing_ = false;
    ended_ = false;
    stopOutput(seq_->param(qp::kZeroControllers) != 0);
    tickBefore(lastRunMs_);
    if (seq_->param(qp::kAutoRewind) != 0) {
        seq_->now = static_cast<Tick>(seq_->param(qp::kRewindTime));
    }
    return 1;
}

void Player::jump(const Tick tick) {
    silence();
    clock_.setValue(static_cast<std::int64_t>(tick));
    next_ = static_cast<std::size_t>(
        std::lower_bound(items_.begin(), items_.end(), tick, [](const Item& it, Tick t) { return it.tick < t; }) -
        items_.begin());
    clickTick_ = beatAtOrAfter(seq_->meterMap, tick, seq_->ppq).first;
    seq_->now = tick;
    schedule();
}

bool Player::stop() {
    if (!playing_) {
        return false;
    }
    playing_ = false;
    ended_ = false;
    stopOutput(seq_->param(qp::kZeroControllers) != 0);
    // the clock counts on from one tick before the timer's last run; when no run came since the
    // start, from the start's (the last run itself)
    if (ranSince_) {
        tickBefore(lastRunMs_);
    }
    return true;
}

void Player::forget(const Sequence& sequence) noexcept {
    if (seq_ == &sequence) {
        seq_ = nullptr;
        playing_ = false;
        ended_ = false;
        offs_.clear();
    }
}

} // namespace libseq
