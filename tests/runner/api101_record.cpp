// The TTSSEQ 1.01 recording calls for the script runner: real-time recording with its count-in
// callback, and step recording.
#include "api.hpp"

namespace ttsrun {

void defineRecord101(Probe& p) {
    p.define("SEQRECORDSTART", [](Args& a, Probe&) -> std::uint32_t {
        const HSTREAM target = a.word();
        const std::uint32_t in = a.dword();
        const std::uint32_t out = a.dword();
        const FarPtr fn = a.ptr();
        const Scoped s(glue().startCb, fn);
        return SeqRecordStart(target, in, out, startCallback(fn));
    });
    p.define("SEQRECORDSTOP", [](Args&, Probe&) -> std::uint32_t { return SeqRecordStop(); });
    p.define("SEQRECCOUNT", [](Args&, Probe&) -> std::uint32_t { return SeqRecCount(); });
    p.define("SEQRECORDCLEANUP", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t keep = a.word();
        const std::uint16_t r = a.word(); // always 0 in the programs' calls; no effect found
        Out<HSTREAM> stream(pr, a.ptr());
        return SeqRecordCleanup(seq, keep, r, stream.ptr());
    });
    p.define("SEQSETRECCOUNT", [](Args& a, Probe&) -> std::uint32_t { return SeqSetRecCount(a.dword()); });
    // the window is the probe's; the engine's message reaches it through the platform
    p.define("SEQSTEPRECSETUP", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        a.word();
        return SeqStepRecSetup(seq, nullptr);
    });
    // returns nothing; read as a word (suite misc), AX still holds the velocity
    p.define("SEQSTEPRECPARAMS", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint32_t position = a.dword();
        const std::uint16_t duration = a.word();
        const std::uint16_t velocity = a.word();
        SeqStepRecParams(position, duration, velocity);
        return velocity;
    });
    p.define("SEQSTEPRECTERM", [](Args&, Probe&) -> std::uint32_t { return SeqStepRecTerm(); });
    p.define("SEQRECORDEVENT", [](Args& a, Probe& pr) -> std::uint32_t {
        SEQEVENT ev{};
        return SeqRecordEvent(readEvent(pr, a.ptr(), ev) ? &ev : nullptr);
    });
}

} // namespace ttsrun
