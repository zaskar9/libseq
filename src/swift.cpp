// libseq::SharedEngine (include/libseq/swift.hpp).
#include "libseq/swift.hpp"

#include "cplatform.hpp"

#include <algorithm>
#include <span>
#include <string>
#include <vector>

namespace libseq {

namespace {

/// A string result: as much as fits in the buffer, with its 0; the whole length.
std::size_t put(const std::string& s, char* buffer, const std::size_t size) {
    if (buffer != nullptr && size > 0) {
        const std::size_t n = std::min(s.size(), size - 1);
        std::copy_n(s.data(), n, buffer);
        buffer[n] = 0;
    }
    return s.size();
}

std::string str(const char* s) {
    return s != nullptr ? std::string(s) : std::string();
}

/// The engine owns the platform it is given, here: the platform goes after the engine.
struct Owned {
    std::shared_ptr<CPlatform> platform = std::make_shared<CPlatform>();
    Engine engine;
};

} // namespace

SharedEngine::SharedEngine() {
    auto owned = std::make_shared<Owned>();
    platform_ = owned->platform;
    engine_ = std::shared_ptr<Engine>(owned, &owned->engine);
}

void SharedEngine::setPlatform(const LIBSEQPLATFORM& platform) const {
    platform_->install(&platform);
    engine_->setPlatform(platform_.get());
}

void SharedEngine::open() const {
    engine_->open();
}

void SharedEngine::close() const {
    engine_->close();
}

bool SharedEngine::idle() const {
    return engine_->idle() != 0;
}

void SharedEngine::timer() const {
    engine_->timer();
}

void SharedEngine::receive(const std::uint8_t* message, const std::size_t length) const {
    if (message != nullptr && length > 0) {
        engine_->receive(std::span<const std::uint8_t>(message, length));
    }
}

std::size_t SharedEngine::portName(const int port, char* buffer, const std::size_t size) const {
    return put(engine_->portName(port), buffer, size);
}

int SharedEngine::fileError() const {
    return engine_->fileError();
}

SeqHandle SharedEngine::createSequence() const {
    return engine_->createSequence();
}

bool SharedEngine::destroySequence(const SeqHandle seq) const {
    return engine_->destroySequence(seq);
}

SeqHandle SharedEngine::loadCakewalkFile(const char* path) const {
    return engine_->loadCakewalkFile(str(path));
}

bool SharedEngine::saveCakewalkFile(const SeqHandle seq, const char* path) const {
    return engine_->saveCakewalkFile(seq, str(path));
}

SeqHandle SharedEngine::loadMidiFile(const char* path) const {
    return engine_->loadMidiFile(str(path));
}

bool SharedEngine::saveMidiFile(const SeqHandle seq, const char* path) const {
    return engine_->saveMidiFile(seq, str(path));
}

std::int32_t SharedEngine::seqParam(const SeqHandle seq, const int param) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->param(param);
}

bool SharedEngine::setSeqParam(const SeqHandle seq, const int param, const std::int32_t value) const {
    return engine_->setSeqParam(seq, param, value);
}

std::uint16_t SharedEngine::ppq(const SeqHandle seq) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->ppq;
}

bool SharedEngine::setPpq(const SeqHandle seq, const std::uint16_t ppq) const {
    Sequence* s = engine_->sequence(seq);
    return s != nullptr && s->setPpq(ppq);
}

Tick SharedEngine::now(const SeqHandle seq) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->now;
}

Tick SharedEngine::setNow(const SeqHandle seq, const Tick tick) const {
    return engine_->setNow(seq, tick);
}

Tick SharedEngine::endTick(const SeqHandle seq) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->end();
}

std::uint16_t SharedEngine::tempoAt(const SeqHandle seq, const Tick tick) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->tempoMap.tempoAt(tick);
}

bool SharedEngine::setTempo(const SeqHandle seq, const Tick tick, const std::uint16_t tempo) const {
    Sequence* s = engine_->sequence(seq);
    if (s == nullptr) {
        return false;
    }
    s->tempoMap.set(tick, tempo);
    return true;
}

bool SharedEngine::insertTempo(const SeqHandle seq, const Tick tick, const std::uint16_t tempo) const {
    Sequence* s = engine_->sequence(seq);
    if (s == nullptr) {
        return false;
    }
    s->tempoMap.insert(tick, tempo);
    return true;
}

MeterEntry SharedEngine::meterAt(const SeqHandle seq, const Tick tick) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? MeterEntry{} : s->meterMap.entries()[s->meterMap.indexAtTick(tick)];
}

bool SharedEngine::insertMeter(const SeqHandle seq, const std::uint16_t measure, const std::int8_t key,
                               const std::uint8_t numerator, const std::uint8_t denominatorLog2) const {
    Sequence* s = engine_->sequence(seq);
    if (s == nullptr) {
        return false;
    }
    const auto meter = static_cast<std::uint16_t>((denominatorLog2 << 8) | numerator);
    s->meterMap.insert(measure, key, meter, s->ppq);
    return true;
}

MBT SharedEngine::toMBT(const SeqHandle seq, const Tick tick) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? MBT{} : s->meterMap.toMBT(tick, s->ppq);
}

Tick SharedEngine::toTick(const SeqHandle seq, const MBT& position) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->meterMap.toTick(position.measure, position.beat, position.tick, s->ppq);
}

std::size_t SharedEngine::formatPosition(const SeqHandle seq, const Tick tick, char* buffer,
                                         const std::size_t size) const {
    const Sequence* s = engine_->sequence(seq);
    return put(s == nullptr ? std::string() : formatMBT(s->meterMap.toMBT(tick, s->ppq), false), buffer, size);
}

std::size_t SharedEngine::streamCount(const SeqHandle seq) const {
    const Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->streams().size();
}

StreamHandle SharedEngine::streamAt(const SeqHandle seq, const std::size_t index) const {
    const Sequence* s = engine_->sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    const std::vector<Stream*> all = s->streams();
    return index < all.size() ? all[index]->handle() : StreamHandle{0};
}

StreamHandle SharedEngine::createStream(const SeqHandle seq) const {
    Sequence* s = engine_->sequence(seq);
    return s == nullptr ? 0 : s->createStream().handle();
}

bool SharedEngine::destroyStream(const StreamHandle stream) const {
    return engine_->destroyStream(stream);
}

std::size_t SharedEngine::streamName(const StreamHandle stream, char* buffer, const std::size_t size) const {
    const Stream* s = engine_->stream(stream);
    return put(s == nullptr ? std::string() : s->name(), buffer, size);
}

bool SharedEngine::setStreamName(const StreamHandle stream, const char* name) const {
    Stream* s = engine_->stream(stream);
    if (s == nullptr) {
        return false;
    }
    s->setName(str(name));
    return true;
}

std::int32_t SharedEngine::streamParam(const StreamHandle stream, const int param) const {
    const Stream* s = engine_->stream(stream);
    return s == nullptr ? 0 : s->param(param);
}

bool SharedEngine::setStreamParam(const StreamHandle stream, const int param, const std::int32_t value) const {
    return engine_->setStreamParam(stream, param, value);
}

std::size_t SharedEngine::eventCount(const StreamHandle stream) const {
    const Stream* s = engine_->stream(stream);
    return s == nullptr ? 0 : s->events().size();
}

Event SharedEngine::eventAt(const StreamHandle stream, const std::size_t index) const {
    const Stream* s = engine_->stream(stream);
    return s == nullptr || index >= s->events().size() ? Event{} : s->events()[index];
}

std::size_t SharedEngine::insertEvent(const StreamHandle stream, const Event& event) const {
    Stream* s = engine_->stream(stream);
    if (s == nullptr) {
        return 0;
    }
    s->cursor = s->indexAfter(event.time);
    const std::size_t at = s->cursor;
    s->insert(event);
    return at;
}

bool SharedEngine::removeEvent(const StreamHandle stream, const std::size_t index) const {
    Stream* s = engine_->stream(stream);
    if (s == nullptr || index >= s->events().size()) {
        return false;
    }
    s->cursor = index;
    return s->erase();
}

bool SharedEngine::setPlayingSeq(const SeqHandle seq) const {
    return engine_->setPlayingSeq(seq);
}

bool SharedEngine::playStart() const {
    return engine_->playStart();
}

bool SharedEngine::playStop() const {
    return engine_->playStop();
}

bool SharedEngine::expired() const {
    return engine_->expired();
}

} // namespace libseq
