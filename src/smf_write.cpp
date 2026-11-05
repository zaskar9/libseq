// libseq: writing a sequence as a Standard MIDI File (SeqSaveMidiFile, TTSSEQ 1.01).
//
// Format 1 with the ppq as the division. Track 0 holds the meters, key signatures, tempos and
// markers; each stream follows as a track of its own, in id order, muted and archived ones too.
// What 1.01 writes was found by saving every song of the corpus and some probes
// (tests/data/ttsseq/101, the files and midifiles suites):
//  - a stream's track starts with its port (meta 21) and its name (meta 03, left out when empty),
//    then its patch, volume and pan parameters as channel messages with a status byte each, on
//    the forced channel or else the channel of the stream's first channel message;
//  - the forced channel, key+ and vel+ are applied; time+ and the loop count are not; the velocity
//    stays within 1 to 127 (for the key, see transposed());
//  - only channel messages are written, and string events as their procedures write them;
//    sysex bank references are left out;
//  - a note is a note on and a note on with velocity 0 at its end, written before the events at or
//    after that time; of notes that end together, the last one started ends first;
//  - running status is used from the first event on, the header's messages do not take part;
//  - each event goes through the scratch event the reader uses too (Engine::midiScratch_).
#include "bytes.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <functional>
#include <vector>

namespace libseq {

namespace {

using bytes::Writer;

std::uint8_t byte(const std::uint32_t v) {
    return static_cast<std::uint8_t>(v & 0xffU);
}

std::uint8_t byte(const std::int32_t v) {
    return static_cast<std::uint8_t>(v & 0xff);
}

/// One track, written to the end of the file: its chunk header, events with delta times, and the
/// length filled in at the end.
class Track {
public:
    explicit Track(std::vector<std::uint8_t>& file) : file_(&file), start_(file.size() + 8) {
        Writer w;
        w.str("MTrk");
        w.be32(0);
        put(w);
    }
    void at(const Tick t) {
        Writer w;
        w.vlq(t - now_);
        put(w);
        now_ = t;
    }
    void meta(const Tick t, const std::uint8_t type, const std::vector<std::uint8_t>& data) {
        at(t);
        Writer w;
        w.u8(0xff);
        w.u8(type);
        w.vlq(static_cast<std::uint32_t>(data.size()));
        w.bytes(data);
        put(w);
        running_ = 0;
    }
    void text(const Tick t, const std::uint8_t type, const std::string& s) {
        meta(t, type, std::vector<std::uint8_t>(s.begin(), s.end()));
    }
    /// A channel message; the status byte is left out when it repeats and `running` is set.
    void channel(const Tick t, const std::uint8_t status, const std::uint8_t d1, const std::uint8_t d2,
                 const bool running = true) {
        at(t);
        Writer w;
        if (!running || status != running_) {
            w.u8(status);
        }
        running_ = running ? status : 0;
        w.u8(d1);
        const unsigned kind = status & 0xf0U;
        if (kind != 0xc0U && kind != 0xd0U) {
            w.u8(d2);
        }
        put(w);
    }
    /// Ends the track and fills in its length.
    void end() {
        meta(now_, 0x2f, {});
        const auto len = static_cast<std::uint32_t>(file_->size() - start_);
        for (std::size_t i = 0; i < 4; ++i) {
            (*file_)[start_ - 4 + i] = static_cast<std::uint8_t>((len >> (8 * (3 - i))) & 0xffU);
        }
    }

private:
    void put(const Writer& w) { file_->insert(file_->end(), w.out.begin(), w.out.end()); }
    std::vector<std::uint8_t>* file_;
    std::size_t start_ = 0;
    Tick now_ = 0;
    std::uint8_t running_ = 0;
};

/// Track 0: at each tick the meter and the key signature when they changed, the tempo and the
/// markers.
void mapTrack(const Sequence& s, std::vector<std::uint8_t>& file) {
    struct Item {
        Tick tick;
        int order;
        std::uint8_t type;
        std::vector<std::uint8_t> data;
    };
    std::vector<Item> items;
    int lastMeter = -1;
    int lastKey = 0x100;
    for (const MeterEntry& m : s.meterMap.entries()) {
        if (m.meter() != lastMeter) {
            const auto clocks = static_cast<std::uint8_t>(96U >> m.denominatorLog2);
            items.push_back({m.tick, 0, 0x58, {m.numerator, m.denominatorLog2, clocks, 8}});
            lastMeter = m.meter();
        }
        if (m.key != lastKey) {
            items.push_back({m.tick, 1, 0x59, {static_cast<std::uint8_t>(m.key), 0}});
            lastKey = m.key;
        }
    }
    for (const TempoEntry& e : s.tempoMap.entries()) {
        // microseconds per quarter, cut (not rounded) as 1.01 does: 99.50 gives 603015
        const auto upq = static_cast<std::uint32_t>(6'000'000'000ULL / e.tempo);
        items.push_back({e.tick, 2, 0x51, {byte(upq >> 16), byte(upq >> 8), byte(upq)}});
    }
    for (const Marker& m : s.markers) {
        items.push_back({s.markerTime(m), 3, m.smpteLocked ? std::uint8_t{0x07} : std::uint8_t{0x06},
                         std::vector<std::uint8_t>(m.name.begin(), m.name.end())});
    }
    std::stable_sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.tick != b.tick ? a.tick < b.tick : a.order < b.order;
    });
    Track t(file);
    for (const Item& i : items) {
        t.meta(i.tick, i.type, i.data);
    }
    t.end();
}

/// A note's key with key+: above 127 it goes to the top octave (108 to 119) keeping its pitch
/// class. Below 0, 1.01 takes the second data byte of the stream's next event modulo 12, whatever
/// the event is (0 after the last event): ENG_PARM's 5 - 12 with a next velocity of 31 gives 7.
std::uint8_t transposed(const std::vector<Event>& events, const std::size_t i, const int keyPlus) {
    const int k = events[i].data1 + keyPlus;
    if (k > 127) {
        return static_cast<std::uint8_t>(108 + k % 12);
    }
    if (k < 0) {
        return i + 1 < events.size() ? static_cast<std::uint8_t>(events[i + 1].data2 % 12) : std::uint8_t{0};
    }
    return static_cast<std::uint8_t>(k);
}

/// A string event's procedure with the file standing at its end.
using Proc = std::function<std::uint32_t(std::uint16_t message, Event& event, std::uint16_t length)>;

void streamTrack(const Stream& st, Event& scratch, std::vector<std::uint8_t>& file, const Proc& proc) {
    Track t(file);
    t.meta(0, 0x21, {byte(st.param(sp::kPort))});
    if (!st.name().empty()) {
        t.text(0, 0x03, st.name());
    }

    const std::vector<Event>& events = st.events();
    const int forced = st.param(sp::kChannel);
    std::uint8_t channel = 0;
    if (forced >= 0) {
        channel = byte(forced) & 0x0fU;
    } else {
        const auto first = std::find_if(events.begin(), events.end(),
                                        [](const Event& e) { return e.status >= 0x80 && e.status < 0xf0; });
        if (first != events.end()) {
            channel = first->status & 0x0fU;
        }
    }
    if (st.param(sp::kPatch) >= 0) {
        t.channel(0, 0xc0U | channel, byte(st.param(sp::kPatch)), 0, false);
    }
    if (st.param(sp::kVolume) >= 0) {
        t.channel(0, 0xb0U | channel, 7, byte(st.param(sp::kVolume)), false);
    }
    if (st.param(sp::kPan) >= 0) {
        t.channel(0, 0xb0U | channel, 10, byte(st.param(sp::kPan)), false);
    }

    struct Off {
        Tick time;
        std::uint8_t status;
        std::uint8_t key;
    };
    std::vector<Off> pending; // the last one started is last
    const auto flush = [&](const Tick upTo, const bool all) {
        for (;;) {
            auto next = pending.end();
            for (auto it = pending.begin(); it != pending.end(); ++it) {
                if ((all || it->time <= upTo) && (next == pending.end() || it->time <= next->time)) {
                    next = it;
                }
            }
            if (next == pending.end()) {
                return;
            }
            t.channel(next->time, next->status, next->key, 0);
            pending.erase(next);
        }
    };

    const int keyPlus = st.param(sp::kKeyPlus);
    const int velPlus = st.param(sp::kVelPlus);
    for (std::size_t i = 0; i < events.size(); ++i) {
        const Event& e = events[i];
        if (e.status >= 0xf0) {
            continue;
        }
        flush(e.time, false);
        if (e.status < 0x80) {
            // a string event: its procedure says whether it writes an event (message 7, length
            // 0), and then writes it after the delta time (length 7). Running status goes on.
            Event copy = e;
            if (proc(ek::kMidi, copy, 0) != 0) {
                t.at(e.time);
                proc(ek::kMidi, copy, 7);
            }
            continue;
        }
        const auto status = static_cast<std::uint8_t>(forced >= 0 ? (e.status & 0xf0U) | channel : e.status);
        scratch = e;
        scratch.status = status;
        if (e.isNote()) {
            scratch.data1 = transposed(events, i, keyPlus);
            scratch.data2 = static_cast<std::uint8_t>(std::clamp(e.data2 + velPlus, 1, 127));
            pending.push_back({e.time + e.duration, status, scratch.data1});
        }
        t.channel(e.time, status, scratch.data1, scratch.data2);
    }
    flush(0, true);
    t.end();
}

} // namespace

bool Engine::saveMidiFile(const SeqHandle seq, const std::filesystem::path& path) {
    const Sequence* s = sequence(seq);
    if (s == nullptr) {
        return false;
    }
    const std::vector<Stream*> streams = s->streams();
    io_ = OpenFile{{}, 0, true};
    Writer w;
    w.str("MThd");
    w.be32(6);
    w.be16(1);
    w.be16(static_cast<std::uint32_t>(streams.size() + 1));
    w.be16(s->ppq);
    io_.data = std::move(w.out);
    mapTrack(*s, io_.data);
    const Proc proc = [this](const std::uint16_t message, Event& e, const std::uint16_t length) {
        io_.pos = io_.data.size();
        return callEventProc(message, e, length);
    };
    for (const Stream* st : streams) {
        streamTrack(*st, midiScratch_, io_.data, proc);
    }
    w.out = std::move(io_.data);
    io_ = OpenFile{};
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
