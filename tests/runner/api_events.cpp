// The TTSSEQ 2.10 interface for the script runner: the event cursor and the edit primitives
// (docs/reverse-engineering/signatures.md, "Streams and events").
#include "api.hpp"

namespace ttsrun {

void defineEvents(Probe& p) {
    // ---- the cursor
    p.define("SEQSEEK", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        const std::uint16_t r = a.word();
        return SeqSeek(h, r, a.dword());
    });
    p.define("SEQTELL", [](Args& a, Probe&) -> std::uint32_t { return SeqTell(a.word()); });
    p.define("SEQEOS", [](Args& a, Probe&) -> std::uint32_t { return SeqEos(a.word()); });
    p.define("SEQNXTEVT", [](Args& a, Probe&) -> std::uint32_t { return SeqNxtEvt(a.word()); });
    const auto getEvt = [](std::uint16_t (*get)(HSTREAM, SEQEVENT*)) {
        return [get](Args& a, Probe& pr) -> std::uint32_t {
            const HSTREAM h = a.word();
            const FarPtr out = a.ptr();
            SEQEVENT ev{};
            const std::uint16_t r = get(h, pr.mem.at(out, sizeof ev) != nullptr ? &ev : nullptr);
            if (r != 0) {
                writeEvent(pr, out, ev);
            }
            return r;
        };
    };
    p.define("SEQGETEVT", getEvt(SeqGetEvt));
    p.define("SEQGETEVTCOPY", getEvt(SeqGetEvtCopy));
    p.define("SEQEVTATTIMEGE", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        return SeqEvtAtTimeGE(h, a.dword());
    });
    p.define("SEQEVTATTIMEGT", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        return SeqEvtAtTimeGT(h, a.dword());
    });
    p.define("SEQMEASHASEVTS", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        return SeqMeasHasEvts(h, a.word());
    });

    // ---- edits at the cursor
    const auto putEvt = [](std::uint16_t (*put)(HSTREAM, const SEQEVENT*)) {
        return [put](Args& a, Probe& pr) -> std::uint32_t {
            const HSTREAM h = a.word();
            SEQEVENT ev{};
            return put(h, readEvent(pr, a.ptr(), ev) ? &ev : nullptr);
        };
    };
    p.define("SEQPUTEVT", putEvt(SeqPutEvt));
    p.define("SEQINSEVT", putEvt(SeqInsEvt));
    p.define("SEQDELEVT", [](Args& a, Probe&) -> std::uint32_t { return SeqDelEvt(a.word()); });
    p.define("SEQMOVEEVT", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        return SeqMoveEvt(h, a.dword());
    });

    // ---- ranges
    p.define("SEQTEMPORALSORT", [](Args& a, Probe&) -> std::uint32_t { return SeqTemporalSort(a.word()); });
    p.define("SEQDELETERANGE", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        const std::uint32_t first = a.dword();
        return SeqDeleteRange(h, first, a.dword());
    });
    p.define("SEQDISPLACEEARLIER", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        const std::uint32_t from = a.dword();
        return SeqDisplaceEarlier(h, from, a.dword());
    });
    p.define("SEQDISPLACELATER", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        const std::uint32_t from = a.dword();
        return SeqDisplaceLater(h, from, a.dword());
    });
    p.define("SEQINSERTRANGE", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM dst = a.word();
        const std::uint32_t at = a.dword();
        return SeqInsertRange(dst, at, a.word());
    });
    p.define("SEQMERGESTREAMS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const HSTREAM sa = a.word();
        const HSEQ seqB = a.word();
        const HSTREAM sb = a.word();
        const std::uint16_t destroyB = a.word();
        return SeqMergeStreams(seq, sa, seqB, sb, destroyB, a.dword());
    });
}

} // namespace ttsrun
