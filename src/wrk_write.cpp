// libseq: writing a sequence as a Cakewalk `.WRK` file (SeqSaveCakewalkFile, TTSSEQ 1.01).
//
// The chunks, their order and their bytes are those 1.01 writes without a program callback
// (tests/data/ttsseq/101/files): the program's chunks (the string table and the comments) are not
// written, and VARS byte 77 (send MIDI sync) is 0. All integers are little-endian.
#include "bytes.hpp"
#include "wrk_chunks.hpp"

#include "libseq/engine.hpp"

#include <algorithm>

namespace libseq {

namespace {

using bytes::Writer;

void chunk(Writer& w, const std::uint8_t type, const Writer& body) {
    w.u8(type);
    w.u32(static_cast<std::uint32_t>(body.size()));
    w.bytes(body.out);
}

std::uint8_t byte(const std::int32_t v) {
    return static_cast<std::uint8_t>(v & 0xff);
}

/// The VARS chunk (89 bytes): the times and the transport, metronome, filter and tempo ratio
/// parameters, and the end of the sequence.
Writer vars(const Sequence& s, const Tick end) {
    std::vector<std::uint8_t> v(89, 0);
    const auto put32 = [&v](const std::size_t at, const std::uint32_t x) {
        for (std::size_t i = 0; i < 4; ++i) {
            v[at + i] = static_cast<std::uint8_t>((x >> (8 * i)) & 0xffU);
        }
    };
    put32(0, s.now);
    // 4 to 11, the program's From and Thru, stay 0. 12 is the tonic of the first key signature
    // (7 semitones per sharp: 5 for one flat, F).
    const int key = s.meterMap.entries().front().key;
    v[12] = static_cast<std::uint8_t>(((key * 7) % 12 + 12) % 12);
    v[17] = byte(s.param(qp::kZeroControllers));
    v[18] = byte(s.param(qp::kSendSpp));
    v[19] = byte(s.param(qp::kUseContinue));
    v[20] = byte(s.param(qp::kSearchBack));
    v[21] = byte(s.param(qp::kAutoStop));
    put32(22, static_cast<std::uint32_t>(s.param(qp::kStopTime)));
    v[26] = byte(s.param(qp::kAutoRewind));
    put32(27, static_cast<std::uint32_t>(s.param(qp::kRewindTime)));
    v[31] = byte(s.param(qp::kMetroPlay));
    v[32] = byte(s.param(qp::kMetroRecord));
    v[33] = byte(s.param(qp::kMetroAccent));
    v[34] = byte(s.param(qp::kCountIn));
    v[35] = 0xff; // the same in every file 1.01 writes
    v[36] = 0xff;
    v[37] = 1;
    v[38] = byte(s.param(qp::kFilter));
    v[57] = byte(s.param(qp::kAutoRestart));
    v[58] = byte(s.tempoRatio);
    for (std::size_t i = 0; i < 3; ++i) {
        v[59 + i] = byte(s.tempoRatios[i]);
    }
    put32(73, end);
    Writer w;
    w.bytes(v);
    return w;
}

/// Appends what the writer holds to the file and empties it.
template <class File>
void emit(File& f, Writer& w) {
    f.data.insert(f.data.end(), w.out.begin(), w.out.end());
    f.pos = f.data.size();
    w.out.clear();
}

} // namespace

bool Engine::stringStream(const Stream& st, const std::uint32_t number, bytes::Writer& w) {
    // A stream with string events is a chunk 18: each event with only the bytes it needs, a
    // string event as its time and kind followed by what its procedure writes (message 5).
    emit(io_, w);
    w.u8(wrk::STRSTREAM);
    const std::size_t at = io_.data.size() + 1;
    w.u32(0);
    w.u16(number);
    w.u32(static_cast<std::uint32_t>(st.events().size()));
    for (const Event& e : st.events()) {
        w.u24(e.time);
        w.u8(e.status);
        if (e.status < 0x80) {
            emit(io_, w);
            Event copy = e;
            if (callEventProc(ek::kSave, copy, 7) == 0) {
                return false;
            }
            io_.pos = io_.data.size();
            continue;
        }
        w.u8(e.data1);
        switch (e.status & 0xf0U) {
        case 0x90:
            w.u8(e.data2);
            w.u16(e.duration);
            break;
        case 0xa0:
        case 0xb0:
        case 0xe0:
            w.u8(e.data2);
            break;
        default:
            break;
        }
    }
    emit(io_, w);
    const auto len = static_cast<std::uint32_t>(io_.data.size() - at - 4);
    for (std::size_t i = 0; i < 4; ++i) {
        io_.data[at + i] = static_cast<std::uint8_t>((len >> (8 * i)) & 0xffU);
    }
    return true;
}

bool Engine::saveCakewalkFile(const SeqHandle seq, const std::filesystem::path& path, const FileProc& callback) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return false;
    }
    const auto ask = [&callback, this](const std::uint16_t code, const std::uint16_t word) -> std::uint32_t {
        io_.pos = io_.data.size();
        return callback ? callback(code, word) : 1U;
    };
    // the program may cancel before anything is written
    if (ask(fileproc::kStart, 0) == 0) {
        return false;
    }
    io_ = OpenFile{{}, 0, true};
    // a failed save leaves no file behind
    const auto fail = [this, &path] {
        io_ = OpenFile{};
        std::error_code ec;
        std::filesystem::remove(path, ec);
        return false;
    };
    Writer w;
    w.str("CAKEWALK");
    w.u8(0x1a);
    w.u8(0); // version 2.0
    w.u8(2);

    Tick end = 0;
    for (const Stream* st : s->streams()) {
        end = std::max(end, st->duration());
    }

    // the percentage done after each stream and sysex bank
    const std::size_t parts = s->streams().size() + s->sysxBanks.size();
    std::size_t done = 0;
    const auto progress = [&] {
        ++done;
        ask(fileproc::kStart, static_cast<std::uint16_t>(done * 100 / parts));
    };

    Writer b;
    b.u16(s->ppq);
    chunk(w, wrk::TIMEBASE, b);
    chunk(w, wrk::VARS, vars(*s, end));
    emit(io_, w);
    if (ask(fileproc::kVars, seq) == 0) {
        return fail();
    }
    // the kinds of string events, when any is registered
    if (!kinds_.empty()) {
        b = Writer{};
        b.u16(static_cast<std::uint32_t>(kinds_.size()));
        for (const auto& [id, k] : kinds_) {
            b.pstr(k.name.substr(0, 255));
            b.u8(id);
        }
        chunk(w, wrk::STRTAB, b);
    }

    b = Writer{};
    b.u16(static_cast<std::uint32_t>(s->tempoMap.size()));
    for (const TempoEntry& e : s->tempoMap.entries()) {
        b.u32(e.tick);
        b.u32(0);
        b.u16(e.tempo);
        b.u32(0);
        b.u32(0);
    }
    chunk(w, wrk::NTEMPO, b);

    b = Writer{};
    b.u16(static_cast<std::uint32_t>(s->meterMap.size()));
    for (const MeterEntry& m : s->meterMap.entries()) {
        b.u32(0);
        b.u16(m.measure + 1U);
        b.u8(m.numerator);
        b.u8(m.denominatorLog2);
        b.u32(0);
    }
    chunk(w, wrk::METER, b);

    b = Writer{};
    b.u16(static_cast<std::uint32_t>(s->meterMap.size()));
    for (const MeterEntry& m : s->meterMap.entries()) {
        b.u16(m.measure + 1U);
        b.u8(m.numerator);
        b.u8(m.denominatorLog2);
        b.s8(m.key);
    }
    chunk(w, wrk::METERKEY, b);

    b = Writer{};
    b.u16(s->fps);
    b.u32(s->frameOffset);
    chunk(w, wrk::TIMEFMT, b);

    b = Writer{};
    for (const int p : {qp::kMetroPort, qp::kMetroChannel, qp::kMetroKey, qp::kMetroVelocity, qp::kMetroDuration}) {
        b.u16(static_cast<std::uint32_t>(s->param(p)) & 0xffffU);
    }
    b.u16(0);
    b.u16(0);
    chunk(w, wrk::METRONOME, b);

    b = Writer{};
    b.u8(byte(s->param(qp::kThruMode)));
    b.u8(0);
    b.u8(byte(s->param(qp::kThruPort)));
    b.u8(byte(s->param(qp::kThruChannel)));
    b.u8(byte(s->param(qp::kThruKeyPlus)));
    b.u8(byte(s->param(qp::kThruVelPlus)));
    b.s16(s->param(qp::kLocalOnPort));
    b.u32(0);
    chunk(w, wrk::THRU, b);

    if (!s->markers.empty()) {
        b = Writer{};
        b.u32(static_cast<std::uint32_t>(s->markers.size()));
        for (const Marker& m : s->markers) {
            b.u8(m.smpteLocked ? 1U : 0U);
            b.u8(0);
            b.u24(s->markerTime(m));
            b.u32(0);
            b.u8(0);
            b.pstr(m.name.substr(0, 255));
        }
        chunk(w, wrk::MARKERS, b);
    }

    for (const Stream* st : s->streams()) {
        const auto number = static_cast<std::uint32_t>(st->param(sp::kPosition));
        const std::string& name = st->name();
        b = Writer{};
        b.u16(number);
        b.pstr(name.substr(0, std::min<std::size_t>(name.size(), 15)));
        b.pstr(name.size() > 15 ? name.substr(15, 15) : std::string());
        b.u8(byte(st->param(sp::kChannel)));
        b.u8(byte(st->param(sp::kKeyPlus)));
        b.u8(byte(st->param(sp::kVelPlus)));
        b.u8(byte(st->param(sp::kPort)));
        unsigned flags = 0;
        flags |= st->param(sp::kMuted) != 0 ? 2U : 0U;
        flags |= st->param(sp::kLoopCount) != 1 ? 4U : 0U;
        flags |= st->param(sp::kArchived) != 0 ? 0x10U : 0U;
        b.u8(flags);
        chunk(w, wrk::TRACK, b);

        b = Writer{};
        b.u16(number);
        b.pstr(name.substr(0, 255));
        chunk(w, wrk::TRKNAME, b);

        const bool strings =
            std::any_of(st->events().begin(), st->events().end(), [](const Event& e) { return e.status < 0x80; });
        if (!strings) {
            b = Writer{};
            b.u16(number);
            b.u16(static_cast<std::uint32_t>(st->events().size()));
            for (const Event& e : st->events()) {
                const auto r = e.bytes();
                b.bytes(r.data(), r.size());
            }
            chunk(w, wrk::STREAM, b);
        } else if (!stringStream(*st, number, w)) {
            return fail();
        }

        if (st->param(sp::kPatch) >= 0) {
            b = Writer{};
            b.u16(number);
            b.u16(static_cast<std::uint32_t>(st->param(sp::kPatch)));
            chunk(w, wrk::TRKPATCH, b);
        }
        const std::int32_t reps = st->param(sp::kLoopCount);
        if (reps != 1 && reps != 9999) {
            b = Writer{};
            b.u16(number);
            b.u16(static_cast<std::uint32_t>(reps));
            chunk(w, wrk::TRKREPS, b);
        }
        if (st->param(sp::kTimePlus) != 0) {
            b = Writer{};
            b.u16(number);
            b.s16(st->param(sp::kTimePlus));
            chunk(w, wrk::TRKOFFS, b);
        }
        if (st->param(sp::kVolume) >= 0 || st->param(sp::kPan) >= 0) {
            b = Writer{};
            b.u16(number);
            b.s16(st->param(sp::kVolume));
            b.s16(st->param(sp::kPan));
            chunk(w, wrk::TRKVOL, b);
        }
        emit(io_, w);
        progress();
    }

    for (const auto& [id, bank] : s->sysxBanks) {
        b = Writer{};
        b.u8(byte(bank.number));
        b.u16(static_cast<std::uint32_t>(bank.data.size()));
        b.u8(bank.autoSend ? 1U : 0U);
        b.pstr(bank.name.substr(0, 255));
        b.bytes(bank.data);
        chunk(w, wrk::SYSEX, b);
        emit(io_, w);
        progress();
    }

    emit(io_, w);
    ask(fileproc::kEnd, fileproc::kFile);
    w.u8(wrk::END);
    io_.pos = io_.data.size();
    emit(io_, w);
    const std::vector<std::uint8_t> out = std::move(io_.data);
    io_ = OpenFile{};
    try {
        bytes::writeFileBytes(path.string(), out);
    } catch (const Error&) {
        fileError_ = 7;
        return false;
    }
    fileError_ = 0;
    return true;
}

} // namespace libseq
