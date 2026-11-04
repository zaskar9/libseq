// libseq: reading a Cakewalk `.WRK` file into a sequence (SeqLoadCakewalkFile, TTSSEQ 1.01).
//
// The chunk layouts are those of the 0.x readers (archive/0.x, src/wrk.cpp); what the engine makes
// of each chunk comes from loading copies of songs with one byte changed and reading the
// parameters back (docs/reverse-engineering/ttsseq-api.md, "Sequences and parameters" and
// "Streams"). All integers are little-endian.
#include "bytes.hpp"
#include "wrk_chunks.hpp"

#include "libseq/engine.hpp"

#include <string_view>

namespace libseq {

namespace {

using bytes::Reader;

constexpr std::string_view kMagic = "CAKEWALK";

std::int32_t u32At(const std::vector<std::uint8_t>& v, const std::size_t off) {
    return static_cast<std::int32_t>(bytes::le(v[off], v[off + 1], v[off + 2], v[off + 3]));
}

/// The sequence parameters a VARS chunk (89 bytes) sets.
void applyVars(Sequence& s, const std::vector<std::uint8_t>& v) {
    s.now = static_cast<Tick>(u32At(v, 0));
    s.from = static_cast<Tick>(u32At(v, 4));
    s.thru = static_cast<Tick>(u32At(v, 8));
    s.setParam(qp::kZeroControllers, v[17]);
    s.setParam(qp::kSendSpp, v[18]);
    s.setParam(qp::kUseContinue, v[19]);
    s.setParam(qp::kSearchBack, v[20]);
    s.setParam(qp::kAutoStop, v[21]);
    s.setParam(qp::kStopTime, u32At(v, 22));
    s.setParam(qp::kAutoRewind, v[26]);
    s.setParam(qp::kRewindTime, u32At(v, 27));
    s.setParam(qp::kMetroPlay, v[31]);
    s.setParam(qp::kMetroRecord, v[32]);
    s.setParam(qp::kMetroAccent, v[33]);
    s.setParam(qp::kCountIn, v[34]);
    s.setParam(qp::kFilter, v[38]);
    s.setParam(qp::kAutoRestart, v[57]);
}

/// A MIDI event of a variable-length stream (chunk 18): only the bytes the message needs.
Event readVarMidiEvent(Reader& r, const Tick time, const std::uint8_t status) {
    Event e;
    e.time = time;
    e.status = status;
    switch (status & 0xf0U) {
    case 0x90:
        e.data1 = r.u8();
        e.data2 = r.u8();
        e.duration = r.u16();
        break;
    case 0xa0:
    case 0xb0:
    case 0xe0:
        e.data1 = r.u8();
        e.data2 = r.u8();
        break;
    case 0xc0:
    case 0xd0:
    case 0xf0:
        e.data1 = r.u8();
        break;
    default:
        throw Error("unknown event status " + std::to_string(status));
    }
    return e;
}

class Loader {
public:
    explicit Loader(Sequence& s) : s_(&s) {}

    /// The stream of a track number: streams get their ids in the order the file mentions them.
    Stream& track(const std::uint16_t number) {
        if (const auto it = byTrack_.find(number); it != byTrack_.end()) {
            return *it->second;
        }
        Stream& st = s_->createStream();
        st.setParam(sp::kMuted, 0);
        st.setParam(sp::kPosition, number);
        byTrack_.emplace(number, &st);
        return st;
    }

    void chunk(const std::uint8_t type, Reader& c) {
        switch (type) {
        case wrk::TIMEBASE:
            s_->ppq = c.u16();
            break;
        case wrk::VARS:
            if (c.size() == 89) {
                applyVars(*s_, c.bytes(89));
            }
            break;
        case wrk::TEMPO:
        case wrk::NTEMPO: {
            const int n = c.u16();
            for (int i = 0; i < n; ++i) {
                TempoEntry e;
                e.tick = c.u32();
                c.skip(4);
                const int raw = c.u16();
                e.tempo = static_cast<std::uint16_t>(type == wrk::TEMPO ? raw * 100 : raw);
                c.skip(8);
                tempo_.push_back(e);
            }
            break;
        }
        case wrk::METER: {
            const int n = c.u16();
            for (int i = 0; i < n; ++i) {
                MeterEntry m;
                c.skip(4);
                m.measure = static_cast<std::uint16_t>(c.u16() - 1); // 1-based in the file
                m.numerator = c.u8();
                m.denominatorLog2 = c.u8();
                c.skip(4);
                meter_.push_back(m);
            }
            break;
        }
        case wrk::METERKEY: {
            const int n = c.u16();
            for (int i = 0; i < n; ++i) {
                MeterEntry m;
                m.measure = static_cast<std::uint16_t>(c.u16() - 1); // 1-based in the file
                m.numerator = c.u8();
                m.denominatorLog2 = c.u8();
                m.key = c.s8();
                meterKey_.push_back(m);
            }
            break;
        }
        case wrk::TIMEFMT:
            s_->fps = c.u16();
            s_->frameOffset = c.u32();
            break;
        case wrk::METRONOME:
            s_->setParam(qp::kMetroPort, c.u16());
            s_->setParam(qp::kMetroChannel, c.u16());
            s_->setParam(qp::kMetroKey, c.u16());
            s_->setParam(qp::kMetroVelocity, c.u16());
            s_->setParam(qp::kMetroDuration, c.u16());
            break;
        case wrk::THRU:
            s_->setParam(qp::kThruMode, c.u8());
            c.skip(1);
            s_->setParam(qp::kThruPort, c.u8());
            s_->setParam(qp::kThruChannel, c.s8());
            s_->setParam(qp::kThruKeyPlus, c.s8());
            s_->setParam(qp::kThruVelPlus, c.s8());
            break;
        case wrk::MARKERS: {
            const std::uint32_t n = c.u32();
            for (std::uint32_t i = 0; i < n; ++i) {
                Marker m;
                m.smpteLocked = c.u8() != 0;
                c.skip(1);
                m.time = c.u24();
                c.skip(5);
                m.name = c.pstr();
                s_->markers.push_back(std::move(m));
            }
            break;
        }
        case wrk::TRACK: {
            Stream& t = track(c.u16());
            std::string name = c.pstr();
            name += c.pstr();
            t.setName(std::move(name));
            t.setParam(sp::kChannel, c.s8());
            t.setParam(sp::kKeyPlus, c.s8());
            t.setParam(sp::kVelPlus, c.s8());
            t.setParam(sp::kPort, c.u8());
            const std::uint8_t flags = c.u8();
            t.setParam(sp::kMuted, (flags & 2U) != 0 ? 1 : 0);
            t.setParam(sp::kLoopCount, (flags & 4U) != 0 ? 9999 : 1);
            t.setParam(sp::kArchived, (flags & 0x10U) != 0 ? 1 : 0);
            break;
        }
        case wrk::TRKNAME: {
            Stream& t = track(c.u16());
            t.setName(c.pstr());
            break;
        }
        case wrk::STREAM: {
            Stream& t = track(c.u16());
            const int n = c.u16();
            for (int i = 0; i < n; ++i) {
                t.events().push_back(Event::fromBytes(c.ptr()));
                c.skip(Event::kSize);
            }
            break;
        }
        case wrk::STRSTREAM: {
            Stream& t = track(c.u16());
            const std::uint32_t n = c.u32();
            for (std::uint32_t i = 0; i < n; ++i) {
                const Tick time = c.u24();
                const std::uint8_t status = c.u8();
                if (status >= 0x90) {
                    t.events().push_back(readVarMidiEvent(c, time, status));
                    continue;
                }
                // A string event: its text belongs to the program (see SeqRegisterEventKind).
                const std::uint32_t len = c.u32();
                c.skip(len);
                Event e;
                e.time = time;
                e.status = status;
                t.events().push_back(e);
            }
            break;
        }
        case wrk::TRKPATCH: {
            Stream& t = track(c.u16());
            t.setParam(sp::kPatch, c.u16());
            break;
        }
        case wrk::TRKREPS: {
            Stream& t = track(c.u16());
            t.setParam(sp::kLoopCount, c.u16());
            break;
        }
        case wrk::TRKOFFS: {
            Stream& t = track(c.u16());
            t.setParam(sp::kTimePlus, c.s16());
            break;
        }
        case wrk::NTRKOFS: {
            Stream& t = track(c.u16());
            t.setParam(sp::kTimePlus, static_cast<std::int32_t>(c.u32()));
            break;
        }
        case wrk::TRKVOL: {
            Stream& t = track(c.u16());
            t.setParam(sp::kVolume, c.s16());
            t.setParam(sp::kPan, c.s16());
            break;
        }
        case wrk::SYSEX: {
            SysxBank b;
            b.number = c.u8();
            const std::size_t n = c.u16();
            b.autoSend = c.u8() != 0;
            b.name = c.pstr();
            b.data = c.bytes(n);
            s_->sysxBanks.push_back(std::move(b));
            break;
        }
        default:
            break; // the program's chunks and those 1.01 does not read
        }
    }

    /// The maps, once all chunks are read: their cached times are computed from the time base.
    void finish() {
        if (!tempo_.empty()) {
            s_->tempoMap.assign(std::move(tempo_));
        }
        // a file with both meter chunks: the one with the keys counts
        if (!meterKey_.empty()) {
            s_->meterMap.assign(std::move(meterKey_), s_->ppq);
        } else if (!meter_.empty()) {
            s_->meterMap.assign(std::move(meter_), s_->ppq);
        }
    }

private:
    Sequence* s_;
    std::map<std::uint16_t, Stream*> byTrack_;
    std::vector<TempoEntry> tempo_;
    std::vector<MeterEntry> meter_;
    std::vector<MeterEntry> meterKey_;
};

} // namespace

SeqHandle Engine::loadCakewalkFile(const std::filesystem::path& path) {
    std::vector<std::uint8_t> data;
    try {
        data = bytes::readFileBytes(path.string());
    } catch (const Error&) {
        fileError_ = 1;
        return 0;
    }
    Reader r(data);
    if (r.remaining() < kMagic.size() + 3 || r.str(kMagic.size()) != kMagic) {
        fileError_ = 2;
        return 0;
    }
    r.skip(3); // 0x1A, minor and major version
    const SeqHandle h = createSequence();
    Sequence& s = *sequence(h);
    Loader loader(s);
    try {
        while (!r.atEnd()) {
            const std::uint8_t type = r.u8();
            if (type == wrk::END) {
                break;
            }
            const std::uint32_t len = r.u32();
            if (len > r.remaining()) {
                throw Error("chunk runs past the end of the file");
            }
            Reader c(r.ptr(), len);
            r.skip(len);
            loader.chunk(type, c);
        }
        loader.finish();
    } catch (const Error&) {
        destroySequence(h);
        fileError_ = 3;
        return 0;
    }
    s.setParam(qp::kNowJumped, 1);
    s.setParam(qp::kMetroSpeaker, 0); // a loaded sequence clicks on MIDI only
    fileError_ = 0;
    return h;
}

} // namespace libseq
