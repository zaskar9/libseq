// libseq: real-time recording (SeqRecordStart, SeqRecordStop, SeqRecordCleanup, SeqRecCount,
// SeqRecordEvent; TTSSEQ 1.01).
//
// Found with the probe and scripted input (suites record and punch; ttsseq-api.md, Recording):
//  - the start plays the sequence from Now (rounded down to a MIDI beat while Song Position is
//    on) after the count-in, and plays on past the end; parameter 3 is set;
//  - messages are taken at the timer's next run (virtual time, ttsseq-api.md, Timing): during
//    the count-in they are recorded at the start tick, later ones at the take's position at that
//    run (the clock from -1 after the count-in, plus the start); a note's duration is its note off's tick minus its
//    own, a note held when the recording stops ends at the take's last event (the engine sends its note off);
//  - the record filter (28) applies, bit (status >> 4) - 8; sysex is not recorded; a message with
//    one data byte keeps 0 as the second;
//  - the take is a stream of the sequence from the start on (a take kept with nothing to merge
//    into keeps that handle); a punch that cuts events first makes a stream for them, so the
//    take's handle is the one after;
//  - SeqRecordStop answers 0 when nothing was recorded;
//  - with 256 streams in the sequence there is none for the take: SeqRecordStart answers 0 after
//    sending the Song Position (suite countins);
//  - SeqRecordCleanup: see Engine::recordCleanup.
//
// Step recording (suite steprec): SeqStepRecSetup makes the stream that collects the take. A note
// goes in at the step position with the step's duration and velocity (0: the one played); a
// step is complete when its last key is up, and the program hears of it with the number of
// events so far. Other channel messages go in at the step position with duration 0 and make no
// step. The record filter applies. SeqRecordEvent does nothing then (it answers 0).
#include "playback.hpp"

#include "libseq/engine.hpp"

#include <algorithm>

namespace libseq {

bool Engine::recordStart(const StreamHandle target, const Tick in, const Tick out, const StartProc& callback) {
    Sequence* s = sequence(current_);
    if (s == nullptr || player_->playing()) {
        return false;
    }
    Stream* punch = target != 0 ? stream(target) : nullptr;
    if (target != 0 && punch == nullptr) {
        return false; // 1.01 crashes
    }
    if (s->streams().size() >= 256) {
        // no stream left for the take (256 takes without a cleanup): only the Song Position goes out
        player_->songPositionOnly(*s);
        return false;
    }
    take_ = Take{};
    recCount_ = 0;
    take_.seq = current_;
    take_.in = in;
    take_.out = out == 0 ? 1 : out;
    std::vector<Event> cut;
    if (punch != nullptr) {
        std::vector<Event>& events = punch->events();
        const auto inRange = [&](const Event& e) { return e.time >= take_.in && e.time < take_.out; };
        std::ranges::copy_if(events, std::back_inserter(cut), inRange);
        std::erase_if(events, inRange);
    }
    player_->start(*s, true);
    if (!cut.empty()) {
        Stream& c = s->createStream();
        c.events() = std::move(cut);
        take_.cut = c.handle();
    }
    take_.stream = s->createStream().handle();
    take_.recording = true;
    while (platform_ != nullptr && player_->countingIn()) {
        if (callback && callback(startproc::kCountIn, player_->countInLeft(), 0) != 0) {
            recordStop();
            StreamHandle back = target; // the cut events go back
            recordCleanup(take_.seq, false, back);
            return false;
        }
        platform_->wait();
    }
    return true;
}

void Engine::record(const std::span<const std::uint8_t> message) {
    if (step_.on) {
        stepRecord(message);
        return;
    }
    if (!take_.recording || !player_->recording() || message.size() < 2) {
        return;
    }
    Sequence* s = sequence(take_.seq);
    Stream* st = s != nullptr ? s->stream(take_.stream) : nullptr;
    if (st == nullptr) {
        return;
    }
    const unsigned kind = message[0] & 0xf0U;
    const unsigned bit = (kind >> 4U) - 8U;
    if ((static_cast<unsigned>(s->param(qp::kFilter)) & (1U << bit)) == 0) {
        return;
    }
    const Tick now = player_->recordTick();
    std::vector<Event>& events = st->events();
    const std::uint8_t d2 = message.size() > 2 ? message[2] : 0;
    if (kind == 0x80U || (kind == 0x90U && d2 == 0)) {
        // the last note started of that key and channel ends
        const auto it = std::find_if(take_.open.rbegin(), take_.open.rend(), [&](const std::size_t i) {
            return events[i].status == (0x90U | (message[0] & 0x0fU)) && events[i].data1 == message[1];
        });
        if (it != take_.open.rend()) {
            events[*it].duration = static_cast<std::uint16_t>(now - events[*it].time);
            take_.open.erase(std::next(it).base());
        }
        return;
    }
    Event e;
    e.time = now;
    e.status = message[0];
    e.data1 = message[1];
    e.data2 = d2;
    if (kind == 0x90U) {
        take_.open.push_back(events.size());
    }
    events.push_back(e);
    ++recCount_;
}

std::uint32_t Engine::setRecCount(const std::uint32_t count) {
    Sequence* s = sequence(step_.on ? step_.seq : take_.seq);
    Stream* st = s != nullptr ? s->stream(step_.on ? step_.stream : take_.stream) : nullptr;
    if (st != nullptr && count < st->events().size()) {
        st->events().resize(count);
        std::erase_if(take_.open, [count](const std::size_t i) { return i >= count; });
    }
    recCount_ = st != nullptr ? static_cast<std::uint32_t>(st->events().size()) : std::min(recCount_, count);
    return recCount_;
}

// ---- step recording

StreamHandle Engine::stepRecSetup(const SeqHandle seq, StepNotify notify) {
    Sequence* s = sequence(seq);
    if (s == nullptr) {
        return 0;
    }
    step_.on = true;
    step_.seq = seq;
    step_.stream = s->createStream().handle();
    step_.notify = std::move(notify);
    step_.held.clear();
    recCount_ = 0;
    return step_.stream;
}

void Engine::stepRecParams(const Tick position, const std::uint16_t duration, const std::uint16_t velocity) {
    step_.position = position;
    step_.duration = duration;
    step_.velocity = velocity;
}

std::uint32_t Engine::stepRecTerm() {
    step_.on = false;
    step_.held.clear();
    step_.notify = {};
    return recCount_;
}

void Engine::stepRecord(const std::span<const std::uint8_t> message) {
    Sequence* s = sequence(step_.seq);
    Stream* st = s != nullptr ? s->stream(step_.stream) : nullptr;
    if (st == nullptr || message.size() < 2) {
        return;
    }
    const unsigned kind = message[0] & 0xf0U;
    const unsigned bit = (kind >> 4U) - 8U;
    if ((static_cast<unsigned>(s->param(qp::kFilter)) & (1U << bit)) == 0) {
        return;
    }
    const std::uint8_t d2 = message.size() > 2 ? message[2] : 0;
    const auto note = std::make_pair(static_cast<unsigned>(message[0] & 0x0fU), message[1]);
    if (kind == 0x80U || (kind == 0x90U && d2 == 0)) {
        // the step is complete when its last key is up
        if (step_.held.erase(note) > 0 && step_.held.empty()) {
            if (step_.notify) {
                step_.notify(recCount_);
            }
        }
        return;
    }
    Event e;
    e.time = step_.position;
    e.status = message[0];
    e.data1 = message[1];
    e.data2 = d2;
    if (kind == 0x90U) {
        step_.held.insert(note);
        e.duration = step_.duration;
        if (step_.velocity != 0) {
            e.data2 = static_cast<std::uint8_t>(step_.velocity & 0x7fU);
        }
    }
    std::vector<Event>& events = st->events();
    events.insert(std::upper_bound(events.begin(), events.end(), e,
                                   [](const Event& a, const Event& b) { return a.time < b.time; }),
                  e);
    ++recCount_;
}

bool Engine::recordEvent(const Event& event) {
    Sequence* s = sequence(take_.seq);
    Stream* st = s != nullptr ? s->stream(take_.stream) : nullptr;
    if (!take_.recording || st == nullptr) {
        return false;
    }
    Event e = event;
    e.time = s->now;
    st->events().push_back(e);
    ++recCount_;
    return true;
}

bool Engine::recordStop() {
    if (!take_.recording) {
        return false;
    }
    Sequence* s = sequence(take_.seq);
    Stream* st = s != nullptr ? s->stream(take_.stream) : nullptr;
    if (st != nullptr && !st->events().empty()) {
        std::vector<Event>& events = st->events();
        const Tick last = events.back().time;
        for (const std::size_t i : take_.open) {
            events[i].duration = static_cast<std::uint16_t>(last - events[i].time);
        }
    }
    take_.open.clear();
    take_.recording = false;
    take_.pending = true;
    player_->stop();
    return recCount_ != 0; // 1.01 answers 0 for an empty take (suite param3)
}

bool Engine::recordCleanup(const SeqHandle seq, const bool keep, StreamHandle& stream) {
    Sequence* s = sequence(take_.seq);
    Stream* st = s != nullptr ? s->stream(take_.stream) : nullptr;
    if ((!take_.pending && !take_.recording) || st == nullptr) {
        return false;
    }
    if (take_.recording) {
        recordStop();
    }
    std::vector<Event> cut;
    if (take_.cut != 0) {
        if (Stream* c = s->stream(take_.cut); c != nullptr) {
            cut = std::move(c->events());
        }
        s->destroyStream(take_.cut);
        // only the take's events inside the punch range stay
        std::erase_if(st->events(), [&](const Event& e) { return e.time < take_.in || e.time + 1 >= take_.out; });
    }
    if (keep) {
        if (stream == 0) {
            stream = take_.stream;
        } else {
            stream = mergeStreams(seq, stream, take_.stream, true, 0);
        }
    } else {
        destroyStream(take_.stream);
        if (Stream* back = stream != 0 ? this->stream(stream) : nullptr; back != nullptr && !cut.empty()) {
            std::vector<Event>& events = back->events();
            for (const Event& e : cut) {
                events.insert(std::upper_bound(events.begin(), events.end(), e,
                                               [](const Event& a, const Event& b) { return a.time < b.time; }),
                              e);
            }
        }
        if (stream != 0 && this->stream(stream) == nullptr) {
            stream = 0;
        }
    }
    take_ = Take{};
    return true;
}

} // namespace libseq
