// libseq: sequences, streams and their parameters (TTSSEQ 1.01).
#include "libseq/engine.hpp"

#include <algorithm>

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

// ---------------------------------------------------------------- Sequence

Sequence::Sequence(const SeqHandle handle) : handle_(handle) {
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
    return p >= 0 && p < qp::kCount ? params_[static_cast<std::size_t>(p)] : 0;
}

bool Sequence::setParam(const int p, const std::int32_t value) noexcept {
    if (p < 0 || p >= qp::kCount || kSeqKinds[static_cast<std::size_t>(p)] == Kind::None) {
        return false;
    }
    params_[static_cast<std::size_t>(p)] = store(kSeqKinds[static_cast<std::size_t>(p)], value);
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

// ---------------------------------------------------------------- Engine

Engine::Engine() = default;
Engine::~Engine() = default;

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
    sequences_.emplace(h, std::make_unique<Sequence>(h));
    return h;
}

bool Engine::destroySequence(const SeqHandle handle) {
    return sequences_.erase(handle) == 1;
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
