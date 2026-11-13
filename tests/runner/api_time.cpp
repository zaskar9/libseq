// The TTSSEQ 2.10 interface for the script runner: the time base, the tempo and meter maps,
// positions and timecode (docs/reverse-engineering/signatures.md).
#include "api.hpp"

namespace ttsrun {

namespace {

/// SeqEnumTempos: (elapsed, value) as four words.
std::uint16_t tempoProc(const std::uint32_t elapsed, const std::uint32_t value) {
    const Glue& g = glue();
    return static_cast<std::uint16_t>(g.probe->callback(g.enumCb, {hi(elapsed), lo(elapsed), hi(value), lo(value)}));
}

/// SeqEnumMeters: (key, meter, value) as four words.
std::uint16_t meterProc(const std::uint16_t key, const std::uint16_t meter, const std::uint32_t value) {
    const Glue& g = glue();
    return static_cast<std::uint16_t>(g.probe->callback(g.enumCb, {key, meter, hi(value), lo(value)}));
}

/// A key as the probe pushes it: a word whose low byte the engine takes.
std::int16_t key(Args& a) {
    return static_cast<std::int16_t>(a.word());
}

} // namespace

void defineTime(Probe& p) {
    // ---- time base
    p.define("SEQGETMAXPPQ", [](Args&, Probe&) -> std::uint32_t { return SeqGetMaxPPQ(); });
    p.define("SEQGETMINTEMPO", [](Args&, Probe&) -> std::uint32_t { return SeqGetMinTempo(); });
    p.define("SEQGETMAXTEMPO", [](Args&, Probe&) -> std::uint32_t { return SeqGetMaxTempo(); });
    p.define("SEQGETPPQ", [](Args& a, Probe&) -> std::uint32_t { return SeqGetPPQ(a.word()); });
    p.define("SEQSETPPQ", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSetPPQ(seq, a.word());
    });
    p.define("SEQRESCALETIME", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint32_t t = a.dword();
        const std::uint16_t from = a.word();
        return SeqRescaleTime(t, from, a.word());
    });

    // ---- tempo map
    p.define("SEQGETTEMPOMAPLEN", [](Args& a, Probe&) -> std::uint32_t { return SeqGetTempoMapLen(a.word()); });
    p.define("SEQGETTEMPOATIX", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t i = a.word();
        Out<std::uint32_t> tick(pr, a.ptr());
        return SeqGetTempoAtIx(seq, i, tick.ptr());
    });
    p.define("SEQTEMPOIXBYTICK", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTempoIxByTick(seq, a.dword());
    });
    // the tempo is a word; the DLL leaves the high word of the tick in DX
    p.define("SEQGETTEMPO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        return SeqGetPPQ(seq) == 0 ? 0 : (t & 0xffff0000U) | SeqGetTempo(seq, t);
    });
    p.define("SEQSETTEMPO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        return SeqSetTempo(seq, t, a.word());
    });
    p.define("SEQINSTEMPO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        return SeqInsTempo(seq, t, a.word());
    });
    p.define("SEQDELTEMPO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqDelTempo(seq, a.dword());
    });
    p.define("SEQCLEARTEMPOMAP", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqClearTempoMap(seq, a.word());
    });
    p.define("SEQRECALCTEMPOMAP", [](Args& a, Probe&) -> std::uint32_t { return SeqRecalcTempoMap(a.word()); });
    p.define("SEQENUMTEMPOS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t from = a.dword();
        const std::uint32_t thru = a.dword();
        const FarPtr cb = a.ptr();
        const Scoped s(glue().enumCb, cb);
        return SeqEnumTempos(seq, from, thru, cb.null() ? nullptr : tempoProc, a.dword());
    });
    p.define("SEQSWAPTEMPOMAPS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ x = a.word();
        return SeqSwapTempoMaps(x, a.word());
    });
    p.define("SEQTEMPOCOPY", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ src = a.word();
        const HSEQ clip = a.word();
        const std::uint32_t from = a.dword();
        const std::uint32_t thru = a.dword();
        const std::uint32_t r = a.dword();
        Out<std::uint16_t> flag(pr, a.ptr());
        return SeqTempoCopy(src, clip, from, thru, r, flag.ptr());
    });
    p.define("SEQTEMPOPASTE", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ clip = a.word();
        const HSEQ dst = a.word();
        const std::uint32_t at = a.dword();
        const std::uint32_t length = a.dword();
        const std::uint16_t reps = a.word();
        return SeqTempoPaste(clip, dst, at, length, reps, a.word());
    });

    // ---- tempo ratios
    p.define("SEQGETTEMPORATIO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqGetTempoRatio(seq, a.word());
    });
    p.define("SEQSETTEMPORATIO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t i = a.word();
        return SeqSetTempoRatio(seq, i, a.word());
    });
    p.define("SEQSELECTTEMPORATIO", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSelectTempoRatio(seq, a.word());
    });
    p.define("SEQWHICHTEMPORATIO", [](Args& a, Probe&) -> std::uint32_t { return SeqWhichTempoRatio(a.word()); });
    // the result is the buffer: the probe's pointer
    p.define("SEQTEMPORATIO2STR", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t ratio = a.word();
        const FarPtr buf = a.ptr();
        {
            Text text(pr, buf);
            SeqTempoRatio2Str(ratio, text.ptr());
        }
        return farResult(buf);
    });

    // ---- meter map
    p.define("SEQGETMETERMAPLEN", [](Args& a, Probe&) -> std::uint32_t { return SeqGetMeterMapLen(a.word()); });
    p.define("SEQGETMETERATIX", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t i = a.word();
        Out<std::uint16_t> measure(pr, a.ptr());
        return SeqGetMeterAtIx(seq, i, measure.ptr());
    });
    // a word; read as a double word (suite misc), DX still holds the tick's high word
    p.define("SEQMETERIXBYTICK", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t tick = a.dword();
        return SeqGetPPQ(seq) == 0 ? 0 : (tick & 0xffff0000U) | SeqMeterIxByTick(seq, tick);
    });
    p.define("SEQGETMETER", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqGetMeter(seq, a.dword());
    });
    p.define("SEQINSMETER", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t measure = a.word();
        const std::int16_t k = key(a);
        return SeqInsMeter(seq, measure, k, a.word());
    });
    p.define("SEQSETMETER", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        const std::int16_t k = key(a);
        return SeqSetMeter(seq, t, k, a.word());
    });
    p.define("SEQDELMETER", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqDelMeter(seq, a.word());
    });
    p.define("SEQCLEARMETERMAP", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::int16_t k = key(a);
        return SeqClearMeterMap(seq, k, a.word());
    });
    p.define("SEQRECALCMETERMAP", [](Args& a, Probe&) -> std::uint32_t { return SeqRecalcMeterMap(a.word()); });
    p.define("SEQENUMMETERS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t from = a.word();
        const std::uint16_t thru = a.word();
        const FarPtr cb = a.ptr();
        const Scoped s(glue().enumCb, cb);
        return SeqEnumMeters(seq, from, thru, cb.null() ? nullptr : meterProc, a.dword());
    });
    p.define("SEQSWAPMETERMAPS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ x = a.word();
        return SeqSwapMeterMaps(x, a.word());
    });
    p.define("SEQMETERCOPY", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ src = a.word();
        const HSEQ clip = a.word();
        const std::uint16_t from = a.word();
        const std::uint16_t thru = a.word();
        const std::uint16_t cut = a.word();
        const std::uint16_t r = a.word();
        Out<std::uint16_t> flag(pr, a.ptr());
        return SeqMeterCopy(src, clip, from, thru, cut, r, flag.ptr());
    });
    p.define("SEQMETERPASTE", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ clip = a.word();
        const HSEQ dst = a.word();
        const std::uint16_t measure = a.word();
        const std::uint16_t length = a.word();
        const std::uint16_t reps = a.word();
        return SeqMeterPaste(clip, dst, measure, length, reps, a.word());
    });

    // ---- positions
    p.define("SEQMBT2TICK", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t m = a.word();
        const std::uint16_t b = a.word();
        return SeqMBT2Tick(seq, m, b, a.word());
    });
    p.define("SEQTICK2MBT", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        Out<std::uint16_t> m(pr, a.ptr());
        Out<std::uint16_t> b(pr, a.ptr());
        Out<std::uint16_t> k(pr, a.ptr());
        return SeqTick2MBT(seq, t, m.ptr(), b.ptr(), k.ptr());
    });
    p.define("SEQTICK2MEAS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTick2Meas(seq, a.dword());
    });
    p.define("SEQTICK2MBTSTR", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        const FarPtr buf = a.ptr();
        {
            Text text(pr, buf);
            SeqTick2MBTStr(seq, t, text.ptr(), a.word());
        }
        return farResult(buf);
    });
    p.define("SEQMBTSTR2MBT", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::string str = pr.mem.string(a.ptr());
        Out<std::uint16_t> m(pr, a.ptr());
        Out<std::uint16_t> b(pr, a.ptr());
        Out<std::uint16_t> t(pr, a.ptr());
        return SeqMBTStr2MBT(seq, str.c_str(), m.ptr(), b.ptr(), t.ptr());
    });
    p.define("SEQTICKNEXTMEAS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTickNextMeas(seq, a.dword());
    });
    p.define("SEQTICKPREVMEAS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTickPrevMeas(seq, a.dword());
    });
    p.define("SEQTICKNEXTBEAT", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTickNextBeat(seq, a.dword());
    });
    p.define("SEQTICKPREVBEAT", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTickPrevBeat(seq, a.dword());
    });

    // ---- timecode
    p.define("SEQGETFPS", [](Args& a, Probe&) -> std::uint32_t { return SeqGetFps(a.word()); });
    p.define("SEQSETFPS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSetFps(seq, a.word());
    });
    p.define("SEQGETFRAMEOFS", [](Args& a, Probe&) -> std::uint32_t { return SeqGetFrameOfs(a.word()); });
    p.define("SEQSETFRAMEOFS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSetFrameOfs(seq, a.dword());
    });
    p.define("SEQTICK2FRAME", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqTick2Frame(seq, a.dword());
    });
    p.define("SEQFRAME2TICKRNDOPT", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t f = a.dword();
        return SeqFrame2TickRndOpt(seq, f, a.word());
    });
    p.define("SEQFRAME2HMSF", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t f = a.dword();
        Out<std::uint16_t> h(pr, a.ptr());
        Out<std::uint16_t> m(pr, a.ptr());
        Out<std::uint16_t> s(pr, a.ptr());
        Out<std::uint16_t> fr(pr, a.ptr());
        return SeqFrame2HMSF(seq, f, h.ptr(), m.ptr(), s.ptr(), fr.ptr());
    });
    p.define("SEQHMSF2FRAME", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint16_t h = a.word();
        const std::uint16_t m = a.word();
        const std::uint16_t s = a.word();
        const std::uint16_t f = a.word();
        return SeqHMSF2Frame(seq, h, m, s, f, a.word());
    });
    p.define("SEQTICK2HMSFSTR", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t t = a.dword();
        const FarPtr buf = a.ptr();
        {
            Text text(pr, buf);
            SeqTick2HMSFStr(seq, t, text.ptr());
        }
        return farResult(buf);
    });
    p.define("SEQHMSFSTR2HMSF", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::string str = pr.mem.string(a.ptr());
        Out<std::uint16_t> h(pr, a.ptr());
        Out<std::uint16_t> m(pr, a.ptr());
        Out<std::uint16_t> s(pr, a.ptr());
        Out<std::uint16_t> f(pr, a.ptr());
        return SeqHMSFStr2HMSF(str.c_str(), h.ptr(), m.ptr(), s.ptr(), f.ptr());
    });
    p.define("SEQHMSFSTR2MBT", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::string str = pr.mem.string(a.ptr());
        Out<std::uint16_t> m(pr, a.ptr());
        Out<std::uint16_t> b(pr, a.ptr());
        Out<std::uint16_t> t(pr, a.ptr());
        return SeqHMSFStr2MBT(seq, str.c_str(), m.ptr(), b.ptr(), t.ptr());
    });
}

} // namespace ttsrun
