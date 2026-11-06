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
    p.define("SEQPLAYSTART", [&e](Args& a, Probe&) -> std::uint32_t {
        a.ptr(); // the start callback: for the count-in and sync, not called yet
        return e.playStart() ? 1 : 0;
    });
    p.define("SEQPLAYSTOP", [&e](Args&, Probe&) -> std::uint32_t { return e.playStop() ? 1 : 0; });
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
