// The TTSSEQ 1.01 interface over libseq, for the script runner: the event cursor and the edit
// primitives (docs/reverse-engineering/signatures.md, "Streams and events").
#include "api.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <cstring>

namespace ttsrun {

using libseq::Engine;
using libseq::Event;
using libseq::Sequence;
using libseq::Stream;

namespace {

/// An event the probe passes by pointer.
bool readEvent(Probe& p, const FarPtr at, Event& e) {
    const std::uint8_t* b = p.mem.at(at, Event::kSize);
    if (b == nullptr) {
        return false;
    }
    e = Event::fromBytes(b);
    return true;
}

bool writeEvent(Probe& p, const FarPtr at, const Event& e) {
    std::uint8_t* out = p.mem.at(at, Event::kSize);
    if (out == nullptr) {
        return false;
    }
    const auto b = e.bytes();
    std::memcpy(out, b.data(), b.size());
    return true;
}

} // namespace

void defineEvents101(Probe& p, Engine& e) {
    // A stream handle the engine does not hold crashes TTSSEQ; the runner answers 0.
    const auto stream = [&e](Args& a) -> Stream* { return e.stream(a.word()); };

    // ---- the cursor
    p.define("SEQSEEK", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        a.word();
        const std::uint32_t index = a.dword();
        if (s == nullptr) {
            return 0;
        }
        s->cursor = std::min<std::size_t>(index, s->events().size());
        return static_cast<std::uint32_t>(s->cursor);
    });
    p.define("SEQTELL", [stream](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = stream(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->cursor);
    });
    p.define("SEQEOS", [stream](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = stream(a);
        return s == nullptr || s->cursor >= s->events().size() ? 1 : 0;
    });
    p.define("SEQNXTEVT", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        if (s == nullptr || s->cursor >= s->events().size()) {
            return 0;
        }
        ++s->cursor;
        return s->cursor < s->events().size() ? 1 : 0;
    });
    const auto getEvt = [stream](Args& a, Probe& pr) -> std::uint32_t {
        const Stream* s = stream(a);
        const FarPtr out = a.ptr();
        if (s == nullptr || s->cursor >= s->events().size()) {
            return 0;
        }
        return writeEvent(pr, out, s->events()[s->cursor]) ? 1 : 0;
    };
    p.define("SEQGETEVT", getEvt);
    p.define("SEQGETEVTCOPY", getEvt);
    p.define("SEQEVTATTIMEGE", [stream](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = stream(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->indexAtOrAfter(a.dword()));
    });
    p.define("SEQEVTATTIMEGT", [stream](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = stream(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->indexAfter(a.dword()));
    });
    p.define("SEQMEASHASEVTS", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::StreamHandle h = a.word();
        const std::uint16_t measure = a.word();
        const Stream* s = e.stream(h);
        const Sequence* q = e.sequence(libseq::seqOf(h));
        if (s == nullptr || q == nullptr) {
            return 0;
        }
        const libseq::Tick from = q->meterMap.toTick(measure, 1, 0, q->ppq);
        const libseq::Tick to = q->meterMap.toTick(static_cast<std::uint16_t>(measure + 1), 1, 0, q->ppq);
        return s->hasEventsIn(from, to) ? 1 : 0;
    });

    // ---- edits at the cursor
    p.define("SEQPUTEVT", [stream](Args& a, Probe& pr) -> std::uint32_t {
        Stream* s = stream(a);
        Event ev;
        return s != nullptr && readEvent(pr, a.ptr(), ev) && s->put(ev) ? 1 : 0;
    });
    p.define("SEQINSEVT", [stream](Args& a, Probe& pr) -> std::uint32_t {
        Stream* s = stream(a);
        Event ev;
        if (s == nullptr || !readEvent(pr, a.ptr(), ev)) {
            return 0;
        }
        s->insert(ev);
        return 1;
    });
    p.define("SEQDELEVT", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        return s != nullptr && s->erase() ? 1 : 0;
    });
    p.define("SEQMOVEEVT", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        const libseq::Tick t = a.dword();
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->move(t));
    });

    // ---- ranges
    p.define("SEQTEMPORALSORT", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        if (s == nullptr) {
            return 0;
        }
        return static_cast<std::uint32_t>(s->sort());
    });
    p.define("SEQDELETERANGE", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        const std::uint32_t first = a.dword();
        const std::uint32_t end = a.dword();
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
    });
    p.define("SEQDISPLACEEARLIER", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        const libseq::Tick from = a.dword();
        const std::uint32_t amount = a.dword();
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->displaceEarlier(from, amount));
    });
    p.define("SEQDISPLACELATER", [stream](Args& a, Probe&) -> std::uint32_t {
        Stream* s = stream(a);
        const libseq::Tick from = a.dword();
        const std::uint32_t amount = a.dword();
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->displaceLater(from, amount));
    });
    p.define("SEQINSERTRANGE", [&e](Args& a, Probe&) -> std::uint32_t {
        Stream* dst = e.stream(a.word());
        const std::uint32_t at = a.dword();
        const Stream* src = e.stream(a.word());
        if (dst == nullptr || src == nullptr) {
            return 0;
        }
        const std::vector<Event> copy = src->events();
        dst->insertRange(at, copy);
        return 1;
    });
    p.define("SEQMERGESTREAMS", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        const libseq::StreamHandle sa = a.word();
        a.word(); // the sequence of b: no visible effect
        const libseq::StreamHandle sb = a.word();
        const bool flag = a.word() != 0;
        const libseq::Tick offset = a.dword();
        return e.mergeStreams(seq, sa, sb, flag, offset);
    });
}

} // namespace ttsrun
