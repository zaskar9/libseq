// libseq: playback (SeqPlayStart, SeqPlayStop, SeqIdle, the engine's timer), as TTSSEQ 1.01 does it.
#pragma once

#include "clock.hpp"
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
    /// The output of a MIDI Time Code lock (at the run that takes the quarter frame after the
    /// second label): the streams' parameters, the Song Position of the start ahead (`at`, when it
    /// is on) and the search-back for it.
    void lockOutput(Tick at);
    /// The search-back output for a position (parameter 1).
    void searchBackFor(Tick at);
    /// MIDI clocks taken at this run: each plays the next ppq / 24 ticks. Clocks taken in the run
    /// that also takes the Start or Continue wait for the next clock.
    void clocks(unsigned count, bool held);
    /// MIDI Stop, or the end of the code: the playback ends at this run; SeqIdle reports it.
    void externalStop();
    /// MIDI Time Code at this run: the run's first quarter frame sets where the playback stands
    /// (`tick`, rounded); after the run the clock goes on from the last (`exact`, in ticks), and the
    /// playback ends when it passes `deadline` without more code (FlywheelAmount).
    void follow(Tick tick, double deadline, bool first, double exact);
    /// The Song Position a start sends, alone (a take refused for want of a stream).
    void songPositionOnly(Sequence& sequence);
    /// MIDI Time Code that jumped: the playback ends at this run, after what the run plays.
    void codeStop() noexcept { codeStop_ = true; }
    /// The count-in's beats that have not clicked yet, while it lasts (0 at its last beat).
    [[nodiscard]] std::uint16_t countInLeft() const noexcept {
        return countInDone_ < countInBeats_ ? static_cast<std::uint16_t>(countInBeats_ - countInDone_) : 0;
    }
    /// True from the start of a recording until the count-in is over.
    [[nodiscard]] bool countingIn() const;
    [[nodiscard]] bool recording() const noexcept { return playing_ && recording_; }
    /// The tick of the take at this run: the start position until the count-in is over, then the
    /// clock.
    [[nodiscard]] Tick recordTick() const;
    /// SeqPlayStop: false when not playing.
    bool stop();
    /// SeqOpen: the timer starts; the clock starts at 0 at the first SeqOpen only.
    void opened(std::uint32_t now);
    /// The engine's timer starts again (SeqSetPlayingSeq, a load).
    void timerStarted(std::uint32_t now);
    /// The first sequence is created: the count-in's position counter starts, one tick behind.
    void created() noexcept;
    /// A timer run: the clock moves on (playing or not).
    void advance(std::uint32_t now);
    /// The rest of the timer run: what is due plays.
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
    /// The playback position: the clock, plus a take's start after its count-in.
    [[nodiscard]] std::int64_t position() const noexcept { return clock_.value() + base_; }
    /// The sequence the clock follows, the clock's units a millisecond, and the size of a tick.
    [[nodiscard]] const Sequence* followed() const;
    [[nodiscard]] std::int64_t perMs() const;
    [[nodiscard]] std::int64_t tickSize(std::int64_t tick) const;
    /// The clock counts on from one tick before `at`.
    void tickBefore(std::uint32_t at);
    /// ppq / 24 ticks of the sequence the clock follows.
    [[nodiscard]] std::int64_t clockTicks() const;
    [[nodiscard]] static bool audible(const Stream& stream);
    [[nodiscard]] Tick lastTick() const noexcept { return end_; }
    /// Where the playback goes no further: the loop's stop, the auto-stop time or the end.
    [[nodiscard]] Tick limit() const noexcept;
    [[nodiscard]] std::set<int> portsInUse() const;
    void schedule();
    void emit(const Item& item);
    void noteOn(const Item& item);
    void noteOff(const Off& off);
    void searchBack(Tick start);
    void stopOutput(bool zeroControllers);
    void silence();
    void click(bool downbeat);
    void clicks(Tick upTo);
    void countInRun();
    void scheduleClock();
    [[nodiscard]] Tick clickTicks() const;
    void send(int port, std::initializer_list<std::uint8_t> bytes);

    // (ordered by size, for the padding)
    Engine* engine_;
    Sequence* seq_ = nullptr;
    TickClock clock_;
    std::int64_t lastIndex_ = 0;     // the clock's MIDI clock at the last run
    std::int64_t countInTarget_ = 0; // the count-in ends when the position counter reaches it
    std::int64_t countInBegin_ = 0;  // the MIDI clock the count-in began at
    std::int64_t countInOff_ = 0;    // the MIDI clock its click ends at
    std::int64_t pOffset_ = 0;       // the position counter: the clock plus this
    std::size_t next_ = 0;
    std::vector<Item> items_;
    OffHeap offs_;
    Tick end_ = 0;          // the last tick anything happens, the ends of notes included
    Tick clickTick_ = 0;    // the metronome's next beat
    Tick clickOffTick_ = 0; // the metronome: when the click sounding ends
    unsigned clocks_ = 0;   // MIDI clock: clocks held for the next one
    Tick spanTick_ = 0;     // MIDI clock: where the next clock's span begins
    std::int64_t base_ = 0; // a take after its count-in: its start (the clock counts from -1)
    double codeLast_ = -1;  // MIDI Time Code: the last quarter frame's time at this run, in ticks
    double deadline_ = 0;   // MIDI Time Code: the playback ends when the clock passes it
    Tick recordTick_ = 0;   // recording: where the take starts
    Tick countInBeatTicks_ = 0;
    // the auto-shuttle
    Tick loopFrom_ = 0;
    Tick loopThru_ = 0;
    Tick stopTick_ = 0;
    Off click_{}; // the click sounding
    std::uint32_t lastRunMs_ = 0;
    std::uint16_t countInBeats_ = 0;
    std::uint16_t countInDone_ = 0;
    std::uint8_t countInNumerator_ = 4;
    bool crossed_ = false; // the clock entered a new MIDI clock at this run
    bool pStarted_ = false;
    bool playing_ = false;
    bool ended_ = false; // reached the end; SeqIdle reports it
    bool clickOn_ = false;
    Drive drive_ = Drive::Timer;
    bool waiting_ = false;    // external sync: the start waits for it
    bool firstClock_ = false; // MIDI clock: the next clock is the first after Start or Continue
    bool recording_ = false;
    bool countIn_ = false;      // counting in
    bool countInWaits_ = false; // the count-in waits for the clock's next MIDI clock
    bool loop_ = false;
    bool stopAt_ = false;
    bool codeStop_ = false;   // MIDI Time Code jumped: the playback ends after this run
    bool ranSince_ = false;   // a timer run came since the start
    bool everOpened_ = false; // SeqOpen came once (the clock carries over a close and open)
    bool atEnd_ = false;      // ended at the sequence's end (SeqIdle has not seen it yet)
};

} // namespace libseq
