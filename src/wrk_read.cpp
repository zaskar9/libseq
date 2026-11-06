// libseq: reading a Cakewalk `.WRK` file into a sequence (SeqLoadCakewalkFile, TTSSEQ 1.01).
//
// The chunk layouts are those of the 0.x readers (archive/0.x, src/wrk.cpp); what the engine makes
// of each chunk comes from loading copies of songs with one byte changed and reading the
// parameters back (docs/reverse-engineering/ttsseq-api.md, "Sequences and parameters" and
// "Streams"). All integers are little-endian.
#include "bytes.hpp"
#include "wrk_chunks.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <optional>
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
    // bytes 4 to 11 are the program's From and Thru; 1.01 does not keep them and writes 0
    s.now = static_cast<Tick>(u32At(v, 0));
    s.clockSource = v[13];
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
    s.tempoRatio = v[58];
    for (std::size_t i = 0; i < 3; ++i) {
        s.tempoRatios[i] = v[59 + i];
    }
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
        case wrk::METRONOME: {
            std::array<std::int32_t, 5> m{};
            for (std::int32_t& v : m) {
                v = c.u16();
            }
            metronome = m;
            break;
        }
        case wrk::THRU:
            s_->setParam(qp::kThruMode, c.u8());
            c.skip(1);
            s_->setParam(qp::kThruPort, c.u8());
            s_->setParam(qp::kThruChannel, c.s8());
            s_->setParam(qp::kThruKeyPlus, c.s8());
            s_->setParam(qp::kThruVelPlus, c.s8());
            s_->setParam(qp::kLocalOnPort, c.s16());
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
                // A string event: its data belongs to the program (SeqRegisterEventKind). The
                // event procedure of its kind reads it (message 4); with no such kind registered
                // the engine leaves it out.
                const std::uint8_t kind = kindOfFileId(status);
                if (kind == 0) {
                    c.skip(c.u32());
                    continue;
                }
                Event e;
                e.time = time;
                e.status = kind;
                t.events().push_back(e);
                if (!stringEvent(t.events().back(), c)) {
                    throw Error("an event procedure did not read its event");
                }
            }
            break;
        }
        case wrk::STRTAB: {
            // each entry goes to the first registered kind of its name that has none yet
            const std::uint16_t n = c.u16();
            for (std::uint16_t i = 0; i < n; ++i) {
                const std::string name = c.pstr();
                const std::uint8_t id = c.u8();
                for (Kind& k : kinds) {
                    if (k.fileId == 0 && k.name == name) {
                        k.fileId = id;
                        break;
                    }
                }
            }
            break;
        }
        case wrk::TRKPATCH: {
            Stream& t = track(c.u16());
            t.setParam(sp::kPatch, c.u16());
            paramSet(t, sp::kPatch);
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
            paramSet(t, sp::kVolume);
            t.setParam(sp::kPan, c.s16());
            paramSet(t, sp::kPan);
            break;
        }
        case wrk::SYSEX: {
            SysxBank b;
            b.number = static_cast<std::int16_t>(c.u8());
            const std::size_t n = c.u16();
            b.autoSend = c.u8() != 0;
            b.name = c.pstr();
            b.data = c.bytes(n);
            // banks get their ids in file order
            s_->sysxBanks.emplace(static_cast<std::uint8_t>(s_->sysxBanks.size()), std::move(b));
            break;
        }
        default:
            break; // the program's chunks and those 1.01 does not read
        }
    }

    /// The maps, once all chunks are read: their cached times are computed from the time base;
    /// then the locked markers get their frames.
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
        // a locked marker keeps the frame its tick has with the file's tempo map and frame rate
        for (Marker& m : s_->markers) {
            if (m.smpteLocked) {
                m.frame = s_->tickToFrame(m.time);
            }
        }
    }

    /// The registered kinds of string events, by number, and the id each has in the file's string
    /// table (chunk 22), 0 for none.
    struct Kind {
        std::uint8_t number = 0;
        std::string name;
        std::uint8_t fileId = 0;
    };
    std::vector<Kind> kinds;
    /// Called when a chunk sets a stream's patch, volume or pan (Engine::sendStreamParam).
    std::function<void(const Stream&, int)> paramSet = [](const Stream&, int) {};
    /// The kind of a string event's status in the file: the first registered kind with that id.
    /// So an id of 0 goes to the first kind the table does not name (seen with SYN_KIND, whose
    /// text a procedure left unread).
    [[nodiscard]] std::uint8_t kindOfFileId(const std::uint8_t id) const {
        for (const Kind& k : kinds) {
            if (k.fileId == id) {
                return k.number;
            }
        }
        return 0;
    }
    /// Lets the event procedure read a string event's data from the chunk; false when it fails.
    std::function<bool(Event&, Reader&)> stringEvent;

    /// The metronome note (parameters 17 to 21), set on the engine only when the sequence becomes
    /// the current one (Engine::current_).
    std::optional<std::array<std::int32_t, 5>> metronome;

private:
    Sequence* s_;
    std::map<std::uint16_t, Stream*> byTrack_;
    std::vector<TempoEntry> tempo_;
    std::vector<MeterEntry> meter_;
    std::vector<MeterEntry> meterKey_;
};

} // namespace

SeqHandle Engine::loadCakewalkFile(const std::filesystem::path& path, const FileProc& callback) {
    std::vector<std::uint8_t> data;
    try {
        data = bytes::readFileBytes(path.string());
    } catch (const Error&) {
        fileError_ = 7; // what 1.01 reports for a file that is not there
        return 0;
    }
    constexpr std::size_t kHeader = kMagic.size() + 3; // the magic, 0x1A, minor and major version
    if (data.size() < kHeader || !std::equal(kMagic.begin(), kMagic.end(), data.begin())) {
        fileError_ = 2;
        return 0;
    }
    io_ = OpenFile{std::move(data), kHeader, true};
    const SeqHandle h = createSequence();
    Sequence& s = *sequence(h);
    Loader loader(s);
    for (const auto& [id, k] : kinds_) {
        loader.kinds.push_back({id, k.name, 0});
    }
    loader.paramSet = [this](const Stream& st, const int param) { sendStreamParam(st, param); };
    std::size_t base = 0; // where the chunk being read starts in the file
    loader.stringEvent = [this, &base](Event& e, Reader& c) {
        io_.pos = base + c.pos();
        const std::uint32_t r = callEventProc(ek::kLoad, e, 7);
        c.seek(io_.pos >= base ? io_.pos - base : c.size());
        return r != 0;
    };
    const auto ask = [&callback](const std::uint16_t code, const std::uint16_t word) {
        return callback ? callback(code, word) : 0U;
    };
    try {
        ask(fileproc::kStart, fileproc::kFile);
        while (io_.pos < io_.data.size()) {
            const std::uint8_t type = io_.data[io_.pos++];
            if (type == wrk::END) {
                break;
            }
            // the program may take any chunk but VARS, which the engine reads and then reports
            if (type != wrk::VARS && ask(type, fileproc::kFile) != 0) {
                continue;
            }
            if (io_.data.size() - io_.pos < 4) {
                throw Error("chunk length past the end of the file");
            }
            Reader l(io_.data.data() + io_.pos, 4);
            const std::uint32_t len = l.u32();
            io_.pos += 4;
            if (len > io_.data.size() - io_.pos) {
                throw Error("chunk runs past the end of the file");
            }
            base = io_.pos;
            Reader c(io_.data.data() + base, len);
            loader.chunk(type, c);
            io_.pos = base + len;
            if (type == wrk::VARS) {
                ask(fileproc::kVars, h);
            }
        }
        loader.finish();
        if (loader.metronome && current_ == h) {
            for (int p = qp::kMetroPort; p <= qp::kMetroDuration; ++p) {
                s.setParam(p, (*loader.metronome)[static_cast<std::size_t>(p - qp::kMetroPort)]);
            }
        }
        ask(fileproc::kEnd, fileproc::kFile);
    } catch (const Error&) {
        io_ = OpenFile{};
        destroySequence(h);
        fileError_ = 3;
        return 0;
    }
    io_ = OpenFile{};
    s.setParam(qp::kNowJumped, 1);
    s.setParam(qp::kMetroSpeaker, 0); // a loaded sequence clicks on MIDI only
    setSrcClock(s.clockSource);       // the engine's one clock source, through SeqSetSrcClock's check
    fileError_ = 0;
    return h;
}

} // namespace libseq
