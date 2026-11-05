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

} // namespace

bool Engine::saveCakewalkFile(const SeqHandle seq, const std::filesystem::path& path) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return false;
    }
    Writer w;
    w.str("CAKEWALK");
    w.u8(0x1a);
    w.u8(0); // version 2.0
    w.u8(2);

    Tick end = 0;
    for (const Stream* st : s->streams()) {
        end = std::max(end, st->duration());
    }

    Writer b;
    b.u16(s->ppq);
    chunk(w, wrk::TIMEBASE, b);
    chunk(w, wrk::VARS, vars(*s, end));

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

        b = Writer{};
        b.u16(number);
        b.u16(static_cast<std::uint32_t>(st->events().size()));
        for (const Event& e : st->events()) {
            const auto r = e.bytes();
            b.bytes(r.data(), r.size());
        }
        chunk(w, wrk::STREAM, b);

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
    }

    for (const auto& [id, bank] : s->sysxBanks) {
        b = Writer{};
        b.u8(byte(bank.number));
        b.u16(static_cast<std::uint32_t>(bank.data.size()));
        b.u8(bank.autoSend ? 1U : 0U);
        b.pstr(bank.name.substr(0, 255));
        b.bytes(bank.data);
        chunk(w, wrk::SYSEX, b);
    }

    w.u8(wrk::END);
    try {
        bytes::writeFileBytes(path.string(), w.out);
    } catch (const Error&) {
        fileError_ = 7;
        return false;
    }
    fileError_ = 0;
    return true;
}

} // namespace libseq
