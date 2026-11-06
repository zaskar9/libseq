// libseq: playback (SeqPlayStart, SeqPlayStop, SeqIdle, the engine's timer), as TTSSEQ 1.01 does it.
#pragma once

#include "libseq/engine.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <set>
#include <tuple>
#include <vector>

namespace libseq {

/// The engine's player: what plays, from where, and what is sounding. The engine's timer moves
/// it on (Engine::timer), sending what is due at that moment; SeqIdle reports the end.
class Player {
public:
    explicit Player(Engine& engine) : engine_(&engine) {}

    /// What moves the playback on: the timer (the internal clock), MIDI clock or MIDI Time Code.
    enum class Drive : std::uint8_t { Timer, Clock, Code };
    /// SeqPlayStart, and the start of a recording (SeqRecordStart): false when already playing.
    /// A recording first counts in (parameter 16, measures of clicks before the start position)
    /// and plays on past the end. With external sync the start sends the streams' parameters and
    /// the search-back, but no Song Position, and waits (playing() is false) until go().
    bool start(Sequence& sequence, bool recording = false, Drive drive = Drive::Timer);
    /// External sync: the playback runs from `from` on.
    void go(Tick from);
    /// A start that waits gives up: the stop output goes out.
    void cancel();
    [[nodiscard]] bool waiting() const noexcept { return waiting_; }
    /// The output of an MIDI Time Code lock: the streams' parameters, the Song Position (when it
    /// is on) and the search-back for `at`.
    void lockOutput(Tick at);
    /// The search-back output for a position (parameter 1).
    void searchBackFor(Tick at);
    /// A MIDI clock: the next ppq / 24 ticks play at the timer's next run.
    void clock();
    /// MIDI Stop, or the end of the code: the playback ends where it is; SeqIdle reports it.
    void externalStop();
    /// MIDI Time Code: Now is `tick` at this moment; the playback runs on at most to `deadline`.
    void follow(Tick tick, Tick deadline);
    /// MIDI Time Code that jumped: the playback runs on to `deadline` and ends there.
    void runOut(Tick deadline);
    /// The count-in's beats that have not clicked yet, while it lasts (0 at its last beat).
    [[nodiscard]] std::uint16_t countInLeft() const noexcept { return countInBeats_ - countInDone_; }
    /// True from the start of a recording until the count-in is over.
    [[nodiscard]] bool countingIn() const;
    [[nodiscard]] bool recording() const noexcept { return playing_ && recording_; }
    /// The tick of the take at a time: the start position until the count-in is over, then the
    /// time since then through the tempo map, on past a loop's wrap.
    [[nodiscard]] Tick recordTick(double ms) const;
    /// SeqPlayStop: false when not playing.
    bool stop();
    /// The engine's timer.
    void timer();
    /// Now jumps while playing: the sounding notes end, and the playback goes on from `tick`.
    void jump(Tick tick);
    /// SeqIdle: 1 once after the playback ended by itself (the stop output goes out then).
    std::uint16_t idle();
    [[nodiscard]] bool playing() const noexcept { return playing_; }
    /// The sequence playing, null when none.
    [[nodiscard]] const Sequence* sequence() const noexcept { return seq_; }
    /// Forgets the sequence (it is destroyed); its notes are not ended.
    void forget(const Sequence& sequence) noexcept;

private:
    enum class Kind : std::uint8_t { Note, Short, Sysex, String };
    /// One thing to play: a note (its end goes to the note-off queue when it plays), another
    /// channel message, a sysex bank, or a string event.
    struct Item {
        Tick tick = 0;
        Kind kind = Kind::Short;
        int rank = 0;        // the stream's place in track order
        bool audible = true; // false for a muted or archived stream
        int index = 0;       // the event's place in its stream, counting repetitions
        int port = 0;
        std::uint8_t size = 0;
        std::array<std::uint8_t, 3> data{};
        Tick off = 0;            // Note
        int bank = -1;           // Sysex: the bank's number
        StreamHandle stream = 0; // String
        std::size_t event = 0;   // String: the event's index in its stream
    };
    struct Off {
        Tick tick = 0;
        int port = 0;
        std::uint8_t status = 0;
        std::uint8_t key = 0;
    };
    /// 1.01's note-off queue: a binary min-heap on the time. A new entry moves up past parents with
    /// an equal or later time; after the top goes, the last entry moves down only past children with
    /// an earlier time (the left child on a tie). Ends at the same time leave in that order.
    class OffHeap {
    public:
        [[nodiscard]] bool empty() const noexcept { return heap_.empty(); }
        [[nodiscard]] const Off& top() const { return heap_.front(); }
        void clear() noexcept { heap_.clear(); }
        void push(const Off& off);
        Off pop();

    private:
        std::vector<Off> heap_;
    };

    void build();
    [[nodiscard]] double tickToMs(Tick tick) const;
    [[nodiscard]] Tick msToTick(double ms) const;
    [[nodiscard]] static bool audible(const Stream& stream);
    [[nodiscard]] Tick lastTick() const noexcept { return end_; }
    /// Where the playback goes no further: the loop's stop, the auto-stop time or the end.
    [[nodiscard]] Tick limit() const noexcept;
    [[nodiscard]] std::set<int> portsInUse() const;
    void schedule(double now);
    void emit(const Item& item);
    void noteOn(const Item& item);
    void noteOff(const Off& off);
    void searchBack(Tick start);
    void stopOutput(bool zeroControllers);
    void silence();
    void click(bool downbeat);
    void clicks(double now, Tick limit);
    void countInClicks(double now);
    void scheduleClock();
    [[nodiscard]] Tick clickTicks() const;
    void send(int port, std::initializer_list<std::uint8_t> bytes);

    // (ordered by size, for the padding)
    Engine* engine_;
    Sequence* seq_ = nullptr;
    double anchorMs_ = 0;
    double speed_ = 1;
    std::size_t next_ = 0;
    double clickOffMs_ = 0;         // the metronome: when the click sounding ends
    double recordMs_ = 0;           // recording: when the take starts (the end of the count-in)
    double msPerTick_ = 0;          // at the start, for the count-in
    std::int64_t countInGrid_ = 0;  // the start of the start's measure: the count-in's beats count from it
    std::int64_t countInFirst_ = 0; // the count-in's first beat, a tick before the start (maybe negative)
    std::vector<Item> items_;
    OffHeap offs_;
    Tick end_ = 0; // the last tick anything happens, the ends of notes included
    Tick anchorTick_ = 0;
    Tick clickTick_ = 0;  // the metronome's next beat
    unsigned clocks_ = 0; // MIDI clock: clocks that came in and have not played yet
    Tick spanTick_ = 0;   // MIDI clock: where the next clock's span begins
    Tick deadline_ = 0;   // MIDI Time Code: the playback goes no further
    Tick recordTick_ = 0; // recording: where the take starts
    Tick countInBeatTicks_ = 0;
    // the auto-shuttle
    Tick loopFrom_ = 0;
    Tick loopThru_ = 0;
    Tick stopTick_ = 0;
    Off click_{}; // the click sounding
    std::uint16_t countInBeats_ = 0;
    std::uint16_t countInDone_ = 0;
    bool playing_ = false;
    bool ended_ = false; // reached the end; SeqIdle reports it
    bool clickOn_ = false;
    Drive drive_ = Drive::Timer;
    bool waiting_ = false;    // external sync: the start waits for it
    bool firstClock_ = false; // MIDI clock: the next clock is the first after Start or Continue
    bool recording_ = false;
    std::uint8_t countInNumerator_ = 4;
    bool loop_ = false;
    bool stopAt_ = false;
};

} // namespace libseq
