// The TTSSEQ 1.01 interface over libseq, for the script runner: the time base, the tempo and
// meter maps, positions and timecode (docs/reverse-engineering/signatures.md).
#include "api.hpp"

#include "libseq/engine.hpp"

namespace ttsrun {

using libseq::Engine;
using libseq::Sequence;
using libseq::Tick;

namespace {

/// The key as the engine passes it: the signed byte, not sign-extended (0x00fd for 3 flats).
std::uint16_t keyWord(const libseq::MeterEntry& m) {
    return static_cast<std::uint8_t>(m.key);
}

/// The meter of an entry as the engine returns it: the key in DX, the meter in AX.
std::uint32_t meterResult(const libseq::MeterEntry& m) {
    return (std::uint32_t{keyWord(m)} << 16) | m.meter();
}

void putMBT(Probe& p, const FarPtr m, const FarPtr b, const FarPtr t, const libseq::MBT& v) {
    putWord(p, m, v.measure);
    putWord(p, b, v.beat);
    putWord(p, t, v.tick);
}

} // namespace

void defineTime101(Probe& p, Engine& e) {
    // A sequence handle the engine does not hold crashes TTSSEQ; the runner answers 0.
    const auto seq = [&e](Args& a) -> Sequence* { return e.sequence(a.word()); };

    // ---- time base
    p.define("SEQGETMAXPPQ", [](Args&, Probe&) -> std::uint32_t { return libseq::kMaxPpq; });
    p.define("SEQGETMINTEMPO", [](Args&, Probe&) -> std::uint32_t { return libseq::kMinTempo; });
    p.define("SEQGETMAXTEMPO", [](Args&, Probe&) -> std::uint32_t { return libseq::kMaxTempo; });
    p.define("SEQGETPPQ", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->ppq;
    });
    p.define("SEQSETPPQ", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint16_t ppq = a.word();
        return s != nullptr && s->setPpq(ppq) ? 1 : 0;
    });
    p.define("SEQRESCALETIME", [](Args& a, Probe&) -> std::uint32_t {
        const Tick t = a.dword();
        const std::uint16_t from = a.word();
        return libseq::rescaleTime(t, from, a.word());
    });

    // ---- tempo map
    p.define("SEQGETTEMPOMAPLEN", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->tempoMap.size());
    });
    p.define("SEQGETTEMPOATIX", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::size_t i = a.word();
        const FarPtr tick = a.ptr();
        if (s == nullptr || i >= s->tempoMap.size()) {
            return 0xffff;
        }
        putDword(pr, tick, s->tempoMap.entries()[i].tick);
        return s->tempoMap.entries()[i].tempo;
    });
    p.define("SEQTEMPOIXBYTICK", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->tempoMap.indexAt(a.dword()));
    });
    p.define("SEQGETTEMPO", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        const Tick t = a.dword();
        // the tempo is a word; DX keeps the high word of the tick
        return s == nullptr ? 0 : (t & 0xffff0000U) | s->tempoMap.tempoAt(t);
    });
    p.define("SEQSETTEMPO", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const Tick t = a.dword();
        const std::uint16_t tempo = a.word();
        if (s == nullptr) {
            return 0;
        }
        s->tempoMap.set(t, tempo);
        return 1;
    });
    p.define("SEQINSTEMPO", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const Tick t = a.dword();
        const std::uint16_t tempo = a.word();
        if (s == nullptr) {
            return 0;
        }
        s->tempoMap.insert(t, tempo);
        return 1;
    });
    p.define("SEQDELTEMPO", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        if (s == nullptr) {
            return 0;
        }
        s->tempoMap.remove(a.dword());
        return 1;
    });
    p.define("SEQCLEARTEMPOMAP", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        if (s == nullptr) {
            return 0;
        }
        s->tempoMap.clear(a.word());
        return 1;
    });
    p.define("SEQRECALCTEMPOMAP", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->tempoMap.recalc());
    });
    p.define("SEQENUMTEMPOS", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const Tick from = a.dword();
        const Tick thru = a.dword();
        const FarPtr cb = a.ptr();
        const std::uint32_t value = a.dword();
        if (s == nullptr) {
            return 0;
        }
        const auto hi = static_cast<std::uint16_t>(value >> 16);
        const auto lo = static_cast<std::uint16_t>(value & 0xffffU);
        // the callback gets each entry's elapsed time (microseconds times the ppq, 32 bits of it),
        // then (0, 0, 0, 0)
        const std::size_t n = s->tempoMap.enumCount(from, thru);
        for (std::size_t i = 0; i < n; ++i) {
            const auto el = static_cast<std::uint32_t>(s->tempoMap.entries()[i].elapsed);
            pr.callback(cb, {static_cast<std::uint16_t>(el >> 16), static_cast<std::uint16_t>(el & 0xffffU), hi, lo});
        }
        pr.callback(cb, {0, 0, 0, 0});
        return 1;
    });
    p.define("SEQSWAPTEMPOMAPS", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* x = seq(a);
        Sequence* y = seq(a);
        if (x != nullptr && y != nullptr) {
            std::swap(x->tempoMap, y->tempoMap);
        }
        return 0;
    });

    p.define("SEQTEMPOCOPY", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* src = seq(a);
        Sequence* clip = seq(a);
        const Tick from = a.dword();
        const Tick thru = a.dword();
        a.dword(); // no visible effect
        const FarPtr flag = a.ptr();
        if (src == nullptr || clip == nullptr) {
            return 0;
        }
        if (clip->tempoMap.copyFrom(src->tempoMap, from, thru)) {
            putWord(pr, flag, 1);
        }
        return 1;
    });
    p.define("SEQTEMPOPASTE", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* clip = seq(a);
        Sequence* dst = seq(a);
        const Tick at = a.dword();
        const Tick length = a.dword();
        const std::uint16_t reps = a.word();
        const bool slide = a.word() != 0;
        if (clip == nullptr || dst == nullptr) {
            return 0;
        }
        dst->tempoMap.paste(clip->tempoMap, at, length, reps, slide);
        return 1;
    });

    // ---- tempo ratios
    p.define("SEQGETTEMPORATIO", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::size_t i = a.word();
        return s == nullptr || i >= s->tempoRatios.size() ? 0 : s->tempoRatios[i];
    });
    p.define("SEQSETTEMPORATIO", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::size_t i = a.word();
        const std::uint16_t ratio = a.word();
        if (s == nullptr || i >= s->tempoRatios.size()) {
            return 0;
        }
        s->tempoRatios[i] = ratio;
        return 1;
    });
    p.define("SEQSELECTTEMPORATIO", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint16_t i = a.word();
        if (s == nullptr || i >= s->tempoRatios.size()) {
            return 0;
        }
        s->tempoRatio = i;
        return 1;
    });
    p.define("SEQWHICHTEMPORATIO", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->tempoRatio;
    });
    p.define("SEQTEMPORATIO2STR", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t ratio = a.word();
        const FarPtr buf = a.ptr();
        putString(pr, buf, libseq::tempoRatioString(ratio));
        return farResult(buf);
    });

    // ---- meter map
    p.define("SEQGETMETERMAPLEN", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->meterMap.size());
    });
    p.define("SEQGETMETERATIX", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::size_t i = a.word();
        const FarPtr measure = a.ptr();
        if (s == nullptr || i >= s->meterMap.size()) {
            return 0xffffffff;
        }
        const libseq::MeterEntry& m = s->meterMap.entries()[i];
        putWord(pr, measure, static_cast<std::uint16_t>(m.measure + 1));
        return meterResult(m);
    });
    p.define("SEQMETERIXBYTICK", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->meterMap.indexAtTick(a.dword()));
    });
    p.define("SEQGETMETER", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : meterResult(s->meterMap.entries()[s->meterMap.indexAtTick(a.dword())]);
    });
    p.define("SEQINSMETER", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint16_t measure = a.word();
        const auto key = static_cast<std::int8_t>(a.word() & 0xffU);
        const std::uint16_t meter = a.word();
        if (s == nullptr) {
            return 0;
        }
        s->meterMap.insert(measure, key, meter, s->ppq);
        return 1;
    });
    p.define("SEQSETMETER", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const Tick t = a.dword();
        const auto key = static_cast<std::int8_t>(a.word() & 0xffU);
        const std::uint16_t meter = a.word();
        if (s == nullptr) {
            return 0;
        }
        s->meterMap.set(t, key, meter, s->ppq);
        return 1;
    });
    p.define("SEQDELMETER", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        if (s == nullptr) {
            return 0;
        }
        s->meterMap.remove(a.word());
        return 1;
    });
    p.define("SEQCLEARMETERMAP", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const auto key = static_cast<std::int8_t>(a.word() & 0xffU);
        const std::uint16_t meter = a.word();
        if (s == nullptr) {
            return 0;
        }
        s->meterMap.clear(key, meter);
        return 1;
    });
    p.define("SEQRECALCMETERMAP", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->meterMap.recalc(s->ppq));
    });
    p.define("SEQENUMMETERS", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::uint16_t from = a.word();
        const std::uint16_t thru = a.word();
        const FarPtr cb = a.ptr();
        const std::uint32_t value = a.dword();
        if (s == nullptr) {
            return 0;
        }
        const auto hi = static_cast<std::uint16_t>(value >> 16);
        const auto lo = static_cast<std::uint16_t>(value & 0xffffU);
        const std::size_t n = s->meterMap.enumCount(from, thru);
        for (std::size_t i = 0; i < n; ++i) {
            const libseq::MeterEntry& m = s->meterMap.entries()[i];
            pr.callback(cb, {keyWord(m), m.meter(), hi, lo});
        }
        pr.callback(cb, {0, 0, 0, 0});
        return 1;
    });
    p.define("SEQSWAPMETERMAPS", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* x = seq(a);
        Sequence* y = seq(a);
        if (x != nullptr && y != nullptr) {
            std::swap(x->meterMap, y->meterMap);
        }
        return 0;
    });

    p.define("SEQMETERCOPY", [seq](Args& a, Probe& pr) -> std::uint32_t {
        Sequence* src = seq(a);
        Sequence* clip = seq(a);
        const std::uint16_t from = a.word();
        const std::uint16_t thru = a.word();
        const bool cut = a.word() != 0;
        a.word(); // no visible effect
        const FarPtr flag = a.ptr();
        if (src == nullptr || clip == nullptr) {
            return 0;
        }
        bool found = false;
        const bool ok = clip->meterMap.copyFrom(src->meterMap, from, thru, cut, src->ppq, found);
        if (found) {
            putWord(pr, flag, 1);
        }
        return ok ? 1 : 0;
    });
    p.define("SEQMETERPASTE", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* clip = seq(a);
        Sequence* dst = seq(a);
        const std::uint16_t measure = a.word();
        const std::uint16_t length = a.word();
        const std::uint16_t reps = a.word();
        const bool slide = a.word() != 0;
        if (clip == nullptr || dst == nullptr) {
            return 0;
        }
        dst->meterMap.paste(clip->meterMap, measure, length, reps, slide, dst->ppq);
        return 1;
    });

    // ---- positions
    p.define("SEQMBT2TICK", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::uint16_t m = a.word();
        const std::uint16_t b = a.word();
        const std::uint16_t t = a.word();
        return s == nullptr ? 0 : s->meterMap.toTick(m, b, t, s->ppq);
    });
    p.define("SEQTICK2MBT", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const Tick t = a.dword();
        const FarPtr m = a.ptr();
        const FarPtr b = a.ptr();
        const FarPtr k = a.ptr();
        if (s == nullptr) {
            return 0;
        }
        const libseq::MBT v = s->meterMap.toMBT(t, s->ppq);
        putMBT(pr, m, b, k, v);
        return v.tick;
    });
    p.define("SEQTICK2MEAS", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->meterMap.toMBT(a.dword(), s->ppq).measure;
    });
    p.define("SEQTICK2MBTSTR", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const Tick t = a.dword();
        const FarPtr buf = a.ptr();
        const bool flag = a.word() != 0;
        if (s != nullptr) {
            putString(pr, buf, libseq::formatMBT(s->meterMap.toMBT(t, s->ppq), flag));
        }
        return farResult(buf);
    });
    p.define("SEQMBTSTR2MBT", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::string str = pr.mem.string(a.ptr());
        const FarPtr m = a.ptr();
        const FarPtr b = a.ptr();
        const FarPtr t = a.ptr();
        if (s == nullptr) {
            return 0;
        }
        putMBT(pr, m, b, t, libseq::parseMBT(str));
        return 1;
    });
    p.define("SEQTICKNEXTMEAS", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->meterMap.nextMeasure(a.dword(), s->ppq);
    });
    p.define("SEQTICKPREVMEAS", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->meterMap.prevMeasure(a.dword(), s->ppq);
    });
    p.define("SEQTICKNEXTBEAT", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->meterMap.nextBeat(a.dword(), s->ppq);
    });
    p.define("SEQTICKPREVBEAT", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->meterMap.prevBeat(a.dword(), s->ppq);
    });

    // ---- timecode
    p.define("SEQGETFPS", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->fps;
    });
    p.define("SEQSETFPS", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint16_t fps = a.word();
        return s != nullptr && s->setFps(fps) ? 1 : 0;
    });
    p.define("SEQGETFRAMEOFS", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->frameOffset;
    });
    p.define("SEQSETFRAMEOFS", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        if (s == nullptr) {
            return 0;
        }
        s->frameOffset = a.dword();
        return 1;
    });
    p.define("SEQTICK2FRAME", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : s->tickToFrame(a.dword());
    });
    p.define("SEQFRAME2TICKRNDOPT", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::uint32_t f = a.dword();
        const bool up = a.word() != 0;
        return s == nullptr ? 0 : s->frameToTick(f, up);
    });
    p.define("SEQFRAME2HMSF", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::uint32_t f = a.dword();
        const FarPtr h = a.ptr();
        const FarPtr m = a.ptr();
        const FarPtr sec = a.ptr();
        const FarPtr fr = a.ptr();
        if (s == nullptr) {
            return 0;
        }
        const libseq::HMSF t = libseq::framesToHMSF(f, s->fps);
        putWord(pr, h, t.hours);
        putWord(pr, m, t.minutes);
        putWord(pr, sec, t.seconds);
        putWord(pr, fr, t.frames);
        return t.hours;
    });
    p.define("SEQHMSF2FRAME", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        libseq::HMSF t;
        t.hours = a.word();
        t.minutes = a.word();
        t.seconds = a.word();
        t.frames = a.word();
        return s == nullptr ? 0 : libseq::hmsfToFrames(t, s->fps);
    });
    p.define("SEQTICK2HMSFSTR", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const Tick t = a.dword();
        const FarPtr buf = a.ptr();
        if (s != nullptr) {
            putString(pr, buf, libseq::formatHMSF(libseq::framesToHMSF(s->tickToFrame(t), s->fps)));
        }
        return farResult(buf);
    });
    p.define("SEQHMSFSTR2HMSF", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::string str = pr.mem.string(a.ptr());
        const libseq::HMSF t = libseq::parseHMSF(str);
        putWord(pr, a.ptr(), t.hours);
        putWord(pr, a.ptr(), t.minutes);
        putWord(pr, a.ptr(), t.seconds);
        putWord(pr, a.ptr(), t.frames);
        return 1;
    });
    p.define("SEQHMSFSTR2MBT", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::string str = pr.mem.string(a.ptr());
        const FarPtr m = a.ptr();
        const FarPtr b = a.ptr();
        const FarPtr t = a.ptr();
        if (s == nullptr) {
            return 0;
        }
        const Tick tick = s->frameToTick(libseq::hmsfToFrames(libseq::parseHMSF(str), s->fps), false);
        putMBT(pr, m, b, t, s->meterMap.toMBT(tick, s->ppq));
        return 1;
    });
}

} // namespace ttsrun
