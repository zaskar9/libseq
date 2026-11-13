// The TTSSEQ 2.10 interface for the script runner: each exported function the oracle scripts call,
// with the arguments as the probe pushes them (see docs/reverse-engineering/signatures.md), handed
// to libseq's C interface. Here: lifecycle, devices, sequences and streams.
#include "api.hpp"

namespace ttsrun {

Glue& glue() {
    static Glue g;
    return g;
}

namespace {

/// What AX holds where 2.10 returns nothing (suite openclose): the same word in every run.
constexpr std::uint16_t kLeftover = 0x0367;

/// SeqEnumStreams and SeqEnumSysxBanks: (seq, handle, value) as four words.
std::uint16_t enumProc(const HSEQ seq, const std::uint16_t handle, const std::uint32_t value) {
    const Glue& g = glue();
    return static_cast<std::uint16_t>(g.probe->callback(g.enumCb, {seq, handle, hi(value), lo(value)}));
}

} // namespace

SEQENUMPROC enumCallback(const FarPtr fn) {
    return fn.null() ? nullptr : enumProc;
}

void defineApi(Probe& p) {
    glue() = Glue{};
    glue().probe = &p;

    // ---- lifecycle
    p.define("SEQGETVERSION", [](Args&, Probe&) -> std::uint32_t { return SeqGetVersion(); });
    p.define("SEQWIRE", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint16_t flag = a.word();
        SeqWire(flag);
        // returns nothing; read as a word (the suites), AX holds 0 after an odd flag
        return (flag & 1U) != 0 ? 0 : kLeftover;
    });
    p.define("SEQOPEN", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint16_t r1 = a.word();
        const std::uint16_t r = SeqOpen(r1, a.word());
        if (r == 0) {
            ++glue().openCount;
        }
        return r;
    });
    p.define("SEQCLOSE", [](Args&, Probe&) -> std::uint32_t {
        Glue& g = glue();
        const std::uint16_t r = SeqClose();
        // closing the last time while asleep, the DLL closes nothing and leaves AX as it was
        return --g.openCount == 0 && g.asleep ? kLeftover : r;
    });
    p.define("SEQSLEEP", [](Args&, Probe&) -> std::uint32_t {
        const std::uint16_t r = SeqSleep();
        if (r == 0) {
            glue().asleep = true;
        }
        return r;
    });
    p.define("SEQWAKE", [](Args&, Probe&) -> std::uint32_t {
        const std::uint16_t r = SeqWake();
        if (r == 0) {
            glue().asleep = false;
        }
        return r;
    });

    // ---- devices and memory
    p.define("SEQGETPORTNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t port = a.word();
        Text name(pr, a.ptr());
        return SeqGetPortName(port, name.ptr());
    });
    p.define("SEQHUGEMEMCPY", [](Args& a, Probe& pr) -> std::uint32_t {
        const FarPtr dst = a.ptr();
        const FarPtr src = a.ptr();
        const std::uint32_t n = a.dword();
        return SeqHugeMemCpy(pr.mem.at(dst, n), pr.mem.at(src, n), n);
    });

    // ---- sequences
    p.define("SEQCREATESEQ", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint16_t r1 = a.word();
        return SeqCreateSeq(r1, a.word());
    });
    p.define("SEQDESTROYSEQ", [](Args& a, Probe&) -> std::uint32_t { return SeqDestroySeq(a.word()); });
    p.define("SEQGETSEQPARAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return dx(SeqGetSeqParam(seq, a.word()));
    });
    p.define("SEQSETSEQPARAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t param = a.word();
        return SeqSetSeqParam(seq, param, static_cast<std::int32_t>(a.dword()));
    });
    p.define("SEQGETFILEERROR", [](Args&, Probe&) -> std::uint32_t { return SeqGetFileError(); });

    // ---- streams
    p.define("SEQSTREAMLEN", [](Args& a, Probe&) -> std::uint32_t { return SeqStreamLen(a.word()); });
    p.define("SEQSTREAMDUR", [](Args& a, Probe&) -> std::uint32_t { return SeqStreamDur(a.word()); });
    p.define("SEQGETSTREAMPARAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        return dx(SeqGetStreamParam(h, a.word()));
    });
    p.define("SEQSETSTREAMPARAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM h = a.word();
        const std::uint16_t param = a.word();
        return SeqSetStreamParam(h, param, static_cast<std::int32_t>(a.dword()));
    });
    p.define("SEQGETSTREAMALLPARAMS", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSTREAM h = a.word();
        return SeqGetStreamAllParams(h, pr.mem.at(a.ptr(), 22));
    });
    p.define("SEQGETSTREAMNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSTREAM h = a.word();
        Text name(pr, a.ptr());
        return SeqGetStreamName(h, name.ptr(), a.word());
    });
    p.define("SEQSETSTREAMNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSTREAM h = a.word();
        return SeqSetStreamName(h, pr.mem.string(a.ptr()).c_str());
    });
    p.define("SEQCREATESTREAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t r1 = a.word();
        return SeqCreateStream(seq, r1, a.word());
    });
    p.define("SEQDESTROYSTREAM", [](Args& a, Probe&) -> std::uint32_t { return SeqDestroyStream(a.word()); });
    p.define("SEQADOPTSTREAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqAdoptStream(seq, a.word());
    });
    p.define("SEQCLONESTREAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const HSTREAM h = a.word();
        return SeqCloneStream(seq, h, a.word());
    });
    p.define("SEQENUMSTREAMS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const FarPtr cb = a.ptr();
        const Scoped s(glue().enumCb, cb);
        return SeqEnumStreams(seq, enumCallback(cb), a.dword());
    });

    defineEvents(p);
    defineMarks(p);
    defineTime(p);
    defineFiles(p);
    definePlay(p);
    defineRecord(p);
}

} // namespace ttsrun
