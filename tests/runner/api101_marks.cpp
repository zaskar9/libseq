// The TTSSEQ 1.01 interface over libseq, for the script runner: markers and sysex banks
// (docs/reverse-engineering/signatures.md, "Markers" and "Sysex banks").
#include "api.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>

namespace ttsrun {

using libseq::Engine;
using libseq::Marker;
using libseq::Sequence;
using libseq::Tick;

void defineMarks101(Probe& p, Engine& e) {
    const auto seq = [&e](Args& a) -> Sequence* { return e.sequence(a.word()); };

    // ---- markers
    p.define("SEQNUMMARKERS", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->markers.size());
    });
    p.define("SEQGETMARKERNAME", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::uint32_t i = a.dword();
        const FarPtr buf = a.ptr();
        const std::size_t size = a.word();
        if (s == nullptr || size == 0) {
            return 0;
        }
        const std::string name = i < s->markers.size() ? s->markers[i].name : std::string();
        const std::string cut = name.substr(0, std::min(name.size(), size - 1));
        putString(pr, buf, cut);
        return static_cast<std::uint32_t>(cut.size());
    });
    p.define("SEQSETMARKERNAME", [seq](Args& a, Probe& pr) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint32_t i = a.dword();
        const std::string name = pr.mem.string(a.ptr());
        if (s == nullptr || i >= s->markers.size()) {
            return 0;
        }
        s->markers[i].name = name;
        return 1;
    });
    p.define("SEQGETMARKERTIME", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const std::uint32_t i = a.dword();
        const FarPtr time = a.ptr();
        const FarPtr flag = a.ptr();
        if (s == nullptr || i >= s->markers.size()) {
            return 0;
        }
        putDword(pr, time, s->markerTime(s->markers[i]));
        if (!flag.null()) {
            putWord(pr, flag, s->markers[i].smpteLocked ? 1 : 0);
        }
        return 1;
    });
    p.define("SEQSETMARKERTIME", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint32_t i = a.dword();
        const Tick t = a.dword();
        const bool flag = a.word() != 0;
        if (s == nullptr || i >= s->markers.size()) {
            return 0;
        }
        s->setMarkerTime(s->markers[i], t, flag);
        return 1;
    });
    p.define("SEQINSMARKER", [seq](Args& a, Probe& pr) -> std::uint32_t {
        Sequence* s = seq(a);
        Marker m;
        m.name = pr.mem.string(a.ptr());
        const Tick t = a.dword();
        const bool locked = a.word() != 0;
        if (s == nullptr) {
            return 0;
        }
        s->setMarkerTime(m, t, locked);
        s->insertMarker(std::move(m));
        return 1;
    });
    p.define("SEQDELMARKER", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        const std::uint32_t i = a.dword();
        if (s == nullptr || i >= s->markers.size()) {
            return 0;
        }
        s->markers.erase(s->markers.begin() + static_cast<std::ptrdiff_t>(i));
        return 1;
    });
    p.define("SEQSORTMARKERS", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        a.dword();
        if (s == nullptr) {
            return 0;
        }
        s->sortMarkers();
        return static_cast<std::uint32_t>(s->markers.size());
    });
    p.define("SEQMARKERFORTIME", [seq](Args& a, Probe&) -> std::uint32_t {
        const Sequence* s = seq(a);
        const Tick t = a.dword();
        return s == nullptr ? 0 : static_cast<std::uint32_t>(s->markerForTime(t));
    });

    // ---- sysex banks
    const auto bank = [&e](Args& a) -> libseq::SysxBank* {
        const libseq::StreamHandle h = a.word();
        Sequence* s = e.sequence(libseq::seqOf(h));
        return s == nullptr ? nullptr : s->sysxBank(h);
    };
    p.define("SEQCREATESYSXBANK", [seq](Args& a, Probe&) -> std::uint32_t {
        Sequence* s = seq(a);
        return s == nullptr ? 0 : s->createSysxBank();
    });
    p.define("SEQDESTROYSYSXBANK", [&e](Args& a, Probe&) -> std::uint32_t {
        const libseq::StreamHandle h = a.word();
        Sequence* s = e.sequence(libseq::seqOf(h));
        return s != nullptr && s->sysxBanks.erase(libseq::idOf(h)) == 1 ? 1 : 0;
    });
    p.define("SEQEMPTYSYSXBANK", [bank](Args& a, Probe&) -> std::uint32_t {
        libseq::SysxBank* b = bank(a);
        if (b == nullptr) {
            return 0;
        }
        b->data.clear();
        return 1;
    });
    p.define("SEQENUMSYSXBANKS", [seq](Args& a, Probe& pr) -> std::uint32_t {
        const Sequence* s = seq(a);
        const FarPtr cb = a.ptr();
        const std::uint32_t value = a.dword();
        if (s == nullptr) {
            return 0;
        }
        const auto hi = static_cast<std::uint16_t>(value >> 16);
        const auto lo = static_cast<std::uint16_t>(value & 0xffffU);
        for (const auto& [id, b] : s->sysxBanks) {
            pr.callback(cb, {s->handle(), libseq::makeStreamHandle(s->handle(), id), hi, lo});
        }
        pr.callback(cb, {0, 0, hi, lo});
        return 1;
    });
    p.define("SEQGETSYSXBANKPARAM", [bank](Args& a, Probe&) -> std::uint32_t {
        const libseq::SysxBank* b = bank(a);
        const int param = a.word();
        return b == nullptr ? 0 : static_cast<std::uint32_t>(b->param(param));
    });
    p.define("SEQSETSYSXBANKPARAM", [bank](Args& a, Probe&) -> std::uint32_t {
        libseq::SysxBank* b = bank(a);
        const int param = a.word();
        const auto v = static_cast<std::int32_t>(a.dword());
        return b != nullptr && b->setParam(param, v) ? 1 : 0;
    });
    p.define("SEQGETSYSXBANKNAME", [bank](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SysxBank* b = bank(a);
        const FarPtr buf = a.ptr();
        const std::size_t size = a.word();
        if (b == nullptr || size == 0) {
            return 0;
        }
        // as much of the name as fits; 1 when all of a name did
        putString(pr, buf, b->name.substr(0, std::min(b->name.size(), size - 1)));
        return !b->name.empty() && b->name.size() < size ? 1 : 0;
    });
    p.define("SEQSETSYSXBANKNAME", [bank](Args& a, Probe& pr) -> std::uint32_t {
        libseq::SysxBank* b = bank(a);
        const std::string name = pr.mem.string(a.ptr());
        if (b == nullptr) {
            return 0;
        }
        b->name = name;
        return 1;
    });
    p.define("SEQGETSYSXBANKLEN", [bank](Args& a, Probe&) -> std::uint32_t {
        const libseq::SysxBank* b = bank(a);
        return b == nullptr ? 0 : static_cast<std::uint32_t>(b->data.size());
    });
    p.define("SEQGETSYSXASCLEN", [bank](Args& a, Probe&) -> std::uint32_t {
        const libseq::SysxBank* b = bank(a);
        const std::size_t perLine = a.word();
        // with 0 bytes per line TTSSEQ divides by zero
        if (b == nullptr || perLine == 0) {
            return 0;
        }
        return static_cast<std::uint32_t>(libseq::sysxToText(b->data, perLine).size() + 1);
    });
    p.define("SEQSYSXBIN2ASC", [bank](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SysxBank* b = bank(a);
        const FarPtr buf = a.ptr();
        const std::size_t perLine = a.word();
        // an empty bank leaves the buffer as it is
        if (b == nullptr || perLine == 0 || b->data.empty()) {
            return 0;
        }
        putString(pr, buf, libseq::sysxToText(b->data, perLine));
        return static_cast<std::uint32_t>(b->data.size());
    });
    p.define("SEQSYSXASC2BIN", [bank](Args& a, Probe& pr) -> std::uint32_t {
        libseq::SysxBank* b = bank(a);
        const std::string text = pr.mem.string(a.ptr());
        const FarPtr position = a.ptr();
        if (b == nullptr) {
            return 0;
        }
        libseq::SysxParse r = libseq::sysxFromText(text);
        if (r.result == libseq::SysxParse::Ok) {
            b->data = std::move(r.data);
        } else if (r.result == libseq::SysxParse::NotHex) {
            putDword(pr, position, static_cast<std::uint32_t>(r.position));
        }
        return r.result;
    });
    p.define("SEQSAVESYSXBANK", [bank](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SysxBank* b = bank(a);
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        if (b == nullptr) {
            return 0;
        }
        std::ofstream f(path, std::ios::binary);
        const std::string bytes(b->data.begin(), b->data.end());
        f << bytes;
        return f ? 1 : 0;
    });
    p.define("SEQLOADSYSXBANK", [bank](Args& a, Probe& pr) -> std::uint32_t {
        libseq::SysxBank* b = bank(a);
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        std::ifstream f(path, std::ios::binary);
        if (b == nullptr || !f) {
            return 0;
        }
        b->data.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
        return 1;
    });
}

} // namespace ttsrun
