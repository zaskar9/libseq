// libseq: sequences, streams and their parameters (TTSSEQ 1.01).
#include "libseq/engine.hpp"

#include "playback.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <iterator>
#include <string_view>

namespace libseq {

namespace {

/// How a parameter keeps its value: the width the engine stores, and whether it reads back
/// sign-extended. A flag keeps 0 or 1.
enum class Kind : std::uint8_t { None, Flag, S8, U8, S16, U16, S32 };

std::int32_t store(const Kind kind, const std::int32_t v) noexcept {
    switch (kind) {
    case Kind::Flag:
        return v != 0 ? 1 : 0;
    case Kind::S8:
        return static_cast<std::int8_t>(v & 0xff);
    case Kind::U8:
        return v & 0xff;
    case Kind::S16:
        return static_cast<std::int16_t>(v & 0xffff);
    case Kind::U16:
        return v & 0xffff;
    case Kind::S32:
        return v;
    case Kind::None:
        break;
    }
    return 0;
}

constexpr std::array<Kind, sp::kCount> kStreamKinds{
    Kind::S8,   // 0 Key+
    Kind::S8,   // 1 Vel+
    Kind::S32,  // 2 Time+
    Kind::U8,   // 3 port
    Kind::S8,   // 4 channel
    Kind::S8,   // 5 patch
    Kind::S8,   // 6 volume
    Kind::Flag, // 7 muted
    Kind::Flag, // 8 archived
    Kind::U16,  // 9 loop count
    Kind::S8,   // 10 pan
    Kind::U16,  // 11 track position
};

constexpr std::array<Kind, qp::kCount> kSeqKinds{
    Kind::None, // 0
    Kind::Flag, // 1 search-back
    Kind::Flag, // 2 zero controllers
    Kind::Flag, // 3 Now jumped
    Kind::Flag, // 4 send SPP
    Kind::Flag, // 5 use Continue
    Kind::Flag, // 6 send sync
    Kind::S8,   // 7 thru mode
    Kind::S8,   // 8 thru port
    Kind::S8,   // 9 thru channel
    Kind::S8,   // 10 thru Key+
    Kind::S8,   // 11 thru Vel+
    Kind::S16,  // 12 Local On port
    Kind::Flag, // 13 metronome on playback
    Kind::Flag, // 14 metronome on recording
    Kind::Flag, // 15 metronome accent
    Kind::U8,   // 16 count-in
    Kind::U8,   // 17 metronome port
    Kind::U8,   // 18 metronome channel
    Kind::U8,   // 19 metronome key
    Kind::U8,   // 20 metronome velocity
    Kind::U8,   // 21 metronome duration
    Kind::Flag, // 22 metronome on the PC speaker
    Kind::Flag, // 23 auto-stop
    Kind::Flag, // 24 auto-rewind
    Kind::Flag, // 25 auto-restart
    Kind::S32,  // 26 rewind time
    Kind::S32,  // 27 stop time
    Kind::U8,   // 28 record and echo filter
};

} // namespace

// ---------------------------------------------------------------- Stream

Stream::Stream(const StreamHandle handle) : handle_(handle) {
    // A new stream: muted, no forced channel, patch, volume or pan, played once.
    params_[sp::kChannel] = -1;
    params_[sp::kPatch] = -1;
    params_[sp::kVolume] = -1;
    params_[sp::kMuted] = 1;
    params_[sp::kLoopCount] = 1;
    params_[sp::kPan] = -1;
}

std::int32_t Stream::param(const int p) const noexcept {
    return p >= 0 && p < sp::kCount ? params_[static_cast<std::size_t>(p)] : 0;
}

bool Stream::setParam(const int p, const std::int32_t value) noexcept {
    if (p < 0 || p >= sp::kCount) {
        return false;
    }
    params_[static_cast<std::size_t>(p)] = store(kStreamKinds[static_cast<std::size_t>(p)], value);
    return true;
}

void Stream::allParams(std::uint8_t* out) const noexcept {
    const auto b = [](const std::int32_t v) { return static_cast<std::uint8_t>(v & 0xff); };
    out[0] = 22;
    out[1] = 0;
    out[2] = b(param(sp::kKeyPlus));
    out[3] = b(param(sp::kVelPlus));
    const auto t = static_cast<std::uint32_t>(param(sp::kTimePlus));
    for (int i = 0; i < 4; ++i) {
        out[4 + i] = static_cast<std::uint8_t>((t >> (8 * i)) & 0xffU);
    }
    out[8] = b(param(sp::kPosition));
    out[9] = b(param(sp::kPosition) >> 8);
    out[10] = b(param(sp::kPort));
    out[11] = b(param(sp::kChannel));
    out[12] = b(param(sp::kPatch));
    out[13] = b(param(sp::kVolume));
    out[14] = b(param(sp::kPan));
    // byte 15 is left as it was
    out[16] = b(param(sp::kMuted));
    out[17] = b(param(sp::kArchived));
    out[18] = 0;
    out[19] = 0;
    out[20] = b(param(sp::kLoopCount));
    out[21] = b(param(sp::kLoopCount) >> 8);
}

Tick Stream::duration() const noexcept {
    // The time of the last event; a note's duration does not count.
    Tick end = 0;
    for (const Event& e : events_) {
        end = std::max(end, e.time);
    }
    return end;
}

// The engine finds times by halving, as if the stream were sorted; on an unsorted stream the answer
// is where that search ends.

std::size_t Stream::indexAtOrAfter(const Tick tick) const noexcept {
    const auto it = std::ranges::lower_bound(events_, tick, {}, &Event::time);
    return static_cast<std::size_t>(it - events_.begin());
}

std::size_t Stream::indexAfter(const Tick tick) const noexcept {
    const auto it = std::ranges::upper_bound(events_, tick, {}, &Event::time);
    return static_cast<std::size_t>(it - events_.begin());
}

bool Stream::hasEventsIn(const Tick from, const Tick to) const noexcept {
    return std::ranges::any_of(events_, [from, to](const Event& e) { return e.time >= from && e.time < to; });
}

bool Stream::put(const Event& e) {
    if (cursor >= events_.size()) {
        return false;
    }
    events_[cursor] = e;
    return true;
}

void Stream::insert(const Event& e) {
    events_.insert(events_.begin() + static_cast<std::ptrdiff_t>(std::min(cursor, events_.size())), e);
}

bool Stream::erase() {
    if (cursor >= events_.size()) {
        return false;
    }
    events_.erase(events_.begin() + static_cast<std::ptrdiff_t>(cursor));
    return true;
}

std::size_t Stream::move(const Tick tick) {
    if (cursor >= events_.size()) {
        return cursor;
    }
    Event e = events_[cursor];
    e.time = tick;
    events_.erase(events_.begin() + static_cast<std::ptrdiff_t>(cursor));
    const std::size_t at = indexAfter(tick);
    events_.insert(events_.begin() + static_cast<std::ptrdiff_t>(at), e);
    return at;
}

std::size_t Stream::sort() {
    // Shell's sort with the gaps n/2, n/4, ..., 1, which is why it is not stable
    const std::size_t n = events_.size();
    for (std::size_t gap = n / 2; gap > 0; gap /= 2) {
        for (std::size_t i = gap; i < n; ++i) {
            const Event e = events_[i];
            std::size_t j = i;
            while (j >= gap && events_[j - gap].time > e.time) {
                events_[j] = events_[j - gap];
                j -= gap;
            }
            events_[j] = e;
        }
    }
    return n;
}

Stream::RangeResult Stream::eraseRange(const std::size_t first, const std::size_t end) {
    if (first >= end) {
        return RangeResult::Empty;
    }
    if (end >= events_.size()) {
        // The tail goes: the stream ends at `first`, also when that is past its end (the new
        // events are zero).
        if (first == events_.size()) {
            return RangeResult::Unchanged;
        }
        events_.resize(first);
        return RangeResult::Done;
    }
    events_.erase(events_.begin() + static_cast<std::ptrdiff_t>(first),
                  events_.begin() + static_cast<std::ptrdiff_t>(end));
    cursor = std::min(cursor, events_.size());
    return RangeResult::Done;
}

std::size_t Stream::displaceEarlier(const Tick from, const Tick amount) {
    // The events from the first one at or after `from` on move; each is then put after the last
    // earlier event at its new time or before it (a halving search in the events before it). The
    // cursor follows the last event that changed its place.
    const std::size_t first = indexAtOrAfter(from);
    for (std::size_t i = first; i < events_.size(); ++i) {
        events_[i].time = (events_[i].time - amount) & Event::kMaxTime;
    }
    for (std::size_t i = first; i < events_.size(); ++i) {
        const auto begin = events_.begin();
        const auto at =
            std::ranges::upper_bound(begin, begin + static_cast<std::ptrdiff_t>(i), events_[i].time, {}, &Event::time);
        if (at != begin + static_cast<std::ptrdiff_t>(i)) {
            std::rotate(at, begin + static_cast<std::ptrdiff_t>(i), begin + static_cast<std::ptrdiff_t>(i + 1));
            cursor = static_cast<std::size_t>(at - begin);
        }
    }
    return events_.size();
}

std::size_t Stream::displaceLater(const Tick from, const Tick amount) {
    // The events from the first one at or after `from` on move, in 24 bits; nothing is sorted, so
    // a time that passes the end of the 24 bits leaves the stream out of order.
    for (std::size_t i = indexAtOrAfter(from); i < events_.size(); ++i) {
        events_[i].time = (events_[i].time + amount) & Event::kMaxTime;
    }
    return events_.size();
}

void Stream::insertRange(const std::size_t at, const std::vector<Event>& src) {
    events_.insert(events_.begin() + static_cast<std::ptrdiff_t>(std::min(at, events_.size())), src.begin(), src.end());
}

// ---------------------------------------------------------------- Sequence

Sequence::Sequence(const SeqHandle handle, std::array<std::int32_t, qp::kCount>* shared)
    : handle_(handle), shared_(shared) {
    // A new sequence (SeqCreateSeq), as TTSSEQ 1.01 sets it up.
    params_[qp::kNowJumped] = 1;
    params_[qp::kSendSpp] = 1;
    params_[qp::kUseContinue] = 1;
    params_[qp::kThruMode] = 2;
    params_[qp::kThruChannel] = -1;
    params_[qp::kLocalOnPort] = -1;
    params_[qp::kMetroRecord] = 1;
    params_[qp::kMetroAccent] = 1;
    params_[qp::kCountIn] = 1;
    params_[qp::kMetroChannel] = 9;
    params_[qp::kMetroKey] = 42;
    params_[qp::kMetroVelocity] = 110;
    params_[qp::kMetroDuration] = 1;
    params_[qp::kMetroSpeaker] = 1;
    params_[qp::kFilter] = 0xdb;
}

std::int32_t Sequence::param(const int p) const noexcept {
    if (p < 0 || p >= qp::kCount) {
        return 0;
    }
    const auto i = static_cast<std::size_t>(p);
    return shared_ != nullptr && isShared(p) ? (*shared_)[i] : params_[i];
}

bool Sequence::setParam(const int p, const std::int32_t value) noexcept {
    if (p < 0 || p >= qp::kCount || kSeqKinds[static_cast<std::size_t>(p)] == Kind::None) {
        return false;
    }
    const auto i = static_cast<std::size_t>(p);
    (shared_ != nullptr && isShared(p) ? *shared_ : params_)[i] = store(kSeqKinds[i], value);
    return true;
}

std::vector<Stream*> Sequence::streams() const {
    std::vector<Stream*> v;
    v.reserve(streams_.size());
    for (const auto& [id, s] : streams_) {
        v.push_back(s.get());
    }
    return v;
}

Stream* Sequence::stream(const StreamHandle handle) const noexcept {
    if (seqOf(handle) != handle_) {
        return nullptr;
    }
    const auto it = streams_.find(idOf(handle));
    return it == streams_.end() ? nullptr : it->second.get();
}

std::uint8_t Sequence::freeId() const {
    for (unsigned id = 0; id < 256; ++id) {
        if (!streams_.contains(static_cast<std::uint8_t>(id))) {
            return static_cast<std::uint8_t>(id);
        }
    }
    throw Error("no free stream id");
}

Stream& Sequence::createStream() {
    const std::uint8_t id = freeId();
    auto s = std::make_unique<Stream>(makeStreamHandle(handle_, id));
    Stream& ref = *s;
    streams_.emplace(id, std::move(s));
    return ref;
}

Stream& Sequence::streamWithId(const std::uint8_t id) {
    auto& slot = streams_[id];
    if (!slot) {
        slot = std::make_unique<Stream>(makeStreamHandle(handle_, id));
    }
    return *slot;
}

bool Sequence::destroyStream(const StreamHandle handle) {
    return stream(handle) != nullptr && streams_.erase(idOf(handle)) == 1;
}

Stream& Sequence::clone(const Stream& source, const bool copyParams) {
    Stream& s = createStream();
    s.events_ = source.events_;
    s.name_ = source.name_;
    if (copyParams) {
        s.params_ = source.params_;
    }
    return s;
}

bool Sequence::setPpq(const std::uint16_t newPpq) {
    if (newPpq == 0 || newPpq > kMaxPpq) {
        return false;
    }
    for (const auto& [id, s] : streams_) {
        for (Event& e : s->events_) {
            e.time = rescaleTime(e.time, ppq, newPpq);
            e.duration = static_cast<std::uint16_t>(rescaleTime(e.duration, ppq, newPpq));
        }
    }
    tempoMap.rescale(ppq, newPpq);
    ppq = newPpq;
    return true;
}

Tick Sequence::markerTime(const Marker& m) const noexcept {
    return m.smpteLocked ? frameToTick(m.frame, false) : m.time;
}

void Sequence::setMarkerTime(Marker& m, const Tick tick, const bool locked) const noexcept {
    m.smpteLocked = locked;
    m.time = tick;
    m.frame = locked ? tickToFrame(tick) : 0;
}

void Sequence::insertMarker(Marker m) {
    const Tick t = markerTime(m);
    const auto at = std::ranges::find_if(markers, [this, t](const Marker& x) { return markerTime(x) > t; });
    markers.insert(at, std::move(m));
}

void Sequence::sortMarkers() {
    std::ranges::stable_sort(markers, {}, [this](const Marker& x) { return markerTime(x); });
}

std::size_t Sequence::markerForTime(const Tick tick) const noexcept {
    std::size_t i = 0;
    while (i + 1 < markers.size() && markerTime(markers[i + 1]) <= tick) {
        ++i;
    }
    return i;
}

bool Sequence::setFps(const std::uint16_t newFps) {
    if (!validFps(newFps)) {
        return false;
    }
    fps = newFps;
    return true;
}

namespace {

/// Frames per second as a fraction: 29 is 29.97.
struct Rate {
    std::uint64_t num;
    std::uint64_t den;
};

Rate rateOf(const std::uint16_t fps) noexcept {
    return fps == 29 ? Rate{2997, 100} : Rate{fps, 1};
}

constexpr std::uint64_t kMicro = 1'000'000;

} // namespace

std::uint32_t Sequence::tickToFrame(const Tick tick) const noexcept {
    // frames = elapsed * rate / (1e6 * ppq), rounded; split so that nothing overflows
    const std::int64_t elapsed = std::max<std::int64_t>(tempoMap.elapsedAt(tick), 0);
    const Rate r = rateOf(fps);
    const std::uint64_t d = kMicro * ppq;
    const auto e = static_cast<std::uint64_t>(elapsed);
    const std::uint64_t a = (e / d) * r.num;
    const std::uint64_t num = (a % r.den) * d + (e % d) * r.num;
    const std::uint64_t frames = a / r.den + (2 * num + r.den * d) / (2 * r.den * d);
    return static_cast<std::uint32_t>(frames + frameOffset);
}

Tick Sequence::frameToTick(const std::uint32_t frame, const bool up) const noexcept {
    if (frame < frameOffset) {
        return 0;
    }
    // elapsed = frames * 1e6 * ppq / rate, kept as a whole part and a fraction of rate.num
    const Rate r = rateOf(fps);
    const std::uint64_t d = kMicro * ppq;
    const std::uint64_t b = std::uint64_t{frame - frameOffset} * r.den;
    const std::uint64_t rest = (b % r.num) * d;
    return tempoMap.tickAt((b / r.num) * d + rest / r.num, rest % r.num, r.num, up);
}

StreamHandle Sequence::createSysxBank() {
    for (unsigned id = 0; id < 256; ++id) {
        if (!sysxBanks.contains(static_cast<std::uint8_t>(id))) {
            sysxBanks.emplace(static_cast<std::uint8_t>(id), SysxBank{});
            return makeStreamHandle(handle_, static_cast<std::uint8_t>(id));
        }
    }
    throw Error("no free sysex bank id");
}

SysxBank* Sequence::sysxBank(const StreamHandle handle) noexcept {
    if (seqOf(handle) != handle_) {
        return nullptr;
    }
    const auto it = sysxBanks.find(idOf(handle));
    return it == sysxBanks.end() ? nullptr : &it->second;
}

// ---------------------------------------------------------------- sysex banks

std::int32_t SysxBank::param(const int p) const noexcept {
    switch (p) {
    case 1:
        return autoSend ? 1 : 0;
    case 2:
        return number;
    case 3:
        return port;
    default:
        return 0;
    }
}

bool SysxBank::setParam(const int p, const std::int32_t value) noexcept {
    switch (p) {
    case 1:
        autoSend = value != 0;
        return true;
    case 2:
        number = static_cast<std::int16_t>(value & 0xffff);
        return true;
    case 3:
        port = static_cast<std::int16_t>(value & 0xffff);
        return true;
    default:
        return false;
    }
}

std::string sysxToText(const std::vector<std::uint8_t>& data, const std::size_t perLine) {
    static constexpr std::string_view kHex = "0123456789ABCDEF";
    std::string t;
    for (std::size_t i = 0; i < data.size(); ++i) {
        t += kHex[data[i] >> 4];
        t += kHex[data[i] & 0x0fU];
        t += ' ';
        if ((i + 1) % perLine == 0) {
            t += "\r\n";
        }
    }
    return t;
}

namespace {

int hexDigit(const char c) noexcept {
    const auto u = static_cast<unsigned char>(c);
    if (std::isdigit(u) != 0) {
        return c - '0';
    }
    if (std::isxdigit(u) != 0) {
        return std::toupper(u) - 'A' + 10;
    }
    return -1;
}

} // namespace

SysxParse sysxFromText(const std::string& text) {
    // groups of hex digits between white space, read two digits to a byte (a last single digit
    // is a byte of its own)
    SysxParse r;
    std::size_t i = 0;
    while (i < text.size()) {
        if (std::isspace(static_cast<unsigned char>(text[i])) != 0) {
            ++i;
            continue;
        }
        unsigned value = 0;
        int digits = 0;
        while (i < text.size() && std::isspace(static_cast<unsigned char>(text[i])) == 0) {
            const int d = hexDigit(text[i]);
            if (d < 0) {
                r.result = SysxParse::NotHex;
                r.position = i;
                return r;
            }
            value = value * 16 + static_cast<unsigned>(d);
            if (++digits == 2) {
                r.data.push_back(static_cast<std::uint8_t>(value));
                value = 0;
                digits = 0;
            }
            ++i;
        }
        if (digits == 1) {
            r.data.push_back(static_cast<std::uint8_t>(value));
        }
    }
    if (r.data.empty() || r.data.front() != 0xf0) {
        r.result = SysxParse::NoF0;
    } else if (r.data.back() != 0xf7) {
        r.result = SysxParse::NoF7;
    }
    return r;
}

// ---------------------------------------------------------------- Engine

Engine::Engine() : player_(std::make_unique<Player>(*this)) {
    const Sequence fresh(0);
    for (int p = qp::kMetroPort; p <= qp::kMetroDuration; ++p) {
        shared_[static_cast<std::size_t>(p)] = fresh.param(p);
    }
}
Engine::~Engine() = default;

// NOLINTNEXTLINE(readability-make-member-function-const): it changes the sequences the engine owns
StreamHandle Engine::mergeStreams(const SeqHandle seq, const StreamHandle a, const StreamHandle b, const bool destroyB,
                                  const Tick offset) {
    Sequence* dst = sequence(seq);
    const Stream* sa = a == 0 ? nullptr : stream(a);
    const Stream* sb = stream(b);
    if (dst == nullptr || sb == nullptr || (a != 0 && sa == nullptr)) {
        return 0;
    }
    Stream& m = dst->createStream();
    std::vector<Event> later = sb->events();
    for (Event& e : later) {
        e.time += offset;
    }
    const std::vector<Event> none;
    const std::vector<Event>& first = sa == nullptr ? none : sa->events();
    std::ranges::merge(first, later, std::back_inserter(m.events_),
                       [](const Event& x, const Event& y) { return x.time < y.time; });
    if (sa != nullptr) {
        m.name_ = sa->name_;
        m.params_ = sa->params_;
    }
    const StreamHandle h = m.handle();
    if (a != 0) {
        sequence(seqOf(a))->destroyStream(a);
    }
    if (destroyB) {
        sequence(seqOf(b))->destroyStream(b);
    }
    return h;
}

SeqHandle Engine::freeHandle() const {
    for (SeqHandle h = 1; h < 256; ++h) {
        if (!sequences_.contains(h)) {
            return h;
        }
    }
    throw Error("no free sequence handle");
}

SeqHandle Engine::createSequence() {
    const SeqHandle h = freeHandle();
    sequences_.emplace(h, std::make_unique<Sequence>(h, &shared_));
    player_->created();
    if (current_ == 0) {
        current_ = h;
    }
    return h;
}

bool Engine::destroyStream(const StreamHandle handle) {
    Sequence* s = sequence(seqOf(handle));
    Stream* st = s == nullptr ? nullptr : s->stream(handle);
    if (st == nullptr) {
        return false;
    }
    freeStringEvents(*st);
    return s->destroyStream(handle);
}

void Engine::send(const int port, const std::span<const std::uint8_t> message) {
    if (platform_ != nullptr && !message.empty()) {
        platform_->send(port, message);
    }
}

bool Engine::setStreamParam(const StreamHandle handle, const int param, const std::int32_t value) {
    Stream* st = stream(handle);
    if (st == nullptr || !st->setParam(param, value)) {
        return false;
    }
    sendStreamParam(*st, param);
    return true;
}

int Engine::outputChannel(const Stream& stream) noexcept {
    if (const int forced = stream.param(sp::kChannel); forced >= 0) {
        return forced & 0x0f;
    }
    for (const Event& e : stream.events()) {
        if (e.status >= 0x80 && e.status < 0xf0) {
            return e.status & 0x0f;
        }
    }
    return -1;
}

void Engine::sendStreamParam(const Stream& stream, const int param, const bool anySequence) {
    const std::int32_t v = stream.param(param);
    const int channel = outputChannel(stream);
    const int port = stream.param(sp::kPort);
    if (v < 0 || channel < 0 || platform_ == nullptr || port >= platform_->outputPorts() ||
        (!anySequence && seqOf(stream.handle()) != current_)) {
        return;
    }
    const auto value = static_cast<std::uint8_t>(v & 0x7f);
    const auto ch = static_cast<unsigned>(channel);
    if (param == sp::kPatch) {
        const std::array<std::uint8_t, 2> m{static_cast<std::uint8_t>(0xc0U | ch), value};
        send(port, m);
    } else if (param == sp::kVolume || param == sp::kPan) {
        const std::array<std::uint8_t, 3> m{static_cast<std::uint8_t>(0xb0U | ch),
                                            static_cast<std::uint8_t>(param == sp::kVolume ? 7 : 10), value};
        send(port, m);
    }
}

void Engine::timer() {
    if (platform_ != nullptr) {
        player_->advance(platform_->milliseconds());
    }
    codeTaken_ = false;
    code_.inRun = false;
    takeInput();
    player_->timer();
    // the run's channel messages (thru and the take) come after its playback and metronome
    takeChannelInput();
}

void Engine::restartTimer() {
    if (platform_ != nullptr) {
        player_->timerStarted(platform_->milliseconds());
        platform_->startTimer(timerPeriod_);
    }
}

bool Engine::setPlayingSeq(const SeqHandle seq) {
    if (seq != 0 && sequence(seq) == nullptr) {
        return false;
    }
    current_ = seq;
    restartTimer(); // 1.01 starts its timer again (timeKillEvent, timeSetEvent)
    const Sequence* s = sequence(seq);
    for (int p = qp::kThruMode; p <= qp::kLocalOnPort; ++p) {
        thru_[static_cast<std::size_t>(p - qp::kThruMode)] = s != nullptr ? s->param(p) : 0;
    }
    return true;
}

bool Engine::setSeqParam(const SeqHandle seq, const int param, const std::int32_t value) {
    Sequence* s = sequence(seq);
    if (s == nullptr || !s->setParam(param, value)) {
        return false;
    }
    if (seq == current_ && param >= qp::kThruMode && param <= qp::kLocalOnPort) {
        thru_[static_cast<std::size_t>(param - qp::kThruMode)] = s->param(param);
    }
    return true;
}

bool Engine::setStandbySeq(const SeqHandle seq) {
    if (seq != 0 && sequence(seq) == nullptr) {
        return false;
    }
    standby_ = seq;
    return true;
}

Tick Sequence::end() const noexcept {
    Tick end = 0;
    for (const Stream* st : streams()) {
        if (!st->events().empty()) {
            end = std::max(end, st->events().back().time);
        }
    }
    return end;
}

Tick Engine::setNow(const SeqHandle seq, const Tick tick) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    if (player_->playing() && player_->sequence() == s) {
        player_->jump(tick);
    } else {
        s->now = tick;
    }
    s->setParam(qp::kNowJumped, 1);
    return tick;
}

bool Engine::playStart(const StartProc& callback) {
    Sequence* s = sequence(current_);
    if (s == nullptr) {
        return false;
    }
    if (clockSource_ == kSourceMidi) {
        return startOnClock(*s, callback);
    }
    if (clockSource_ == kSourceCode) {
        return startOnCode(*s, callback);
    }
    return player_->start(*s);
}

bool Engine::playStop() {
    return player_->stop();
}

std::uint16_t Engine::idle() {
    return player_->idle();
}

bool Engine::expired() const noexcept {
    return !player_->playing();
}

void Engine::open() {
    thru_ = {2, 0, -1, 0, 0, -1};
    if (platform_ != nullptr) {
        player_->opened(platform_->milliseconds());
        platform_->opened(*this);
        platform_->startTimer(timerPeriod_);
    }
    const int ports = platform_ != nullptr ? platform_->outputPorts() : 0;
    for (int port = 0; port < ports; ++port) {
        for (unsigned ch = 0; ch < 16; ++ch) {
            const std::array<std::uint8_t, 3> localOff{static_cast<std::uint8_t>(0xb0U | ch), 122, 0};
            send(port, localOff);
        }
    }
}

void Engine::close() {
    while (!sequences_.empty()) {
        destroySequence(sequences_.begin()->first);
    }
    notes_.clear();
    // 1.01 sends nothing itself here: it resets its output ports (midiOutReset), and what goes out
    // then is the driver's (Wine's sends All Sound Off and sustain off on every channel)
    if (platform_ != nullptr) {
        platform_->stopTimer();
        platform_->closed();
    }
}

std::uint8_t Engine::registerEventKind(std::string name, EventProc proc, const std::uint32_t value) {
    for (unsigned k = 1; k < 0x80; ++k) {
        const auto id = static_cast<std::uint8_t>(k);
        if (!kinds_.contains(id)) {
            kindNames_[id] = name;
            kinds_[id] = EventKind{std::move(name), std::move(proc), value};
            return id;
        }
    }
    return 0;
}

bool Engine::unregisterEventKind(const std::string& name) {
    for (auto it = kinds_.begin(); it != kinds_.end(); ++it) {
        if (it->second.name == name) {
            kindNames_.erase(it->first);
            kinds_.erase(it);
            return true;
        }
    }
    return false;
}

const Engine::EventKind* Engine::kindOf(const std::uint8_t status) const noexcept {
    const auto it = kinds_.find(status);
    return it == kinds_.end() ? nullptr : &it->second;
}

void Engine::playStringEvent(const Event& event) {
    const EventKind* k = kindOf(event.status);
    if (k != nullptr && k->proc && (k->value & kNoPlay) == 0) {
        Event copy = event; // the procedure gets a copy of the event
        k->proc(ek::kPlay, copy, 0);
    }
}

std::uint32_t Engine::callEventProc(const std::uint16_t message, Event& event, const std::uint16_t length) {
    const EventKind* k = kindOf(event.status);
    return k == nullptr || !k->proc ? 0 : k->proc(message, event, length);
}

void Engine::freeStringEvents(Stream& stream) {
    for (Event& e : stream.events()) {
        const EventKind* k = kindOf(e.status);
        if (k != nullptr && k->proc && (k->value & kNoFree) == 0) {
            k->proc(ek::kFree, e, 0);
        }
    }
}

bool Engine::destroySequence(const SeqHandle handle) {
    Sequence* s = sequence(handle);
    if (s == nullptr) {
        return false;
    }
    for (Stream* st : s->streams()) {
        freeStringEvents(*st);
    }
    player_->forget(*s);
    if (standby_ == handle) {
        standby_ = 0;
    }
    if (handle == current_) {
        current_ = 0;
    }
    sequences_.erase(handle);
    return true;
}

std::uint32_t Engine::ioSeek(const std::int32_t offset, const int origin) {
    if (!io_.open) {
        return 0;
    }
    std::int64_t base = 0;
    if (origin == 1) {
        base = static_cast<std::int64_t>(io_.pos);
    } else if (origin == 2) {
        base = static_cast<std::int64_t>(io_.data.size());
    }
    io_.pos = static_cast<std::size_t>(std::max<std::int64_t>(base + offset, 0));
    return static_cast<std::uint32_t>(io_.pos);
}

std::size_t Engine::ioRead(std::uint8_t* to, const std::size_t count) {
    if (!io_.open || io_.pos >= io_.data.size()) {
        return 0;
    }
    const std::size_t n = std::min(count, io_.data.size() - io_.pos);
    std::copy_n(io_.data.begin() + static_cast<std::ptrdiff_t>(io_.pos), n, to);
    io_.pos += n;
    return n;
}

std::size_t Engine::ioWrite(const std::uint8_t* from, const std::size_t count) {
    if (!io_.open) {
        return 0;
    }
    if (io_.data.size() < io_.pos + count) {
        io_.data.resize(io_.pos + count);
    }
    std::copy_n(from, count, io_.data.begin() + static_cast<std::ptrdiff_t>(io_.pos));
    io_.pos += count;
    return count;
}

Sequence* Engine::sequence(const SeqHandle handle) const noexcept {
    const auto it = sequences_.find(handle);
    return it == sequences_.end() ? nullptr : it->second.get();
}

Stream* Engine::stream(const StreamHandle handle) const noexcept {
    const Sequence* s = sequence(seqOf(handle));
    return s == nullptr ? nullptr : s->stream(handle);
}

// NOLINTNEXTLINE(readability-make-member-function-const): it changes the sequences the engine owns
StreamHandle Engine::adoptStream(const SeqHandle seq, const StreamHandle stream) {
    Sequence* dst = sequence(seq);
    Sequence* src = sequence(seqOf(stream));
    if (dst == nullptr || src == nullptr || src->stream(stream) == nullptr) {
        return 0;
    }
    const std::uint8_t id = dst->freeId();
    auto node = src->streams_.extract(idOf(stream));
    std::unique_ptr<Stream> s = std::move(node.mapped());
    s->handle_ = makeStreamHandle(seq, id);
    const StreamHandle h = s->handle_;
    dst->streams_.emplace(id, std::move(s));
    return h;
}

} // namespace libseq
