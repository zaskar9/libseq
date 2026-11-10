// libseq: the TTSSEQ.DLL 1.01 interface in C (include/libseq/ttsseq.h), over libseq::Engine.
#include "libseq/ttsseq.h"

#include "libseq/engine.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using libseq::Engine;
using libseq::Event;
using libseq::Sequence;
using libseq::Stream;
using libseq::Tick;

namespace {

// ---- the platform the host installs, as a libseq::Platform

class CPlatform final : public libseq::Platform {
public:
    void install(const LIBSEQPLATFORM* p) noexcept {
        installed_ = p != nullptr;
        p_ = installed_ ? *p : LIBSEQPLATFORM{};
    }
    [[nodiscard]] bool installed() const noexcept { return installed_; }
    [[nodiscard]] const LIBSEQPLATFORM& c() const noexcept { return p_; }

    [[nodiscard]] int outputPorts() const override {
        return p_.outputPorts != nullptr ? p_.outputPorts(p_.context) : 0;
    }
    [[nodiscard]] std::string outputName(const int port) const override {
        const char* n = p_.outputName != nullptr ? p_.outputName(p_.context, port) : nullptr;
        return n != nullptr ? std::string(n) : std::string();
    }
    [[nodiscard]] libseq::Options options() const override {
        libseq::Options o;
        if (p_.options == nullptr) {
            return o;
        }
        LIBSEQOPTIONS c{o.syncOutputPort,  o.sendLocalOff,       o.timebase,           o.timerPeriod,
                        o.timerResolution, o.sysxSendPacketSize, o.sysxReceiveBuffers, o.sysxReceiveBuffersSize,
                        o.chaseDelay,      o.flywheelAmount};
        p_.options(p_.context, &c);
        o.syncOutputPort = c.syncOutputPort;
        o.sendLocalOff = c.sendLocalOff;
        o.timebase = c.timebase;
        o.timerPeriod = c.timerPeriod;
        o.timerResolution = c.timerResolution;
        o.sysxSendPacketSize = c.sysxSendPacketSize;
        o.sysxReceiveBuffers = c.sysxReceiveBuffers;
        o.sysxReceiveBuffersSize = c.sysxReceiveBuffersSize;
        o.chaseDelay = c.chaseDelay;
        o.flywheelAmount = c.flywheelAmount;
        return o;
    }
    void send(const int port, const std::span<const std::uint8_t> message) override {
        if (p_.send != nullptr) {
            p_.send(p_.context, port, message.data(), message.size());
        }
    }
    [[nodiscard]] std::uint32_t milliseconds() const override {
        return p_.milliseconds != nullptr ? p_.milliseconds(p_.context) : 0;
    }
    void opened(Engine& /*engine*/) override {
        if (p_.opened != nullptr) {
            p_.opened(p_.context);
        }
    }
    void closed() override {
        if (p_.closed != nullptr) {
            p_.closed(p_.context);
        }
    }
    void startTimer(const unsigned periodMs) override {
        if (p_.startTimer != nullptr) {
            p_.startTimer(p_.context, periodMs);
        }
    }
    void stopTimer() override {
        if (p_.stopTimer != nullptr) {
            p_.stopTimer(p_.context);
        }
    }
    void wait() override {
        if (p_.wait != nullptr) {
            p_.wait(p_.context);
        }
    }
    void postMessage(void* hwnd, const std::uint16_t message, const std::uint16_t wParam,
                     const std::uint32_t lParam) const {
        if (p_.postMessage != nullptr) {
            p_.postMessage(p_.context, hwnd, message, wParam, lParam);
        }
    }

private:
    LIBSEQPLATFORM p_{};
    bool installed_ = false;
};

CPlatform& platform() {
    static CPlatform p;
    return p;
}

// ---- the one engine of the process

struct Instance {
    Engine engine;
    /// Where SeqCallEventProc's caller has the event: the procedure gets that pointer.
    SEQEVENT* callerEvent = nullptr;
};

std::unique_ptr<Instance>& slot() {
    static std::unique_ptr<Instance> instance;
    return instance;
}

Instance& instance() {
    std::unique_ptr<Instance>& s = slot();
    if (!s) {
        s = std::make_unique<Instance>();
        s->engine.setPlatform(platform().installed() ? &platform() : nullptr);
    }
    return *s;
}

Engine& engine() {
    return instance().engine;
}

Sequence* sequence(const HSEQ seq) {
    return engine().sequence(seq);
}

Stream* findStream(const HSTREAM h) {
    return engine().stream(h);
}

libseq::SysxBank* findBank(const HSYSXBANK h) {
    Sequence* s = sequence(libseq::seqOf(h));
    return s == nullptr ? nullptr : s->sysxBank(h);
}

// ---- conversions

Event fromC(const SEQEVENT& c) noexcept {
    Event e;
    e.time = Tick{c.time[0]} | (Tick{c.time[1]} << 8) | (Tick{c.time[2]} << 16);
    e.status = c.status;
    e.data1 = c.data1;
    e.data2 = c.data2;
    e.duration = c.duration;
    return e;
}

SEQEVENT toC(const Event& e) noexcept {
    SEQEVENT c{};
    c.time[0] = static_cast<std::uint8_t>(e.time & 0xffU);
    c.time[1] = static_cast<std::uint8_t>((e.time >> 8) & 0xffU);
    c.time[2] = static_cast<std::uint8_t>((e.time >> 16) & 0xffU);
    c.status = e.status;
    c.data1 = e.data1;
    c.data2 = e.data2;
    c.duration = e.duration;
    return c;
}

/// Writes a string and its 0.
void putString(char* to, const std::string& s) {
    if (to != nullptr) {
        std::memcpy(to, s.c_str(), s.size() + 1);
    }
}

std::string str(const char* s) {
    return s != nullptr ? std::string(s) : std::string();
}

template <typename T>
void put(T* to, const T v) {
    if (to != nullptr) {
        *to = v;
    }
}

std::int8_t keyByte(const std::int16_t key) {
    return static_cast<std::int8_t>(static_cast<std::uint16_t>(key) & 0xffU);
}

/// The key as the engine passes it: the signed byte, not sign-extended (0x00FD for 3 flats).
std::uint16_t keyWord(const libseq::MeterEntry& m) {
    return static_cast<std::uint8_t>(m.key);
}

std::uint32_t meterResult(const libseq::MeterEntry& m) {
    return (std::uint32_t{keyWord(m)} << 16) | m.meter();
}

void putMBT(std::uint16_t* m, std::uint16_t* b, std::uint16_t* t, const libseq::MBT& v) {
    put(m, v.measure);
    put(b, v.beat);
    put(t, v.tick);
}

libseq::StartProc startProc(const SEQSTARTPROC proc) {
    if (proc == nullptr) {
        return {};
    }
    return [proc](const std::uint16_t code, const std::uint16_t a, const std::uint16_t b) -> std::uint32_t {
        return proc(code, a, b);
    };
}

libseq::FileProc fileProc(const SEQFILEPROC proc, const std::uint32_t value) {
    if (proc == nullptr) {
        return {};
    }
    return [proc, value](const std::uint16_t code, const std::uint16_t word) -> std::uint32_t {
        return proc(code, word, value);
    };
}

} // namespace

extern "C" {

// NOLINTBEGIN(readability-identifier-naming)

uint16_t SeqGetVersion(void) {
    return Engine::kVersion;
}

uint16_t SeqOpen(uint16_t /*reserved1*/, uint16_t /*reserved2*/) {
    engine().open();
    return 1;
}

uint16_t SeqClose(void) {
    engine().close();
    return 1;
}

uint16_t SeqIdle(void) {
    return engine().idle();
}

HSEQ SeqCreateSeq(uint16_t /*reserved1*/, uint16_t /*reserved2*/) {
    return engine().createSequence();
}

uint16_t SeqDestroySeq(const HSEQ seq) {
    engine().destroySequence(seq);
    return 1;
}

uint16_t SeqSetSeqParam(const HSEQ seq, const uint16_t param, const int32_t value) {
    return engine().setSeqParam(seq, param, value) ? 1 : 0;
}

int32_t SeqGetSeqParam(const HSEQ seq, const uint16_t param) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->param(param);
}

HSTREAM SeqCreateStream(const HSEQ seq, uint16_t /*reserved1*/, uint16_t /*reserved2*/) {
    Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->createStream().handle();
}

uint16_t SeqDestroyStream(const HSTREAM stream) {
    return engine().destroyStream(stream) ? 1 : 0;
}

uint32_t SeqEnumStreams(const HSEQ seq, const SEQENUMPROC proc, const uint32_t value) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    if (proc != nullptr) {
        for (const Stream* st : s->streams()) {
            proc(seq, st->handle(), value);
        }
        proc(0, 0, value);
    }
    return 1;
}

uint16_t SeqRegisterEventKind(const char* name, const SEQEVENTPROC proc, const uint32_t flags) {
    return engine().registerEventKind(
        str(name),
        [proc](const std::uint16_t message, Event& ev, const std::uint16_t length) -> std::uint32_t {
            if (proc == nullptr) {
                return 0;
            }
            // the event where the caller of SeqCallEventProc has it, otherwise a copy of its own
            Instance& in = instance();
            SEQEVENT local = toC(ev);
            SEQEVENT* at = in.callerEvent != nullptr ? in.callerEvent : &local;
            in.callerEvent = nullptr;
            *at = toC(ev);
            const std::uint16_t r = proc(message, at, length);
            ev = fromC(*at);
            return r;
        },
        flags);
}

uint16_t SeqUnregisterEventKind(const char* name) {
    return engine().unregisterEventKind(str(name)) ? 1 : 0;
}

uint16_t SeqCallEventProc(const uint16_t message, SEQEVENT* event, const uint16_t length) {
    if (event == nullptr) {
        return 0;
    }
    Instance& in = instance();
    Event ev = fromC(*event);
    in.callerEvent = event;
    const std::uint32_t r = in.engine.callEventProc(message, ev, length);
    in.callerEvent = nullptr;
    *event = toC(ev);
    return static_cast<std::uint16_t>(r & 0xffffU);
}

uint32_t SeqStreamLen(const HSTREAM stream) {
    const Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->events().size());
}

uint32_t SeqStreamDur(const HSTREAM stream) {
    const Stream* s = findStream(stream);
    return s == nullptr ? 0 : s->duration();
}

uint32_t SeqEvtAtTimeGE(const HSTREAM stream, const uint32_t tick) {
    const Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->indexAtOrAfter(tick));
}

uint32_t SeqEvtAtTimeGT(const HSTREAM stream, const uint32_t tick) {
    const Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->indexAfter(tick));
}

uint32_t SeqMeasHasEvts(const HSTREAM stream, const uint16_t measure) {
    const Stream* s = findStream(stream);
    const Sequence* q = sequence(libseq::seqOf(stream));
    if (s == nullptr || q == nullptr) {
        return 0;
    }
    const Tick from = q->meterMap.toTick(measure, 1, 0, q->ppq);
    const Tick to = q->meterMap.toTick(static_cast<std::uint16_t>(measure + 1), 1, 0, q->ppq);
    return s->hasEventsIn(from, to) ? 1 : 0;
}

uint32_t SeqTell(const HSTREAM stream) {
    const Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->cursor);
}

uint16_t SeqEos(const HSTREAM stream) {
    const Stream* s = findStream(stream);
    return s == nullptr || s->cursor >= s->events().size() ? 1 : 0;
}

uint32_t SeqSeek(const HSTREAM stream, uint16_t /*reserved*/, const uint32_t index) {
    Stream* s = findStream(stream);
    if (s == nullptr) {
        return 0;
    }
    s->cursor = std::min<std::size_t>(index, s->events().size());
    return static_cast<std::uint32_t>(s->cursor);
}

uint32_t SeqNxtEvt(const HSTREAM stream) {
    Stream* s = findStream(stream);
    if (s == nullptr || s->cursor >= s->events().size()) {
        return 0;
    }
    ++s->cursor;
    return s->cursor < s->events().size() ? 1 : 0;
}

uint16_t SeqGetEvt(const HSTREAM stream, SEQEVENT* event) {
    const Stream* s = findStream(stream);
    if (s == nullptr || event == nullptr || s->cursor >= s->events().size()) {
        return 0;
    }
    *event = toC(s->events()[s->cursor]);
    return 1;
}

uint16_t SeqGetEvtCopy(const HSTREAM stream, SEQEVENT* event) {
    return SeqGetEvt(stream, event);
}

uint16_t SeqPutEvt(const HSTREAM stream, const SEQEVENT* event) {
    Stream* s = findStream(stream);
    return s != nullptr && event != nullptr && s->put(fromC(*event)) ? 1 : 0;
}

uint16_t SeqInsEvt(const HSTREAM stream, const SEQEVENT* event) {
    Stream* s = findStream(stream);
    if (s == nullptr || event == nullptr) {
        return 0;
    }
    s->insert(fromC(*event));
    return 1;
}

uint16_t SeqDelEvt(const HSTREAM stream) {
    Stream* s = findStream(stream);
    return s != nullptr && s->erase() ? 1 : 0;
}

uint32_t SeqMoveEvt(const HSTREAM stream, const uint32_t tick) {
    Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->move(tick));
}

HSTREAM SeqCloneStream(const HSEQ seq, const HSTREAM stream, const uint16_t copyParams) {
    Sequence* dst = sequence(seq);
    const Stream* src = findStream(stream);
    return dst == nullptr || src == nullptr ? 0 : dst->clone(*src, copyParams != 0).handle();
}

HSTREAM SeqAdoptStream(const HSEQ seq, const HSTREAM stream) {
    return engine().adoptStream(seq, stream);
}

HSTREAM SeqMergeStreams(const HSEQ seq, const HSTREAM a, HSEQ /*seqB*/, const HSTREAM b, const uint16_t destroyB,
                        const uint32_t offset) {
    return engine().mergeStreams(seq, a, b, destroyB != 0, offset);
}

uint32_t SeqInsertRange(const HSTREAM dst, const uint32_t index, const HSTREAM src) {
    Stream* d = findStream(dst);
    const Stream* s = findStream(src);
    if (d == nullptr || s == nullptr) {
        return 0;
    }
    const std::vector<Event> copy = s->events();
    d->insertRange(index, copy);
    return 1;
}

uint32_t SeqDeleteRange(const HSTREAM stream, const uint32_t first, const uint32_t end) {
    Stream* s = findStream(stream);
    if (s == nullptr) {
        return 0;
    }
    // AX as the engine leaves it: the first index when the range is empty, the length when
    // nothing changes, else 1
    switch (s->eraseRange(first, end)) {
    case Stream::RangeResult::Empty:
        return first & 0xffffU;
    case Stream::RangeResult::Unchanged:
        return static_cast<std::uint32_t>(s->events().size()) & 0xffffU;
    case Stream::RangeResult::Done:
        break;
    }
    return 1;
}

uint32_t SeqDisplaceEarlier(const HSTREAM stream, const uint32_t from, const uint32_t amount) {
    Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->displaceEarlier(from, amount));
}

uint32_t SeqDisplaceLater(const HSTREAM stream, const uint32_t from, const uint32_t amount) {
    Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->displaceLater(from, amount));
}

uint32_t SeqTemporalSort(const HSTREAM stream) {
    Stream* s = findStream(stream);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->sort());
}

uint16_t SeqSetStreamParam(const HSTREAM stream, const uint16_t param, const int32_t value) {
    return engine().setStreamParam(stream, param, value) ? 1 : 0;
}

int32_t SeqGetStreamParam(const HSTREAM stream, const uint16_t param) {
    const Stream* s = findStream(stream);
    return s == nullptr ? 0 : s->param(param);
}

uint32_t SeqGetStreamAllParams(const HSTREAM stream, uint8_t* buffer) {
    const Stream* s = findStream(stream);
    if (s == nullptr || buffer == nullptr) {
        return 0;
    }
    s->allParams(buffer);
    return 22;
}

uint16_t SeqSetStreamName(const HSTREAM stream, const char* name) {
    Stream* s = findStream(stream);
    if (s == nullptr) {
        return 0;
    }
    s->setName(str(name));
    return 1;
}

uint16_t SeqGetStreamName(const HSTREAM stream, char* buffer, const uint16_t size) {
    const Stream* s = findStream(stream);
    if (s == nullptr || buffer == nullptr || size == 0) {
        return 0;
    }
    const std::size_t n = std::min<std::size_t>(s->name().size(), size - 1U);
    std::memcpy(buffer, s->name().data(), n);
    buffer[n] = 0;
    return 1;
}

uint16_t SeqGetMaxPPQ(void) {
    return libseq::kMaxPpq;
}

uint32_t SeqSetPPQ(const HSEQ seq, const uint16_t ppq) {
    Sequence* s = sequence(seq);
    return s != nullptr && s->setPpq(ppq) ? 1 : 0;
}

uint32_t SeqRescaleTime(const uint32_t time, const uint16_t fromPpq, const uint16_t toPpq) {
    return libseq::rescaleTime(time, fromPpq, toPpq);
}

uint16_t SeqGetPPQ(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->ppq;
}

uint16_t SeqGetMinTempo(void) {
    return libseq::kMinTempo;
}

uint16_t SeqGetMaxTempo(void) {
    return libseq::kMaxTempo;
}

uint16_t SeqClearTempoMap(const HSEQ seq, const uint16_t tempo) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->tempoMap.clear(tempo);
    return 1;
}

uint16_t SeqRecalcTempoMap(const HSEQ seq) {
    Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint16_t>(s->tempoMap.recalc());
}

uint16_t SeqGetTempoMapLen(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint16_t>(s->tempoMap.size());
}

uint32_t SeqTempoIxByTick(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->tempoMap.indexAt(tick));
}

uint32_t SeqGetTempoAtIx(const HSEQ seq, const uint16_t index, uint32_t* tick) {
    const Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->tempoMap.size()) {
        return 0xffff;
    }
    put(tick, s->tempoMap.entries()[index].tick);
    return s->tempoMap.entries()[index].tempo;
}

uint16_t SeqGetTempo(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->tempoMap.tempoAt(tick);
}

uint16_t SeqSetTempo(const HSEQ seq, const uint32_t tick, const uint16_t tempo) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->tempoMap.set(tick, tempo);
    return 1;
}

uint16_t SeqInsTempo(const HSEQ seq, const uint32_t tick, const uint16_t tempo) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->tempoMap.insert(tick, tempo);
    return 1;
}

uint16_t SeqDelTempo(const HSEQ seq, const uint32_t tick) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->tempoMap.remove(tick);
    return 1;
}

uint32_t SeqEnumTempos(const HSEQ seq, const uint32_t from, const uint32_t thru, const SEQENUMTEMPOPROC proc,
                       const uint32_t value) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    if (proc != nullptr) {
        // each entry's elapsed time (microseconds times the ppq, 32 bits of it), then (0, 0)
        const std::size_t n = s->tempoMap.enumCount(from, thru);
        for (std::size_t i = 0; i < n; ++i) {
            proc(static_cast<std::uint32_t>(s->tempoMap.entries()[i].elapsed), value);
        }
        proc(0, 0);
    }
    return 1;
}

uint16_t SeqTempoCopy(const HSEQ src, const HSEQ clip, const uint32_t from, const uint32_t thru, uint32_t /*reserved*/,
                      uint16_t* flag) {
    const Sequence* s = sequence(src);
    Sequence* c = sequence(clip);
    if (s == nullptr || c == nullptr) {
        return 0;
    }
    if (c->tempoMap.copyFrom(s->tempoMap, from, thru)) {
        put<std::uint16_t>(flag, 1);
    }
    return 1;
}

uint16_t SeqTempoPaste(const HSEQ clip, const HSEQ dst, const uint32_t at, const uint32_t length, const uint16_t reps,
                       const uint16_t slide) {
    const Sequence* c = sequence(clip);
    Sequence* d = sequence(dst);
    if (c == nullptr || d == nullptr) {
        return 0;
    }
    d->tempoMap.paste(c->tempoMap, at, length, reps, slide != 0);
    return 1;
}

uint16_t SeqSwapTempoMaps(const HSEQ a, const HSEQ b) {
    Sequence* x = sequence(a);
    Sequence* y = sequence(b);
    if (x != nullptr && y != nullptr) {
        std::swap(x->tempoMap, y->tempoMap);
    }
    return 0;
}

uint16_t SeqSetFps(const HSEQ seq, const uint16_t fps) {
    Sequence* s = sequence(seq);
    return s != nullptr && s->setFps(fps) ? 1 : 0;
}

uint16_t SeqGetFps(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->fps;
}

uint16_t SeqSetFrameOfs(const HSEQ seq, const uint32_t frames) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->frameOffset = frames;
    return 1;
}

uint32_t SeqGetFrameOfs(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->frameOffset;
}

uint32_t SeqTick2Frame(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->tickToFrame(tick);
}

uint32_t SeqFrame2TickRndOpt(const HSEQ seq, const uint32_t frame, const uint16_t option) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->frameToTick(frame, option != 0);
}

uint32_t SeqFrame2HMSF(const HSEQ seq, const uint32_t frame, uint16_t* hours, uint16_t* minutes, uint16_t* seconds,
                       uint16_t* frames) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    const libseq::HMSF t = libseq::framesToHMSF(frame, s->fps);
    put(hours, t.hours);
    put(minutes, t.minutes);
    put(seconds, t.seconds);
    put(frames, t.frames);
    return t.hours;
}

uint32_t SeqHMSF2Frame(const HSEQ seq, const uint16_t hours, const uint16_t minutes, const uint16_t seconds,
                       const uint16_t frames, uint16_t /*reserved*/) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    libseq::HMSF t;
    t.hours = hours;
    t.minutes = minutes;
    t.seconds = seconds;
    t.frames = frames;
    return libseq::hmsfToFrames(t, s->fps);
}

char* SeqTick2HMSFStr(const HSEQ seq, const uint32_t tick, char* buffer) {
    const Sequence* s = sequence(seq);
    if (s != nullptr) {
        putString(buffer, libseq::formatHMSF(libseq::framesToHMSF(s->tickToFrame(tick), s->fps)));
    }
    return buffer;
}

uint32_t SeqHMSFStr2HMSF(const char* string, uint16_t* hours, uint16_t* minutes, uint16_t* seconds, uint16_t* frames) {
    const libseq::HMSF t = libseq::parseHMSF(str(string));
    put(hours, t.hours);
    put(minutes, t.minutes);
    put(seconds, t.seconds);
    put(frames, t.frames);
    return 1;
}

uint32_t SeqHMSFStr2MBT(const HSEQ seq, const char* string, uint16_t* measure, uint16_t* beat, uint16_t* tick) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    const Tick t = s->frameToTick(libseq::hmsfToFrames(libseq::parseHMSF(str(string)), s->fps), false);
    putMBT(measure, beat, tick, s->meterMap.toMBT(t, s->ppq));
    return 1;
}

uint16_t SeqGetTempoRatio(const HSEQ seq, const uint16_t index) {
    const Sequence* s = sequence(seq);
    return s == nullptr || index >= s->tempoRatios.size() ? 0 : s->tempoRatios.at(index);
}

uint16_t SeqSetTempoRatio(const HSEQ seq, const uint16_t index, const uint16_t ratio) {
    Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->tempoRatios.size()) {
        return 0;
    }
    s->tempoRatios.at(index) = ratio;
    return 1;
}

uint16_t SeqSelectTempoRatio(const HSEQ seq, const uint16_t index) {
    Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->tempoRatios.size()) {
        return 0;
    }
    s->tempoRatio = index;
    return 1;
}

uint16_t SeqWhichTempoRatio(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->tempoRatio;
}

char* SeqTempoRatio2Str(const uint16_t ratio, char* buffer) {
    putString(buffer, libseq::tempoRatioString(ratio));
    return buffer;
}

uint16_t SeqClearMeterMap(const HSEQ seq, const int16_t key, const uint16_t meter) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->meterMap.clear(keyByte(key), meter);
    return 1;
}

uint32_t SeqRecalcMeterMap(const HSEQ seq) {
    Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->meterMap.recalc(s->ppq));
}

uint16_t SeqGetMeterMapLen(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint16_t>(s->meterMap.size());
}

uint16_t SeqMeterIxByTick(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint16_t>(s->meterMap.indexAtTick(tick));
}

uint32_t SeqGetMeterAtIx(const HSEQ seq, const uint16_t index, uint16_t* measure) {
    const Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->meterMap.size()) {
        return 0xffffffff;
    }
    const libseq::MeterEntry& m = s->meterMap.entries()[index];
    put(measure, static_cast<std::uint16_t>(m.measure + 1));
    return meterResult(m);
}

uint32_t SeqGetMeter(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : meterResult(s->meterMap.entries()[s->meterMap.indexAtTick(tick)]);
}

uint16_t SeqSetMeter(const HSEQ seq, const uint32_t tick, const int16_t key, const uint16_t meter) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->meterMap.set(tick, keyByte(key), meter, s->ppq);
    return 1;
}

uint16_t SeqInsMeter(const HSEQ seq, const uint16_t measure, const int16_t key, const uint16_t meter) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->meterMap.insert(measure, keyByte(key), meter, s->ppq);
    return 1;
}

uint16_t SeqDelMeter(const HSEQ seq, const uint16_t measure) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->meterMap.remove(measure);
    return 1;
}

uint32_t SeqEnumMeters(const HSEQ seq, const uint16_t from, const uint16_t thru, const SEQENUMMETERPROC proc,
                       const uint32_t value) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    if (proc != nullptr) {
        const std::size_t n = s->meterMap.enumCount(from, thru);
        for (std::size_t i = 0; i < n; ++i) {
            const libseq::MeterEntry& m = s->meterMap.entries()[i];
            proc(keyWord(m), m.meter(), value);
        }
        proc(0, 0, 0);
    }
    return 1;
}

uint16_t SeqMeterCopy(const HSEQ src, const HSEQ clip, const uint16_t from, const uint16_t thru, const uint16_t cut,
                      uint16_t /*reserved*/, uint16_t* flag) {
    Sequence* s = sequence(src);
    Sequence* c = sequence(clip);
    if (s == nullptr || c == nullptr) {
        return 0;
    }
    bool found = false;
    const bool ok = c->meterMap.copyFrom(s->meterMap, from, thru, cut != 0, s->ppq, found);
    if (found) {
        put<std::uint16_t>(flag, 1);
    }
    return ok ? 1 : 0;
}

uint16_t SeqMeterPaste(const HSEQ clip, const HSEQ dst, const uint16_t measure, const uint16_t length,
                       const uint16_t reps, const uint16_t slide) {
    const Sequence* c = sequence(clip);
    Sequence* d = sequence(dst);
    if (c == nullptr || d == nullptr) {
        return 0;
    }
    d->meterMap.paste(c->meterMap, measure, length, reps, slide != 0, d->ppq);
    return 1;
}

uint16_t SeqSwapMeterMaps(const HSEQ a, const HSEQ b) {
    Sequence* x = sequence(a);
    Sequence* y = sequence(b);
    if (x != nullptr && y != nullptr) {
        std::swap(x->meterMap, y->meterMap);
    }
    return 0;
}

uint32_t SeqMBT2Tick(const HSEQ seq, const uint16_t measure, const uint16_t beat, const uint16_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->meterMap.toTick(measure, beat, tick, s->ppq);
}

uint32_t SeqTick2MBT(const HSEQ seq, const uint32_t tick, uint16_t* measure, uint16_t* beat, uint16_t* ticks) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    const libseq::MBT v = s->meterMap.toMBT(tick, s->ppq);
    putMBT(measure, beat, ticks, v);
    return v.tick;
}

uint32_t SeqTick2Meas(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->meterMap.toMBT(tick, s->ppq).measure;
}

char* SeqTick2MBTStr(const HSEQ seq, const uint32_t tick, char* buffer, const uint16_t flag) {
    const Sequence* s = sequence(seq);
    if (s != nullptr) {
        putString(buffer, libseq::formatMBT(s->meterMap.toMBT(tick, s->ppq), flag != 0));
    }
    return buffer;
}

uint32_t SeqMBTStr2MBT(const HSEQ seq, const char* string, uint16_t* measure, uint16_t* beat, uint16_t* tick) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    putMBT(measure, beat, tick, libseq::parseMBT(str(string)));
    return 1;
}

uint32_t SeqTickNextMeas(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->meterMap.nextMeasure(tick, s->ppq);
}

uint32_t SeqTickPrevMeas(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->meterMap.prevMeasure(tick, s->ppq);
}

uint32_t SeqTickNextBeat(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->meterMap.nextBeat(tick, s->ppq);
}

uint32_t SeqTickPrevBeat(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->meterMap.prevBeat(tick, s->ppq);
}

uint16_t SeqSetPlayingSeq(const HSEQ seq) {
    return engine().setPlayingSeq(seq) ? 1 : 0;
}

uint16_t SeqSetStandbySeq(const HSEQ seq) {
    return engine().setStandbySeq(seq) ? 1 : 0;
}

uint16_t SeqSetSrcClock(const uint16_t source) {
    return engine().setSrcClock(source) ? 1 : 0;
}

uint16_t SeqGetSrcClock(void) {
    return engine().srcClock();
}

uint16_t SeqPlayStart(const SEQSTARTPROC proc) {
    return engine().playStart(startProc(proc)) ? 1 : 0;
}

uint16_t SeqPlayStop(void) {
    return engine().playStop() ? 1 : 0;
}

uint16_t SeqRecordStart(const HSTREAM target, const uint32_t in, const uint32_t out, const SEQSTARTPROC proc) {
    return engine().recordStart(target, in, out, startProc(proc)) ? 1 : 0;
}

uint16_t SeqRecordStop(void) {
    return engine().recordStop() ? 1 : 0;
}

uint16_t SeqRecordCleanup(const HSEQ seq, const uint16_t keep, uint16_t /*reserved*/, HSTREAM* stream) {
    HSTREAM s = stream != nullptr ? *stream : 0;
    const bool ok = engine().recordCleanup(seq, keep != 0, s);
    put(stream, s);
    return ok ? 1 : 0;
}

uint16_t SeqExpired(void) {
    return engine().expired() ? 1 : 0;
}

uint32_t SeqRewind(const HSEQ seq) {
    return engine().setNow(seq, 0);
}

uint32_t SeqSetNow(const HSEQ seq, const uint32_t tick) {
    return engine().setNow(seq, tick);
}

uint32_t SeqGetNow(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->now;
}

uint32_t SeqGetEnd(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->end();
}

uint32_t SeqSetDur(const HSEQ seq, const uint32_t ticks) {
    Sequence* s = sequence(seq);
    if (s != nullptr) {
        s->dur = ticks;
    }
    return ticks;
}

uint32_t SeqGetDur(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->dur;
}

HSTREAM SeqStepRecSetup(const HSEQ seq, void* hwnd) {
    return engine().stepRecSetup(
        seq, [hwnd](const std::uint32_t count) { platform().postMessage(hwnd, 0x0464, 0x0100, count); });
}

void SeqStepRecParams(const uint32_t position, const uint16_t duration, const uint16_t velocity) {
    engine().stepRecParams(position, duration, velocity);
}

uint16_t SeqRecordEvent(const SEQEVENT* event) {
    return event != nullptr && engine().recordEvent(fromC(*event)) ? 1 : 0;
}

uint32_t SeqRecCount(void) {
    return engine().recCount();
}

uint16_t SeqSetRecCount(const uint32_t count) {
    return static_cast<std::uint16_t>(engine().setRecCount(count));
}

uint16_t SeqStepRecTerm(void) {
    return static_cast<std::uint16_t>(engine().stepRecTerm());
}

uint16_t SeqTxMidiMsg(const uint16_t port, const uint16_t channel, const uint16_t status, const uint16_t data1,
                      const uint16_t data2) {
    return engine().txMidiMsg(port, channel, status, data1, data2) ? 1 : 0;
}

HSYSXBANK SeqCreateSysxBank(const HSEQ seq) {
    Sequence* s = sequence(seq);
    return s == nullptr ? 0 : s->createSysxBank();
}

uint16_t SeqDestroySysxBank(const HSYSXBANK bank) {
    Sequence* s = sequence(libseq::seqOf(bank));
    return s != nullptr && s->sysxBanks.erase(libseq::idOf(bank)) == 1 ? 1 : 0;
}

uint16_t SeqEmptySysxBank(const HSYSXBANK bank) {
    libseq::SysxBank* b = findBank(bank);
    if (b == nullptr) {
        return 0;
    }
    b->data.clear();
    return 1;
}

uint32_t SeqEnumSysxBanks(const HSEQ seq, const SEQENUMPROC proc, const uint32_t value) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    if (proc != nullptr) {
        for (const auto& entry : s->sysxBanks) {
            proc(s->handle(), libseq::makeStreamHandle(s->handle(), entry.first), value);
        }
        proc(0, 0, value);
    }
    return 1;
}

uint16_t SeqSetSysxBankParam(const HSYSXBANK bank, const uint16_t param, const int32_t value) {
    libseq::SysxBank* b = findBank(bank);
    return b != nullptr && b->setParam(param, value) ? 1 : 0;
}

int32_t SeqGetSysxBankParam(const HSYSXBANK bank, const uint16_t param) {
    const libseq::SysxBank* b = findBank(bank);
    return b == nullptr ? 0 : b->param(param);
}

uint16_t SeqSetSysxBankName(const HSYSXBANK bank, const char* name) {
    libseq::SysxBank* b = findBank(bank);
    if (b == nullptr) {
        return 0;
    }
    b->name = str(name);
    return 1;
}

uint16_t SeqGetSysxBankName(const HSYSXBANK bank, char* buffer, const uint16_t size) {
    const libseq::SysxBank* b = findBank(bank);
    if (b == nullptr || size == 0) {
        return 0;
    }
    // as much of the name as fits; 1 when all of a name did
    putString(buffer, b->name.substr(0, std::min<std::size_t>(b->name.size(), size - 1U)));
    return !b->name.empty() && b->name.size() < size ? 1 : 0;
}

uint32_t SeqGetSysxBankLen(const HSYSXBANK bank) {
    const libseq::SysxBank* b = findBank(bank);
    return b == nullptr ? 0 : static_cast<std::uint32_t>(b->data.size());
}

uint32_t SeqGetSysxAscLen(const HSYSXBANK bank, const uint16_t bytesPerLine) {
    const libseq::SysxBank* b = findBank(bank);
    // with 0 bytes per line TTSSEQ divides by zero
    if (b == nullptr || bytesPerLine == 0) {
        return 0;
    }
    return static_cast<std::uint32_t>(libseq::sysxToText(b->data, bytesPerLine).size() + 1);
}

uint32_t SeqSysxBin2Asc(const HSYSXBANK bank, char* buffer, const uint16_t bytesPerLine) {
    const libseq::SysxBank* b = findBank(bank);
    // an empty bank leaves the buffer as it is
    if (b == nullptr || bytesPerLine == 0 || b->data.empty()) {
        return 0;
    }
    putString(buffer, libseq::sysxToText(b->data, bytesPerLine));
    return static_cast<std::uint32_t>(b->data.size());
}

uint16_t SeqSysxAsc2Bin(const HSYSXBANK bank, const char* text, uint32_t* position) {
    libseq::SysxBank* b = findBank(bank);
    if (b == nullptr) {
        return 0;
    }
    libseq::SysxParse r = libseq::sysxFromText(str(text));
    if (r.result == libseq::SysxParse::Ok) {
        b->data = std::move(r.data);
    } else if (r.result == libseq::SysxParse::NotHex) {
        put(position, static_cast<std::uint32_t>(r.position));
    }
    return static_cast<std::uint16_t>(r.result);
}

uint16_t SeqSaveSysxBank(const HSYSXBANK bank, const char* file) {
    const libseq::SysxBank* b = findBank(bank);
    if (b == nullptr || file == nullptr) {
        return 0;
    }
    std::ofstream f(file, std::ios::binary);
    const std::string bytes(b->data.begin(), b->data.end());
    f << bytes;
    return f ? 1 : 0;
}

uint16_t SeqLoadSysxBank(const HSYSXBANK bank, const char* file) {
    libseq::SysxBank* b = findBank(bank);
    if (b == nullptr || file == nullptr) {
        return 0;
    }
    std::ifstream f(file, std::ios::binary);
    if (!f) {
        return 0;
    }
    b->data.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    return 1;
}

uint16_t SeqSendSysxBank(const HSYSXBANK bank, uint16_t /*reserved1*/, uint16_t /*reserved2*/) {
    return engine().sendSysxBank(bank) ? 1 : 0;
}

int16_t SeqReceiveSysxBank(const HSYSXBANK bank, const SEQSYSXRECEIVEPROC proc, uint16_t /*reserved1*/,
                           uint16_t /*reserved2*/, uint16_t /*reserved3*/) {
    return engine().receiveSysxBank(bank,
                                    [proc](const std::uint32_t received, const std::uint32_t free) -> std::uint32_t {
                                        return proc != nullptr ? proc(received, free) : 0U;
                                    });
}

HSEQ SeqLoadCakewalkFile(const char* file, uint16_t /*reserved*/, const SEQFILEPROC proc, const uint32_t value) {
    return engine().loadCakewalkFile(str(file), fileProc(proc, value));
}

uint16_t SeqSaveCakewalkFile(const HSEQ seq, const char* file, uint32_t /*reserved*/, const SEQFILEPROC proc,
                             const uint32_t value) {
    return engine().saveCakewalkFile(seq, str(file), fileProc(proc, value)) ? 1 : 0;
}

HSEQ SeqLoadMidiFile(const char* file, uint16_t /*reserved*/) {
    return engine().loadMidiFile(str(file));
}

uint16_t SeqSaveMidiFile(const HSEQ seq, const char* file, uint32_t /*reserved*/) {
    return engine().saveMidiFile(seq, str(file)) ? 1 : 0;
}

uint32_t SeqIOSeek(uint16_t /*file*/, const int32_t offset, const uint16_t origin) {
    return engine().ioSeek(offset, origin);
}

uint16_t SeqIOWrite(uint16_t /*file*/, const void* buffer, const uint16_t count) {
    return buffer == nullptr
               ? 0
               : static_cast<std::uint16_t>(engine().ioWrite(static_cast<const std::uint8_t*>(buffer), count));
}

uint16_t SeqIORead(uint16_t /*file*/, void* buffer, const uint16_t count) {
    return buffer == nullptr ? 0
                             : static_cast<std::uint16_t>(engine().ioRead(static_cast<std::uint8_t*>(buffer), count));
}

uint16_t SeqGetFileError(void) {
    return static_cast<std::uint16_t>(engine().fileError());
}

uint32_t SeqGetPortName(const uint16_t port, char* buffer) {
    putString(buffer, engine().portName(port));
    return 1;
}

uint32_t SeqNumMarkers(const HSEQ seq) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->markers.size());
}

uint32_t SeqMarkerForTime(const HSEQ seq, const uint32_t tick) {
    const Sequence* s = sequence(seq);
    return s == nullptr ? 0 : static_cast<std::uint32_t>(s->markerForTime(tick));
}

uint16_t SeqSetMarkerName(const HSEQ seq, const uint32_t index, const char* name) {
    Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->markers.size()) {
        return 0;
    }
    s->markers[index].name = str(name);
    return 1;
}

uint16_t SeqGetMarkerName(const HSEQ seq, const uint32_t index, char* buffer, const uint16_t size) {
    const Sequence* s = sequence(seq);
    if (s == nullptr || size == 0) {
        return 0;
    }
    const std::string name = index < s->markers.size() ? s->markers[index].name : std::string();
    const std::string cut = name.substr(0, std::min<std::size_t>(name.size(), size - 1U));
    putString(buffer, cut);
    return static_cast<std::uint16_t>(cut.size());
}

uint16_t SeqSetMarkerTime(const HSEQ seq, const uint32_t index, const uint32_t time, const uint16_t smpteLock) {
    Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->markers.size()) {
        return 0;
    }
    s->setMarkerTime(s->markers[index], time, smpteLock != 0);
    return 1;
}

uint16_t SeqGetMarkerTime(const HSEQ seq, const uint32_t index, uint32_t* time, uint16_t* smpteLock) {
    const Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->markers.size()) {
        return 0;
    }
    put(time, s->markerTime(s->markers[index]));
    put<std::uint16_t>(smpteLock, s->markers[index].smpteLocked ? 1 : 0);
    return 1;
}

uint16_t SeqInsMarker(const HSEQ seq, const char* name, const uint32_t time, const uint16_t smpteLock) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    libseq::Marker m;
    m.name = str(name);
    s->setMarkerTime(m, time, smpteLock != 0);
    s->insertMarker(std::move(m));
    return 1;
}

uint16_t SeqDelMarker(const HSEQ seq, const uint32_t index) {
    Sequence* s = sequence(seq);
    if (s == nullptr || index >= s->markers.size()) {
        return 0;
    }
    s->markers.erase(s->markers.begin() + static_cast<std::ptrdiff_t>(index));
    return 1;
}

uint16_t SeqSortMarkers(const HSEQ seq, uint32_t /*reserved*/) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    s->sortMarkers();
    return static_cast<std::uint16_t>(s->markers.size());
}

uint32_t SeqHugeMemCpy(void* dst, const void* src, const uint32_t count) {
    if (dst != nullptr && src != nullptr && count > 0) {
        std::memmove(dst, src, count);
    }
    return count;
}

// ---- libseq

void LibSeqSetPlatform(const LIBSEQPLATFORM* p) {
    platform().install(p);
    if (const std::unique_ptr<Instance>& s = slot()) {
        s->engine.setPlatform(platform().installed() ? &platform() : nullptr);
    }
}

void LibSeqTimer(void) {
    engine().timer();
}

void LibSeqReceive(const uint8_t* message, const size_t length) {
    if (message != nullptr && length > 0) {
        engine().receive({message, length});
    }
}

void LibSeqUnload(void) {
    slot().reset();
}

// NOLINTEND(readability-identifier-naming)

} // extern "C"
