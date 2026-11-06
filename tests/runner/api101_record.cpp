// The TTSSEQ 1.01 recording calls over libseq, for the script runner: real-time recording with
// its count-in callback.
#include "api.hpp"

#include "libseq/engine.hpp"

namespace ttsrun {

using libseq::Engine;

libseq::StartProc startProc(Probe& p, const FarPtr fn) {
    if (fn.null()) {
        return {};
    }
    return [&p, fn](const std::uint16_t code, const std::uint16_t a, const std::uint16_t b) {
        return p.callback(fn, {code, a, b});
    };
}

void defineRecord101(Probe& p, Engine& e) {
    p.define("SEQRECORDSTART", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::StreamHandle target = a.word();
        const libseq::Tick in = a.dword();
        const libseq::Tick out = a.dword();
        return e.recordStart(target, in, out, startProc(pr, a.ptr())) ? 1 : 0;
    });
    p.define("SEQRECORDSTOP", [&e](Args&, Probe&) -> std::uint32_t { return e.recordStop() ? 1 : 0; });
    p.define("SEQRECCOUNT", [&e](Args&, Probe&) -> std::uint32_t { return e.recCount(); });
    p.define("SEQRECORDCLEANUP", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        const bool keep = a.word() != 0;
        a.word(); // always 0 in the programs' calls; no effect found
        std::uint8_t* at = pr.mem.at(a.ptr(), 2);
        libseq::StreamHandle h = at != nullptr ? static_cast<libseq::StreamHandle>(at[0] | (at[1] << 8)) : 0;
        const bool ok = e.recordCleanup(seq, keep, h);
        if (at != nullptr) {
            at[0] = static_cast<std::uint8_t>(h & 0xffU);
            at[1] = static_cast<std::uint8_t>(h >> 8);
        }
        return ok ? 1 : 0;
    });
    p.define("SEQSETRECCOUNT", [&e](Args& a, Probe&) -> std::uint32_t { return e.setRecCount(a.dword()); });
    p.define("SEQSTEPRECSETUP", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        a.word(); // the window: the probe's
        return e.stepRecSetup(seq, [&pr](const std::uint32_t count) {
            pr.post(
                {0x0464, 0x0100, static_cast<std::uint16_t>(count >> 16), static_cast<std::uint16_t>(count & 0xffffU)});
        });
    });
    p.define("SEQSTEPRECPARAMS", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::Tick position = a.dword();
        const std::uint16_t duration = a.word();
        e.stepRecParams(position, duration, a.word());
        return 0;
    });
    p.define("SEQSTEPRECTERM", [&e](Args&, Probe&) -> std::uint32_t { return e.stepRecTerm(); });
    p.define("SEQRECORDEVENT", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint8_t* b = pr.mem.at(a.ptr(), libseq::Event::kSize);
        return b != nullptr && e.recordEvent(libseq::Event::fromBytes(b)) ? 1 : 0;
    });
}

} // namespace ttsrun
