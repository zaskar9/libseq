// The TTSSEQ 1.01 interface for the script runner: markers and sysex banks
// (docs/reverse-engineering/signatures.md, "Markers" and "Sysex banks").
#include "api.hpp"

namespace ttsrun {

namespace {

/// SeqReceiveSysxBank: (received, free) as four words.
std::uint16_t sysxProc(const std::uint32_t received, const std::uint32_t free) {
    const Glue& g = glue();
    return static_cast<std::uint16_t>(g.probe->callback(g.sysxCb, {hi(received), lo(received), hi(free), lo(free)}));
}

} // namespace

void defineMarks101(Probe& p) {
    // ---- markers
    p.define("SEQNUMMARKERS", [](Args& a, Probe&) -> std::uint32_t { return SeqNumMarkers(a.word()); });
    p.define("SEQGETMARKERNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t i = a.dword();
        Text name(pr, a.ptr());
        return SeqGetMarkerName(seq, i, name.ptr(), a.word());
    });
    p.define("SEQSETMARKERNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t i = a.dword();
        return SeqSetMarkerName(seq, i, pr.mem.string(a.ptr()).c_str());
    });
    p.define("SEQGETMARKERTIME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t i = a.dword();
        Out<std::uint32_t> time(pr, a.ptr());
        Out<std::uint16_t> flag(pr, a.ptr());
        return SeqGetMarkerTime(seq, i, time.ptr(), flag.ptr());
    });
    p.define("SEQSETMARKERTIME", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::uint32_t i = a.dword();
        const std::uint32_t t = a.dword();
        return SeqSetMarkerTime(seq, i, t, a.word());
    });
    p.define("SEQINSMARKER", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::string name = pr.mem.string(a.ptr());
        const std::uint32_t t = a.dword();
        return SeqInsMarker(seq, name.c_str(), t, a.word());
    });
    p.define("SEQDELMARKER", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqDelMarker(seq, a.dword());
    });
    p.define("SEQSORTMARKERS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqSortMarkers(seq, a.dword());
    });
    p.define("SEQMARKERFORTIME", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        return SeqMarkerForTime(seq, a.dword());
    });

    // ---- sysex banks
    p.define("SEQCREATESYSXBANK", [](Args& a, Probe&) -> std::uint32_t { return SeqCreateSysxBank(a.word()); });
    p.define("SEQDESTROYSYSXBANK", [](Args& a, Probe&) -> std::uint32_t { return SeqDestroySysxBank(a.word()); });
    p.define("SEQEMPTYSYSXBANK", [](Args& a, Probe&) -> std::uint32_t { return SeqEmptySysxBank(a.word()); });
    p.define("SEQENUMSYSXBANKS", [](Args& a, Probe&) -> std::uint32_t {
        const HSEQ seq = a.word();
        const FarPtr cb = a.ptr();
        const Scoped s(glue().enumCb, cb);
        return SeqEnumSysxBanks(seq, enumCallback(cb), a.dword());
    });
    p.define("SEQGETSYSXBANKPARAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        return dx(SeqGetSysxBankParam(b, a.word()));
    });
    p.define("SEQSETSYSXBANKPARAM", [](Args& a, Probe&) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        const std::uint16_t param = a.word();
        return SeqSetSysxBankParam(b, param, static_cast<std::int32_t>(a.dword()));
    });
    p.define("SEQGETSYSXBANKNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        Text name(pr, a.ptr());
        return SeqGetSysxBankName(b, name.ptr(), a.word());
    });
    p.define("SEQSETSYSXBANKNAME", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        return SeqSetSysxBankName(b, pr.mem.string(a.ptr()).c_str());
    });
    p.define("SEQGETSYSXBANKLEN", [](Args& a, Probe&) -> std::uint32_t { return SeqGetSysxBankLen(a.word()); });
    p.define("SEQGETSYSXASCLEN", [](Args& a, Probe&) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        return SeqGetSysxAscLen(b, a.word());
    });
    p.define("SEQSYSXBIN2ASC", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        Text text(pr, a.ptr());
        return SeqSysxBin2Asc(b, text.ptr(), a.word());
    });
    p.define("SEQSYSXASC2BIN", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        const std::string text = pr.mem.string(a.ptr());
        Out<std::uint32_t> position(pr, a.ptr());
        return SeqSysxAsc2Bin(b, text.c_str(), position.ptr());
    });
    p.define("SEQSAVESYSXBANK", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        return SeqSaveSysxBank(b, pr.mapPath(pr.mem.string(a.ptr())).c_str());
    });
    p.define("SEQLOADSYSXBANK", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        return SeqLoadSysxBank(b, pr.mapPath(pr.mem.string(a.ptr())).c_str());
    });
    p.define("SEQRECEIVESYSXBANK", [](Args& a, Probe&) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        const FarPtr fn = a.ptr();
        const std::uint16_t r1 = a.word();
        const std::uint16_t r2 = a.word();
        const Scoped s(glue().sysxCb, fn);
        return static_cast<std::uint16_t>(SeqReceiveSysxBank(b, fn.null() ? nullptr : sysxProc, r1, r2, a.word()));
    });
    p.define("SEQSENDSYSXBANK", [](Args& a, Probe&) -> std::uint32_t {
        const HSYSXBANK b = a.word();
        const std::uint16_t r1 = a.word();
        return SeqSendSysxBank(b, r1, a.word());
    });
}

} // namespace ttsrun
