// The TTSSEQ 1.01 file functions over libseq, for the script runner: loading and saving `.WRK`
// and MIDI files with the program's callback, the `SeqIO` functions the callbacks use, and the
// kinds of string events with their procedures.
#include "api.hpp"

#include "libseq/engine.hpp"

#include <memory>
#include <utility>

namespace ttsrun {

using libseq::Engine;
using libseq::Event;

namespace {

/// The program's load or save callback: (code, word, value32) as four words.
libseq::FileProc fileProc(Probe& p, const FarPtr fn, const std::uint32_t value) {
    if (fn.null()) {
        return {};
    }
    return [&p, fn, value](const std::uint16_t code, const std::uint16_t word) {
        return p.callback(
            fn, {code, word, static_cast<std::uint16_t>(value >> 16), static_cast<std::uint16_t>(value & 0xffffU)});
    };
}

} // namespace

void defineFiles101(Probe& p, Engine& e) {
    // where SeqCallEventProc's caller has the event: the procedure gets that pointer
    const auto callerEvent = std::make_shared<FarPtr>();

    p.define("SEQLOADCAKEWALKFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        a.word(); // always 0
        const FarPtr fn = a.ptr();
        return e.loadCakewalkFile(path, fileProc(pr, fn, a.dword()));
    });
    p.define("SEQSAVECAKEWALKFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        a.dword(); // always 0
        const FarPtr fn = a.ptr();
        return e.saveCakewalkFile(seq, path, fileProc(pr, fn, a.dword())) ? 1 : 0;
    });
    p.define("SEQLOADMIDIFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        a.word(); // always 0
        return e.loadMidiFile(path);
    });
    p.define("SEQSAVEMIDIFILE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        const libseq::SeqHandle seq = a.word();
        const std::string path = pr.mapPath(pr.mem.string(a.ptr()));
        a.dword(); // always 0
        return e.saveMidiFile(seq, path) ? 1 : 0;
    });

    // ---- the file the engine has open, whatever handle comes
    p.define("SEQIOSEEK", [&e](Args& a, Probe&) -> std::uint32_t {
        a.word();
        const auto offset = static_cast<std::int32_t>(a.dword());
        return e.ioSeek(offset, a.word());
    });
    p.define("SEQIOREAD", [&e](Args& a, Probe& pr) -> std::uint32_t {
        a.word();
        const FarPtr to = a.ptr();
        const std::uint16_t n = a.word();
        std::uint8_t* b = pr.mem.at(to, n);
        return b == nullptr ? 0 : static_cast<std::uint32_t>(e.ioRead(b, n));
    });
    p.define("SEQIOWRITE", [&e](Args& a, Probe& pr) -> std::uint32_t {
        a.word();
        const FarPtr from = a.ptr();
        const std::uint16_t n = a.word();
        const std::uint8_t* b = pr.mem.at(from, n);
        return b == nullptr ? 0 : static_cast<std::uint32_t>(e.ioWrite(b, n));
    });

    // ---- kinds of string events. A procedure gets (message, pointer to the event, length).
    p.define("SEQREGISTEREVENTKIND", [&e, callerEvent](Args& a, Probe& pr) -> std::uint32_t {
        std::string name = pr.mem.string(a.ptr());
        const FarPtr fn = a.ptr();
        const std::uint32_t value = a.dword();
        return e.registerEventKind(
            std::move(name),
            [&pr, fn, callerEvent](const std::uint16_t message, Event& ev, const std::uint16_t length) {
                // the event where the caller of SeqCallEventProc has it, otherwise a place of its own
                const auto b = ev.bytes();
                FarPtr at = *callerEvent;
                *callerEvent = {};
                if (at.null()) {
                    at = pr.mem.window({b.begin(), b.end()});
                }
                const std::uint32_t r = pr.callback(fn, {message, at.seg, at.off, length});
                if (const std::uint8_t* p8 = pr.mem.at(at, Event::kSize)) {
                    ev = Event::fromBytes(p8);
                }
                return r;
            },
            value);
    });
    p.define("SEQUNREGISTEREVENTKIND", [&e](Args& a, Probe& pr) -> std::uint32_t {
        return e.unregisterEventKind(pr.mem.string(a.ptr())) ? 1 : 0;
    });
    p.define("SEQCALLEVENTPROC", [&e, callerEvent](Args& a, Probe& pr) -> std::uint32_t {
        const std::uint16_t message = a.word();
        const FarPtr at = a.ptr();
        const std::uint16_t length = a.word();
        std::uint8_t* b = pr.mem.at(at, Event::kSize);
        if (b == nullptr) {
            return 0;
        }
        Event ev = Event::fromBytes(b);
        *callerEvent = at;
        const std::uint32_t r = e.callEventProc(message, ev, length);
        *callerEvent = {};
        return r & 0xffffU;
    });
}

} // namespace ttsrun
