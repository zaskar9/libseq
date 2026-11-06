// The TTSSEQ 1.01 transport over libseq, for the script runner: the playing sequence, start, stop,
// SeqIdle and Now.
#include "api.hpp"

#include "libseq/engine.hpp"

namespace ttsrun {

using libseq::Engine;
using libseq::Sequence;

void definePlay101(Probe& p, Engine& e) {
    p.define("SEQSETPLAYINGSEQ", [&e](Args& a, Probe&) -> std::uint32_t { return e.setPlayingSeq(a.word()) ? 1 : 0; });
    p.define("SEQSETSTANDBYSEQ", [&e](Args& a, Probe&) -> std::uint32_t { return e.setStandbySeq(a.word()) ? 1 : 0; });
    p.define("SEQPLAYSTART",
             [&e](Args& a, Probe& pr) -> std::uint32_t { return e.playStart(startProc(pr, a.ptr())) ? 1 : 0; });
    p.define("SEQSETSRCCLOCK", [&e](Args& a, Probe&) -> std::uint32_t { return e.setSrcClock(a.word()) ? 1 : 0; });
    p.define("SEQGETSRCCLOCK", [&e](Args&, Probe&) -> std::uint32_t { return e.srcClock(); });
    p.define("SEQPLAYSTOP", [&e](Args&, Probe&) -> std::uint32_t { return e.playStop() ? 1 : 0; });
    p.define("SEQTXMIDIMSG", [&e](Args& a, Probe&) -> std::uint32_t {
        const std::uint16_t port = a.word();
        const std::uint16_t channel = a.word();
        const std::uint16_t status = a.word();
        const std::uint16_t d1 = a.word();
        return e.txMidiMsg(port, channel, status, d1, a.word()) ? 1 : 0;
    });
    p.define("SEQIDLE", [&e](Args&, Probe&) -> std::uint32_t { return e.idle(); });
    p.define("SEQEXPIRED", [&e](Args&, Probe&) -> std::uint32_t { return e.expired() ? 1 : 0; });
    p.define("SEQGETNOW", [&e](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = e.sequence(a.word());
        return s == nullptr ? 0 : s->now;
    });
    p.define("SEQSETNOW", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        return e.setNow(seq, a.dword());
    });
    p.define("SEQREWIND", [&e](Args& a, Probe&) -> std::uint32_t { return e.setNow(a.word(), 0); });
    p.define("SEQGETEND", [&e](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = e.sequence(a.word());
        return s == nullptr ? 0 : s->end();
    });
    p.define("SEQGETDUR", [&e](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = e.sequence(a.word());
        return s == nullptr ? 0 : s->dur;
    });
    p.define("SEQSETDUR", [&e](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = e.sequence(a.word());
        const libseq::Tick d = a.dword();
        if (s != nullptr) {
            s->dur = d;
        }
        return d;
    });
}

} // namespace ttsrun
