// libseq: playback (SeqPlayStart, SeqPlayStop, SeqIdle, the engine's timer), as TTSSEQ 1.01 does it.
#pragma once

#include "libseq/engine.hpp"

#include <array>
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

    /// SeqPlayStart: false when already playing or there is nothing to play.
    bool start(Sequence& sequence);
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
    using NoteKey = std::tuple<int, std::uint8_t, std::uint8_t>; // port, status (0x9n), key

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
    [[nodiscard]] Tick clickTicks() const;
    void send(int port, std::initializer_list<std::uint8_t> bytes);

    Engine* engine_;
    Sequence* seq_ = nullptr;
    std::vector<Item> items_;
    Tick end_ = 0; // the last tick anything happens, the ends of notes included
    bool playing_ = false;
    bool ended_ = false; // reached the end; SeqIdle reports it
    double anchorMs_ = 0;
    Tick anchorTick_ = 0;
    double speed_ = 1;
    std::size_t next_ = 0;
    OffHeap offs_;
    std::set<NoteKey> sounding_;
    // the metronome
    Tick clickTick_ = 0;
    bool clickOn_ = false;
    Off click_{}; // the click sounding
    double clickOffMs_ = 0;
    // the auto-shuttle
    bool loop_ = false;
    Tick loopFrom_ = 0;
    Tick loopThru_ = 0;
    bool stopAt_ = false;
    Tick stopTick_ = 0;
};

} // namespace libseq
