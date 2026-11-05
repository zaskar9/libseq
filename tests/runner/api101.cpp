// The TTSSEQ 1.01 interface over libseq, for the script runner: each exported function the
// oracle scripts call, with the arguments as the probe pushes them (see
// docs/reverse-engineering/signatures.md) and the result the DLL returns.
#include "api.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <cstring>

namespace ttsrun {

using libseq::Engine;
using libseq::Sequence;
using libseq::Stream;

void defineApi101(Probe& p, Engine& e) {
    const auto w = [](const std::uint32_t v) { return v & 0xffffU; };

    // ---- lifecycle
    p.define("SEQGETVERSION", [](Args&, Probe&) -> std::uint32_t { return Engine::kVersion; });
    p.define("SEQOPEN", [](Args&, Probe&) -> std::uint32_t { return 1; });
    p.define("SEQCLOSE", [](Args&, Probe&) -> std::uint32_t { return 1; });

    // ---- sequences
    p.define("SEQCREATESEQ", [&e](Args&, Probe&) -> std::uint32_t { return e.createSequence(); });
    p.define("SEQDESTROYSEQ", [&e](Args& a, Probe&) -> std::uint32_t {
        e.destroySequence(a.word());
        return 1; // also for a handle that holds no sequence
    });
    p.define("SEQGETSEQPARAM", [&e](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = e.sequence(a.word());
        const int param = a.word();
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->param(param));
    });
    p.define("SEQSETSEQPARAM", [&e](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = e.sequence(a.word());
        const int param = a.word();
        const auto v = static_cast<std::int32_t>(a.dword());
        return s != nullptr && s->setParam(param, v) ? 1 : 0;
    });
    p.define("SEQLOADCAKEWALKFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        return e.loadCakewalkFile(path);
    });
    p.define("SEQSAVECAKEWALKFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        return e.saveCakewalkFile(seq, path) ? 1 : 0;
    });
    p.define("SEQLOADMIDIFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        a.word(); // always 0 in the corpus
        return e.loadMidiFile(path);
    });
    p.define("SEQSAVEMIDIFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        a.dword(); // the program callback, not called
        return e.saveMidiFile(seq, path) ? 1 : 0;
    });
    p.define("SEQGETFILEERROR",
             [&e](Args&, Probe&) -> std::uint32_t { return static_cast<std::uint32_t>(e.fileError()); });

    // ---- streams
    p.define("SEQSTREAMLEN", [&e](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = e.stream(a.word());
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->events().size());
    });
    p.define("SEQSTREAMDUR", [&e](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = e.stream(a.word());
        return s == nullptr ? 0 : s->duration();
    });
    p.define("SEQGETSTREAMPARAM", [&e](Args& a, Probe&) -> std::uint32_t {
        const Stream* s = e.stream(a.word());
        const int param = a.word();
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->param(param));
    });
    p.define("SEQSETSTREAMPARAM", [&e](Args& a, Probe&) -> std::uint32_t {
        Stream* s = e.stream(a.word());
        const int param = a.word();
        const auto v = static_cast<std::int32_t>(a.dword());
        return s != nullptr && s->setParam(param, v) ? 1 : 0;
    });
    p.define("SEQGETSTREAMALLPARAMS", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const Stream* s = e.stream(a.word());
        std::uint8_t* out = pr.mem.at(a.ptr(), 22);
        if (s == nullptr || out == nullptr) {
            return 0;
        }
        s->allParams(out);
        return 22;
    });
    p.define("SEQGETSTREAMNAME", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const Stream* s = e.stream(a.word());
        const FarPtr buf = a.ptr();
        const std::size_t size = a.word();
        if (s == nullptr || size == 0) {
            return 0;
        }
        const std::size_t n = std::min(s->name().size(), size - 1);
        std::uint8_t* out = pr.mem.at(buf, n + 1);
        if (out == nullptr) {
            return 0;
        }
        std::memcpy(out, s->name().data(), n);
        out[n] = 0;
        return 1;
    });
    p.define("SEQSETSTREAMNAME", [&e](Args& a, Probe& pr) -> std::uint32_t {
        Stream* s = e.stream(a.word());
        if (s == nullptr) {
            return 0;
        }
        s->setName(pr.mem.string(a.ptr()));
        return 1;
    });
    p.define("SEQCREATESTREAM", [&e](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = e.sequence(a.word());
        return s == nullptr ? 0 : s->createStream().handle();
    });
    p.define("SEQDESTROYSTREAM", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::StreamHandle h = a.word();
        Sequence* s = e.sequence(libseq::seqOf(h));
        return s != nullptr && s->destroyStream(h) ? 1 : 0;
    });
    p.define("SEQADOPTSTREAM", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        return e.adoptStream(seq, a.word());
    });
    p.define("SEQCLONESTREAM", [&e](Args& a, Probe&) -> std::uint32_t {
        Sequence* dst = e.sequence(a.word());
        const Stream* src = e.stream(a.word());
        const bool flag = a.word() != 0;
        return dst == nullptr || src == nullptr ? 0 : dst->clone(*src, flag).handle();
    });
    p.define("SEQENUMSTREAMS", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle h = a.word();
        const FarPtr cb = a.ptr();
        const std::uint32_t value = a.dword();
        const Sequence* s = e.sequence(h);
        if (s == nullptr) {
            return 0;
        }
        const auto hi = static_cast<std::uint16_t>(value >> 16);
        const auto lo = static_cast<std::uint16_t>(value & 0xffffU);
        for (const Stream* st : s->streams()) {
            pr.callback(cb, {h, st->handle(), hi, lo});
        }
        pr.callback(cb, {0, 0, hi, lo});
        return 1;
    });

    (void)w;
    defineEvents101(p, e);
    defineMarks101(p, e);
    defineTime101(p, e);
}

} // namespace ttsrun
