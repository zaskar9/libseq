// The TTSSEQ 1.01 transport for the script runner: the playing sequence, start, stop, SeqIdle and
// Now.
#include "api.hpp"

namespace ttsrun {

namespace {

/// A start that waits: (code, a, b) as three words.
std::uint16_t startProc(const std::uint16_t code, const std::uint16_t a, const std::uint16_t b) {
    const Glue& g = glue();
    return static_cast<std::uint16_t>(g.probe->callback(g.startCb, {code, a, b}));
}

} // namespace

SEQSTARTPROC startCallback(const FarPtr fn) {
    return fn.null() ? nullptr : startProc;
}

void definePlay101(Probe& p) {
    p.define("SEQSETPLAYINGSEQ", [](Args& a, Probe&) -> std::uint32_t { return SeqSetPlayingSeq(a.word()); });
    p.define("SEQSETSTANDBYSEQ", [](Args& a, Probe&) -> std::uint32_t { return SeqSetStandbySeq(a.word()); });
    p.define("SEQPLAYSTART", [](Args& a, Probe&) -> std::uint32_t {
        const FarPtr fn = a.ptr();
        const Scoped s(glue().startCb, fn);
        return SeqPlayStart(startCallback(fn));
    });
    p.define("SEQSETSRCCLOCK", [](Args& a, Probe&) -> std::uint32_t { return SeqSetSrcClock(a.word()); });
    p.define("SEQGETSRCCLOCK", [](Args&, Probe&) -> std::uint32_t { return SeqGetSrcClock(); });
    p.define("SEQPLAYSTOP", [](Args&, Probe&) -> std::uint32_t { return SeqPlayStop(); });
    p.define("SEQTXMIDIMSG", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint16_t port = a.word();
        const std::uint16_t channel = a.word();
        const std::uint16_t status = a.word();
        const std::uint16_t d1 = a.word();
        return SeqTxMidiMsg(port, channel, status, d1, a.word());
    });
    p.define("SEQIDLE", [](Args&, Probe&) -> std::uint32_t { return SeqIdle(); });
    p.define("SEQEXPIRED", [](Args&, Probe&) -> std::uint32_t { return SeqExpired(); });
    p.define("SEQGETNOW", [](Args& a, Probe&) -> std::uint32_t { return SeqGetNow(a.word()); });
    p.define("SEQSETNOW", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSetNow(seq, a.dword());
    });
    p.define("SEQREWIND", [](Args& a, Probe&) -> std::uint32_t { return SeqRewind(a.word()); });
    p.define("SEQGETEND", [](Args& a, Probe&) -> std::uint32_t { return SeqGetEnd(a.word()); });
    p.define("SEQGETDUR", [](Args& a, Probe&) -> std::uint32_t { return SeqGetDur(a.word()); });
    p.define("SEQSETDUR", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSetDur(seq, a.dword());
    });
}

} // namespace ttsrun
