// The TTSSEQ 2.10 file functions for the script runner: loading and saving `.WRK` and MIDI files
// with the program's callback, the `SeqIO` functions the callbacks use, and the kinds of string
// events with their procedures.
#include "api.hpp"

#include <vector>

namespace ttsrun {

namespace {

/// The program's load or save callback: (code, word, value) as four words.
std::uint16_t fileProc(const std::uint16_t code, const std::uint16_t word, const std::uint32_t value) {
    const Glue& g = glue();
    return static_cast<std::uint16_t>(g.probe->callback(g.fileCb, {code, word, hi(value), lo(value)}));
}

/// An event procedure: (message, pointer to the event, length). The probe's procedure of the
/// event's kind gets the event where the caller of SeqCallEventProc has it, otherwise a place of
/// its own in the engine's memory.
std::uint16_t eventProc(const std::uint16_t message, SEQEVENT* event, const std::uint16_t length) {
    Glue& g = glue();
    Probe& pr = *g.probe;
    const auto kind = g.kinds.find(event->status);
    if (kind == g.kinds.end()) {
        return 0;
    }
    FarPtr at;
    if (event == g.callerEvent) {
        at = g.callerFar;
        g.callerEvent = nullptr;
    } else {
        std::vector<std::uint8_t> bytes(sizeof(SEQEVENT));
        std::memcpy(bytes.data(), event, sizeof(SEQEVENT));
        at = pr.mem.window(bytes);
    }
    const std::uint32_t r = pr.callback(kind->second, {message, at.seg, at.off, length});
    readEvent(pr, at, *event);
    return static_cast<std::uint16_t>(r);
}

} // namespace

void defineFiles(Probe& p) {
    p.define("SEQLOADCAKEWALKFILE", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        const std::uint16_t r = a.word(); // always 0
        const FarPtr fn = a.ptr();
        const Scoped s(glue().fileCb, fn);
        return SeqLoadCakewalkFile(path.c_str(), r, fn.null() ? nullptr : fileProc, a.dword());
    });
    p.define("SEQSAVECAKEWALKFILE", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        const std::uint32_t r = a.dword(); // always 0
        const FarPtr fn = a.ptr();
        const Scoped s(glue().fileCb, fn);
        return SeqSaveCakewalkFile(seq, path.c_str(), r, fn.null() ? nullptr : fileProc, a.dword());
    });
    p.define("SEQLOADMIDIFILE", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        return SeqLoadMidiFile(path.c_str(), a.word());
    });
    p.define("SEQSAVEMIDIFILE", [](Args& a, Probe& pr) -> std::uint32_t {
        const HSEQ seq = a.word();
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        return SeqSaveMidiFile(seq, path.c_str(), a.dword());
    });

    // ---- the file the engine has open
    p.define("SEQIOSEEK", [](Args& a, Probe&) -> std::uint32_t {
        const std::uint16_t file = a.word();
        const auto offset = static_cast<std::int32_t>(a.dword());
        return SeqIOSeek(file, offset, a.word());
    });
    p.define("SEQIOREAD", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t file = a.word();
        const FarPtr to = a.ptr();
        const std::uint16_t n = a.word();
        return SeqIORead(file, pr.mem.at(to, n), n);
    });
    p.define("SEQIOWRITE", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t file = a.word();
        const FarPtr from = a.ptr();
        const std::uint16_t n = a.word();
        return SeqIOWrite(file, pr.mem.at(from, n), n);
    });

    // ---- kinds of string events
    p.define("SEQREGISTEREVENTKIND", [](Args& a, Probe& pr) -> std::uint32_t {
        // TTSSEQ keeps the name the program passes; libseq keeps a copy
        const std::string name = pr.mem.string(a.ptr());
        const FarPtr fn = a.ptr();
        const std::uint16_t kind = SeqRegisterEventKind(name.c_str(), fn.null() ? nullptr : eventProc, a.dword());
        if (kind != 0) {
            glue().kinds[kind] = fn;
        }
        return kind;
    });
    p.define("SEQUNREGISTEREVENTKIND", [](Args& a, Probe& pr) -> std::uint32_t {
        return SeqUnregisterEventKind(pr.mem.string(a.ptr()).c_str());
    });
    p.define("SEQCALLEVENTPROC", [](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t message = a.word();
        const FarPtr at = a.ptr();
        const std::uint16_t length = a.word();
        SEQEVENT ev{};
        if (!readEvent(pr, at, ev)) {
            return 0;
        }
        Glue& g = glue();
        g.callerEvent = &ev;
        g.callerFar = at;
        const std::uint16_t r = SeqCallEventProc(message, &ev, length);
        g.callerEvent = nullptr;
        writeEvent(pr, at, ev);
        return r;
    });
}

} // namespace ttsrun
