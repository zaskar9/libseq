// libseq: reading a Standard MIDI File into a new sequence (SeqLoadMidiFile, TTSSEQ 1.01).
//
// What 1.01 does was found by reading back every MIDI file it wrote of the corpus and some probes
// (tests/data/ttsseq/101, the *_M.WRK files of the files suite) and the midifiles suite:
//  - the sequence starts as SeqCreateSeq makes it, with the division as its ppq;
//  - tempos (meta 51) become tempo entries of whole beats per minute, inserted as SeqInsTempo
//    does, so the cached times keep its slip; meters (58) and key signatures (59) become meter
//    entries at the measure their tick falls in; markers (06) and cue points (07, as locked
//    markers whose frame is taken with the tempo map read so far) are inserted as SeqInsMarker does;
//  - every track but the first of a format 1 file becomes a stream, numbered from 0, with the
//    track's name (03) and port (21), not muted, and the channel of its messages as the forced
//    channel when they are all on one;
//  - the track's first program change and first volume and pan controllers become the stream's
//    patch, volume and pan, wherever they are, and leave the stream; one that comes after a
//    message the stream keeps goes out at once (Engine::sendStreamParam);
//  - a note off (or note on with velocity 0) ends the last note started of its key on its
//    channel; other channel messages are kept, sysex and other meta events are not;
//  - every message is read into the scratch event the writer uses too (Engine::midiScratch_): a
//    message with two data bytes sets the second one and a duration of 1 for notes, 0 for the
//    others; a program change or channel pressure leaves both as the message before it left them
//    (E2_CHAS: a program change after a note off gets 0 and a duration of 1).
#include "bytes.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <functional>

namespace libseq {

namespace {

using bytes::Reader;

struct Track {
    const std::uint8_t* data;
    std::size_t size;
};

class Loader {
public:
    Loader(Sequence& s, Event& scratch) : s_(&s), scratch_(&scratch) {}

    void track(const Track& tr, const bool conductor) {
        Reader r(tr.data, tr.size);
        Stream* st = conductor ? nullptr : &s_->createStream();
        if (st != nullptr) {
            st->setParam(sp::kPosition, position_++);
            st->setParam(sp::kMuted, 0);
        }
        bool named = false;
        bool haveChannel = false;
        int trackChannel = -1;
        bool havePatch = false;
        bool haveVolume = false;
        bool havePan = false;
        std::vector<std::size_t> open; // notes not ended yet, in the order they started
        Tick now = 0;
        std::uint8_t running = 0;
        while (!r.atEnd()) {
            now += r.vlq();
            std::uint8_t status = r.u8();
            if (status == 0xff) {
                const std::uint8_t type = r.u8();
                const std::uint32_t len = r.vlq();
                const std::vector<std::uint8_t> data = r.bytes(len);
                if (type == 0x2f) {
                    break;
                }
                if (type == 0x03 && st != nullptr && !named) {
                    st->setName(std::string(data.begin(), data.end()));
                    named = true;
                } else if (type == 0x21 && st != nullptr && !data.empty()) {
                    st->setParam(sp::kPort, data[0]);
                } else {
                    meta(now, type, data);
                }
                continue;
            }
            if (status == 0xf0 || status == 0xf7) {
                r.skip(r.vlq());
                continue;
            }
            std::uint8_t d1 = 0;
            if ((status & 0x80U) != 0) {
                running = status;
                d1 = r.u8();
            } else {
                d1 = status;
                status = running;
                if (status == 0) {
                    throw Error("data byte without a status");
                }
            }
            const unsigned kind = status & 0xf0U;
            scratch_->time = now;
            scratch_->status = status;
            scratch_->data1 = d1;
            if (kind != 0xc0U && kind != 0xd0U) {
                scratch_->data2 = r.u8();
                scratch_->duration = kind == 0x80U || kind == 0x90U ? 1 : 0;
            }
            if (st == nullptr) {
                continue;
            }
            // the channel all the track's messages are on, -1 when they are on several; it
            // becomes the stream's when the track is read
            const int channel = status & 0x0f;
            if (!haveChannel) {
                trackChannel = channel;
                haveChannel = true;
            } else if (trackChannel != channel) {
                trackChannel = -1;
            }
            std::vector<Event>& events = st->events();
            if (kind == 0x80U || (kind == 0x90U && scratch_->data2 == 0)) {
                // the last note started of that key ends first
                const auto it = std::find_if(open.rbegin(), open.rend(), [&](const std::size_t i) {
                    return (events[i].status & 0x0fU) == (status & 0x0fU) && events[i].data1 == d1;
                });
                if (it != open.rend()) {
                    events[*it].duration = static_cast<std::uint16_t>(now - events[*it].time);
                    open.erase(std::next(it).base());
                }
                continue;
            }
            if (kind == 0xc0U && !havePatch) {
                st->setParam(sp::kPatch, d1);
                paramSet(*st, sp::kPatch);
                havePatch = true;
                continue;
            }
            if (kind == 0xb0U && d1 == 7 && !haveVolume) {
                st->setParam(sp::kVolume, scratch_->data2);
                paramSet(*st, sp::kVolume);
                haveVolume = true;
                continue;
            }
            if (kind == 0xb0U && d1 == 10 && !havePan) {
                st->setParam(sp::kPan, scratch_->data2);
                paramSet(*st, sp::kPan);
                havePan = true;
                continue;
            }
            if (kind == 0x90U) {
                open.push_back(events.size());
            }
            events.push_back(*scratch_);
        }
        if (st != nullptr) {
            st->setParam(sp::kChannel, trackChannel);
        }
    }

    /// Called when a track sets a stream's patch, volume or pan (Engine::sendStreamParam): it goes
    /// out when the stream already has a message to take the channel from.
    std::function<void(const Stream&, int)> paramSet = [](const Stream&, int) {};

private:
    void meta(const Tick now, const std::uint8_t type, const std::vector<std::uint8_t>& d) {
        switch (type) {
        case 0x51:
            if (d.size() >= 3) {
                const std::uint32_t upq = (std::uint32_t{d[0]} << 16) | (std::uint32_t{d[1]} << 8) | d[2];
                if (upq != 0) {
                    // whole beats per minute: 603015 us gives 99.00, not 99.50
                    s_->tempoMap.insert(now, static_cast<std::uint16_t>((60'000'000U / upq) * 100U));
                }
            }
            break;
        case 0x58:
            if (d.size() >= 2) {
                const MeterEntry& e = s_->meterMap.entries()[s_->meterMap.indexAtTick(now)];
                const auto meter = static_cast<std::uint16_t>((d[1] << 8) | d[0]);
                s_->meterMap.insert(s_->meterMap.toMBT(now, s_->ppq).measure, e.key, meter, s_->ppq);
            }
            break;
        case 0x59:
            if (!d.empty()) {
                const MeterEntry& e = s_->meterMap.entries()[s_->meterMap.indexAtTick(now)];
                s_->meterMap.insert(s_->meterMap.toMBT(now, s_->ppq).measure, static_cast<std::int8_t>(d[0]), e.meter(),
                                    s_->ppq);
            }
            break;
        case 0x06:
        case 0x07: {
            Marker m;
            m.name = std::string(d.begin(), d.end());
            s_->setMarkerTime(m, now, type == 0x07);
            s_->insertMarker(std::move(m));
            break;
        }
        default:
            break;
        }
    }

    Sequence* s_;
    Event* scratch_;
    int position_ = 0;
};

} // namespace

SeqHandle Engine::loadMidiFile(const std::filesystem::path& path) {
    restartTimer(); // 1.01 starts its timer again around every load
    std::vector<std::uint8_t> data;
    try {
        data = bytes::readFileBytes(path.string());
    } catch (const Error&) {
        fileError_ = 7; // what 1.01 reports for a file that is not there
        return 0;
    }
    std::uint16_t format = 0;
    std::uint16_t division = 0;
    std::vector<Track> tracks;
    try {
        Reader r(data);
        if (r.str(4) != "MThd") {
            throw Error("not a MIDI file");
        }
        const std::uint32_t len = r.be32();
        if (len < 6) {
            throw Error("short header");
        }
        format = r.be16();
        r.be16(); // the number of tracks: the chunks are counted instead
        division = r.be16();
        r.skip(len - 6);
        while (r.remaining() >= 8) {
            const std::string tag = r.str(4);
            const std::uint32_t n = r.be32();
            if (n > r.remaining()) {
                throw Error("track runs past the end of the file");
            }
            if (tag == "MTrk") {
                tracks.push_back({r.ptr(), n});
            }
            r.skip(n);
        }
    } catch (const Error&) {
        fileError_ = 2;
        return 0;
    }
    if (division == 0 || (division & 0x8000U) != 0) {
        fileError_ = 2;
        return 0;
    }
    const SeqHandle h = createSequence();
    Sequence& s = *sequence(h);
    s.ppq = division;
    Loader loader(s, midiScratch_);
    loader.paramSet = [this](const Stream& st, const int param) { sendStreamParam(st, param); };
    try {
        for (std::size_t i = 0; i < tracks.size(); ++i) {
            loader.track(tracks[i], format == 1 && i == 0);
        }
    } catch (const Error&) {
        destroySequence(h);
        fileError_ = 3;
        return 0;
    }
    fileError_ = 0;
    return h;
}

} // namespace libseq
