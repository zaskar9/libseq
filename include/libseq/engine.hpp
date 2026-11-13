// libseq: the engine's model of sequences and streams, as TTSSEQ 1.01 keeps them.
//
// The engine hands out handles: sequences are small numbers, streams carry their sequence in the
// high byte (see types.hpp). Parameters are numbered as the engine numbers them, and each one takes
// values the way the engine does (a flag reads back 1, a byte keeps 8 bits, and so on). The
// behaviour comes from the observations in docs/reverse-engineering/; the tests compare it with
// what TTSSEQ.DLL 1.01 answered (tests/data/ttsseq/101).
#pragma once

#include "event.hpp"
#include "platform.hpp"
#include "timemap.hpp"
#include "types.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace libseq {

namespace bytes {
class Writer;
} // namespace bytes
class Player;

/// Stream parameters (SeqGetStreamParam / SeqSetStreamParam).
namespace sp {
inline constexpr int kKeyPlus = 0;   // signed 8 bits
inline constexpr int kVelPlus = 1;   // signed 8 bits
inline constexpr int kTimePlus = 2;  // signed 32 bits, ticks
inline constexpr int kPort = 3;      // 8 bits
inline constexpr int kChannel = 4;   // signed 8 bits, -1 = as recorded
inline constexpr int kPatch = 5;     // signed 8 bits, -1 = none
inline constexpr int kVolume = 6;    // signed 8 bits, -1 = none
inline constexpr int kMuted = 7;     // flag
inline constexpr int kArchived = 8;  // flag
inline constexpr int kLoopCount = 9; // 16 bits
inline constexpr int kPan = 10;      // signed 8 bits, -1 = none
inline constexpr int kPosition = 11; // 16 bits: the track the stream is shown on
inline constexpr int kCount = 12;    // 12 and above do not exist in 1.01: set fails, get gives 0
} // namespace sp

/// Sequence parameters (SeqGetSeqParam / SeqSetSeqParam).
namespace qp {
inline constexpr int kSearchBack = 1;      // flag
inline constexpr int kZeroControllers = 2; // flag
inline constexpr int kNowJumped = 3;       // flag the engine sets when Now is set or jumps
inline constexpr int kSendSpp = 4;         // flag
inline constexpr int kUseContinue = 5;     // flag
inline constexpr int kSendSync = 6;        // flag
inline constexpr int kThruMode = 7;        // signed 8 bits: 0 none, 1 manual, 2 auto
inline constexpr int kThruPort = 8;        // signed 8 bits
inline constexpr int kThruChannel = 9;     // signed 8 bits, -1 = as received
inline constexpr int kThruKeyPlus = 10;    // signed 8 bits
inline constexpr int kThruVelPlus = 11;    // signed 8 bits
inline constexpr int kLocalOnPort = 12;    // signed 16 bits, -1 = none
inline constexpr int kMetroPlay = 13;      // flag
inline constexpr int kMetroRecord = 14;    // flag
inline constexpr int kMetroAccent = 15;    // flag
inline constexpr int kCountIn = 16;        // 8 bits, measures
inline constexpr int kMetroPort = 17;      // 8 bits
inline constexpr int kMetroChannel = 18;   // 8 bits
inline constexpr int kMetroKey = 19;       // 8 bits
inline constexpr int kMetroVelocity = 20;  // 8 bits
inline constexpr int kMetroDuration = 21;  // 8 bits
inline constexpr int kMetroSpeaker = 22;   // flag
inline constexpr int kAutoStop = 23;       // flag
inline constexpr int kAutoRewind = 24;     // flag
inline constexpr int kAutoRestart = 25;    // flag
inline constexpr int kRewindTime = 26;     // 32 bits
inline constexpr int kStopTime = 27;       // 32 bits
inline constexpr int kFilter = 28;         // 8 bits, one bit per channel message type
inline constexpr int kCount = 29;          // 0 and 29 and above do not exist in 1.01
} // namespace qp

/// A stream: a list of events with a cursor, a name and parameters.
class Stream {
public:
    explicit Stream(StreamHandle handle);

    [[nodiscard]] StreamHandle handle() const noexcept { return handle_; }

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    void setName(std::string name) { name_ = std::move(name); }

    /// The parameter as the engine reports it (sign-extended where the engine sign-extends).
    [[nodiscard]] std::int32_t param(int p) const noexcept;
    /// Stores the parameter with the engine's width; false for a parameter 1.01 does not have.
    bool setParam(int p, std::int32_t value) noexcept;

    /// SeqGetStreamAllParams: the parameters as one 22-byte record (byte 15 is not written).
    void allParams(std::uint8_t* out) const noexcept;

    [[nodiscard]] std::vector<Event>& events() noexcept { return events_; }
    [[nodiscard]] const std::vector<Event>& events() const noexcept { return events_; }

    /// Length in ticks (SeqStreamDur): the time of the last event (a note's duration does not count).
    [[nodiscard]] Tick duration() const noexcept;

    /// The event cursor (SeqSeek, SeqTell, SeqNxtEvt).
    std::size_t cursor = 0;

    /// SeqEvtAtTimeGE / GT: the index of the first event at or after (after) a tick; the length
    /// when there is none. Found by halving, as in a sorted stream.
    [[nodiscard]] std::size_t indexAtOrAfter(Tick tick) const noexcept;
    [[nodiscard]] std::size_t indexAfter(Tick tick) const noexcept;
    /// SeqHasEvts for a tick range: whether an event starts in [from, to).
    [[nodiscard]] bool hasEventsIn(Tick from, Tick to) const noexcept;

    /// SeqPutEvt: replaces the event at the cursor. False at the end of the stream.
    bool put(const Event& e);
    /// SeqInsEvt: inserts an event before the cursor.
    void insert(const Event& e);
    /// SeqDelEvt: removes the event at the cursor. False at the end of the stream.
    bool erase();
    /// SeqMoveEvt: gives the event at the cursor a new time and moves it to its place among
    /// the others; returns its new index.
    std::size_t move(Tick tick);
    /// SeqTemporalSort: sorts the events by time as the engine does, with Shell's sort, which
    /// does not keep the order of events at the same time. Returns the length.
    std::size_t sort();
    /// What SeqDeleteRange did.
    enum class RangeResult : std::uint8_t {
        Empty,     // `first` is not before `end`: nothing
        Unchanged, // the range starts at the end of the stream: nothing
        Done,
    };
    /// SeqDeleteRange: removes the events with indices in [first, end). A range that reaches the
    /// end makes the stream end at `first`, also when `first` lies past it: 1.01 then lengthens
    /// the stream with zero events. The cursor stays where it was, within the new length when
    /// events in the middle went.
    RangeResult eraseRange(std::size_t first, std::size_t end);
    /// SeqDisplaceEarlier: moves the events from the first one at or after `from` on earlier by
    /// `amount` ticks (in the 24 bits an event keeps) and puts each back in order among the events
    /// before it. Returns the length.
    std::size_t displaceEarlier(Tick from, Tick amount);
    /// SeqDisplaceLater: moves the events from the first one at or after `from` on later by
    /// `amount` ticks (in 24 bits) without sorting. Returns the length.
    std::size_t displaceLater(Tick from, Tick amount);
    /// SeqInsertRange: inserts copies of the events of `src` before an index, unsorted.
    void insertRange(std::size_t at, const std::vector<Event>& src);

private:
    friend class Sequence;
    friend class Engine;
    StreamHandle handle_;
    std::string name_;
    std::array<std::int32_t, sp::kCount> params_{};
    std::vector<Event> events_;
};

/// A marker. A locked marker keeps a frame instead of a tick, so its tick follows the tempo map and
/// the frame rate (Sequence::markerTime).
struct Marker {
    std::string name;
    Tick time = 0;           // unlocked
    std::uint32_t frame = 0; // locked
    bool smpteLocked = false;
};

/// A sysex bank: bytes with a name, the bank number and whether it is sent when playback starts.
/// The handle packs the sequence like a stream handle (0x0100 is bank 0 of sequence 1).
struct SysxBank {
    std::int16_t number = -1; // -1 for a bank made with SeqCreateSysxBank
    bool autoSend = false;
    std::int16_t port = 0; // parameter 3; a guess: later file versions keep a port per bank
    std::string name;
    std::vector<std::uint8_t> data;

    /// SeqGetSysxBankParam / SeqSetSysxBankParam: 1 auto-send, 2 number, 3 the port; 0, 4 and 5
    /// read 0 and cannot be set.
    [[nodiscard]] std::int32_t param(int p) const noexcept;
    bool setParam(int p, std::int32_t value) noexcept;
};

/// The sysex text of the "Edit Bytes" dialog: "F0 7E ... " with CR LF after every `perLine` bytes
/// (SeqSysxBin2Asc). `perLine` must not be 0.
[[nodiscard]] std::string sysxToText(const std::vector<std::uint8_t>& data, std::size_t perLine);
/// What SeqSysxAsc2Bin made of a text.
struct SysxParse {
    enum Result : std::uint8_t { Ok = 0, NoF0 = 1, NoF7 = 2, NotHex = 4 } result = Ok;
    std::size_t position = 0; // with NotHex: the offset of the character
    std::vector<std::uint8_t> data;
};
[[nodiscard]] SysxParse sysxFromText(const std::string& text);

/// A sequence: streams, parameters, maps, markers and sysex banks.
class Sequence {
public:
    /// `shared` holds the parameters the sequences of an engine share (Sequence::isShared); without
    /// it the sequence keeps its own.
    explicit Sequence(SeqHandle handle, std::array<std::int32_t, qp::kCount>* shared = nullptr);

    /// The metronome note (parameters 17 to 21) is one for all sequences in 1.01: setting it on one
    /// sequence shows on every other, and a new sequence finds it as it was.
    [[nodiscard]] static constexpr bool isShared(const int p) noexcept {
        return p >= qp::kMetroPort && p <= qp::kMetroDuration;
    }

    [[nodiscard]] SeqHandle handle() const noexcept { return handle_; }

    [[nodiscard]] std::int32_t param(int p) const noexcept;
    bool setParam(int p, std::int32_t value) noexcept;

    /// The streams in the order of their ids.
    [[nodiscard]] std::vector<Stream*> streams() const;
    [[nodiscard]] Stream* stream(StreamHandle handle) const noexcept;
    /// A new stream with the lowest free id and the default parameters.
    Stream& createStream();
    /// A stream with this id (used by the file reader); replaces nothing that exists.
    Stream& streamWithId(std::uint8_t id);
    bool destroyStream(StreamHandle handle);
    /// A copy of a stream with a new handle (SeqCloneStream): the events and the name, and the
    /// parameters too when `copyParams`; otherwise a new stream's parameters.
    Stream& clone(const Stream& source, bool copyParams);

    /// A marker's tick: a locked marker's frame converted with the tempo map and the frame rate
    /// as they are now (SeqGetMarkerTime).
    [[nodiscard]] Tick markerTime(const Marker& m) const noexcept;
    /// SeqSetMarkerTime: a tick, or for a locked marker the frame at the tick.
    void setMarkerTime(Marker& m, Tick tick, bool locked) const noexcept;
    /// SeqInsMarker: a marker after those at or before its time.
    void insertMarker(Marker m);
    /// SeqSortMarkers: the markers in time order.
    void sortMarkers();
    /// SeqMarkerForTime: the last marker at or before a tick (0 when there is none).
    [[nodiscard]] std::size_t markerForTime(Tick tick) const noexcept;

    /// SeqSetPPQ: a new time base; the events and the tempo map move to it.
    bool setPpq(std::uint16_t ppq);
    /// SeqSetFPS: false (and no change) for a rate the engine does not take.
    bool setFps(std::uint16_t fps);

    /// SeqTick2Frame: the frame at a tick, rounded to the nearest, from the frame offset on.
    [[nodiscard]] std::uint32_t tickToFrame(Tick tick) const noexcept;
    /// SeqFrame2TickRndOpt: the tick at a frame, rounded to the nearest or up. A frame before the
    /// frame offset gives 0 (TTSSEQ 1.01 answers it with the result of an overflow).
    [[nodiscard]] Tick frameToTick(std::uint32_t frame, bool up) const noexcept;

    std::uint16_t ppq = 120;
    Tick now = 0;
    /// SeqSetDur, SeqGetDur: a length the program keeps here; the engine does not use it.
    Tick dur = 0;
    /// SeqGetEnd: the time of the last event of any stream (where it starts, not where it ends).
    [[nodiscard]] Tick end() const noexcept;
    TempoMap tempoMap;
    MeterMap meterMap;
    std::vector<Marker> markers;
    /// The sysex banks by id (the low byte of the handle).
    std::map<std::uint8_t, SysxBank> sysxBanks;
    /// SeqCreateSysxBank: a new bank with the lowest free id; returns its handle.
    StreamHandle createSysxBank();
    [[nodiscard]] SysxBank* sysxBank(StreamHandle handle) noexcept;
    std::uint16_t fps = 30;
    std::uint32_t frameOffset = 0;
    /// The clock source a `.WRK` file names (VARS 13); 1.01 has one source for the engine, which
    /// the load sets from it (Engine::setSrcClock, so a value it does not take changes nothing).
    std::uint8_t clockSource = 0;
    /// The three tempo ratios (SeqGetTempoRatio) in 64ths, and the one selected.
    std::array<std::uint16_t, 3> tempoRatios{0x20, 0x40, 0x80};
    std::uint16_t tempoRatio = 1;

private:
    friend class Engine;
    [[nodiscard]] std::uint8_t freeId() const;
    SeqHandle handle_;
    std::array<std::int32_t, qp::kCount> params_{};
    std::array<std::int32_t, qp::kCount>* shared_ = nullptr;
    std::map<std::uint8_t, std::unique_ptr<Stream>> streams_;
};

/// What the engine asks of an event procedure (SeqRegisterEventKind): the message, then the
/// string event in the stream (its time, its kind as the status and four bytes the program may
/// use) and a length.
namespace ek {
inline constexpr std::uint16_t kPlay = 1; // the event plays (a copy of it, length 0)
inline constexpr std::uint16_t kFree = 3; // the event goes away (length 0)
inline constexpr std::uint16_t kLoad = 4; // read the event's data from the file (length 7)
inline constexpr std::uint16_t kSave = 5; // write it to the .WRK file (length 7)
inline constexpr std::uint16_t kMidi = 7; // length 0: write to the MIDI file?  length 7: write it
} // namespace ek

/// An event procedure: (message, event, length). The program reads and writes the event's data in
/// the file with Engine::ioRead and Engine::ioWrite; a load fails when it returns 0.
using EventProc = std::function<std::uint32_t(std::uint16_t message, Event& event, std::uint16_t length)>;

/// The program's callback of a file load or save (`LoadProc`, `SaveProc`): (code, word), the code
/// being a chunk type or one of the codes in fileproc.
using FileProc = std::function<std::uint32_t(std::uint16_t code, std::uint16_t word)>;

namespace fileproc {
inline constexpr std::uint16_t kStart = 0xfe; // load: the file is open; save: a percentage
inline constexpr std::uint16_t kVars = 3;     // the VARS chunk is done (the word is not the file)
inline constexpr std::uint16_t kEnd = 0xff;   // the end; a save appends the program's chunks here
inline constexpr std::uint16_t kFile = 7;     // the file word libseq passes (1.01's DOS handle)
} // namespace fileproc

/// The program's callback of a start that waits (`StartProc`): (code, value, value). 1 counting
/// in (the beats left), 2 waiting for MIDI sync, 3 waiting for time code, 4 locking to it, 5
/// still waiting. It returns 0 to go on; anything else gives up.
using StartProc = std::function<std::uint32_t(std::uint16_t code, std::uint16_t a, std::uint16_t b)>;

namespace startproc {
inline constexpr std::uint16_t kCountIn = 1;
inline constexpr std::uint16_t kWaitSync = 2;
inline constexpr std::uint16_t kWaitCode = 3;
inline constexpr std::uint16_t kLocking = 4;
inline constexpr std::uint16_t kWaiting = 5;
} // namespace startproc

/// The engine: the sequences it holds.
class Engine {
public:
    static constexpr std::uint16_t kVersion = 0x020a;

    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    /// A new, empty sequence with the lowest free handle.
    SeqHandle createSequence();
    bool destroySequence(SeqHandle handle);
    [[nodiscard]] Sequence* sequence(SeqHandle handle) const noexcept;
    [[nodiscard]] Stream* stream(StreamHandle handle) const noexcept;
    /// SeqAdoptStream: moves a stream into a sequence under a new handle, which is chosen while
    /// the stream still holds its old one. Returns the new handle, 0 when it fails.
    StreamHandle adoptStream(SeqHandle seq, StreamHandle stream);

    /// SeqMergeStreams: a new stream in `seq` with the events of `a` and those of `b` moved later
    /// by `offset`, `a`'s first at equal times; `a` takes its parameters and name to it and is
    /// destroyed, `b` too when `destroyB`. `a` = 0 means no stream. Returns the new handle, 0
    /// when it fails.
    StreamHandle mergeStreams(SeqHandle seq, StreamHandle a, StreamHandle b, bool destroyB, Tick offset);

    /// SeqDestroyStream: the stream goes, after its string events are freed (freeStringEvents).
    bool destroyStream(StreamHandle handle);
    /// The system the engine sends its MIDI to and takes its time from; null for none.
    void setPlatform(Platform* platform) noexcept { platform_ = platform; }
    [[nodiscard]] Platform* platform() const noexcept { return platform_; }
    /// SeqWire: lets the engine see the system's MIDI devices; without it SeqOpen finds none. The
    /// flag has no effect found.
    void wire(std::uint16_t flag) noexcept;
    /// What SeqOpen returns.
    enum class OpenResult : std::uint8_t {
        Ok = 0,
        /// The engine was not wired: TTSSEQ.DLL reports that no driver is installed for the device,
        /// shows its device dialog with empty lists, and returns this when the dialog is cancelled.
        NoDevices = 3,
    };
    /// SeqOpen: counts the call. When the count becomes 1 the ports open: the engine reads the
    /// platform's options (TTSSEQ.INI) and sends Local Control Off on all 16 channels of every
    /// output port (`SendLocalOff`).
    OpenResult open();
    /// What SeqSleep and SeqWake return.
    enum class SleepResult : std::uint8_t {
        Ok = 0,
        Asleep = 1,
        /// SeqSleep: SeqOpen's count is not 1. SeqWake: the engine is not asleep.
        NotNow = 2,
        Playing = 3,
    };
    /// SeqSleep: closes the ports and stops the timer, as SeqClose does, and keeps everything
    /// else; nothing goes out while the engine is asleep.
    SleepResult sleep();
    /// SeqWake: opens the ports again, as SeqOpen does, Local Control Off included. A playback
    /// started while asleep begins now.
    SleepResult wake();
    /// Whether the ports are open: between SeqOpen and SeqClose, and not asleep.
    [[nodiscard]] bool portsOpen() const noexcept { return portsOpen_; }
    /// Whether SeqSleep closed the ports.
    [[nodiscard]] bool asleep() const noexcept { return asleep_; }
    /// SeqGetPortName: "n: device" for port n - 1.
    [[nodiscard]] std::string portName(int port) const;
    /// The port the Song Position goes to (`SyncOutputPort`; port 1 when that is no port).
    [[nodiscard]] int syncPort() const noexcept;
    /// SeqClose: destroys every sequence and counts SeqOpen's count down, also below 0. When the
    /// count becomes 0 the timer stops and the ports close: the engine resets its output ports
    /// (midiOutReset) and sends nothing itself (the host's closed() may reset its ports). Returns
    /// false when the engine was asleep at that moment: the ports are closed already.
    bool close();
    /// Sends one message to a port, as the engine does all its output; nothing while the ports are
    /// closed.
    void send(int port, std::span<const std::uint8_t> message);
    /// SeqSetStreamParam: sets the parameter; a patch, volume or pan of 0 or more also goes out at
    /// once, on the stream's channel and port (sendStreamParam).
    bool setStreamParam(StreamHandle handle, int param, std::int32_t value);
    /// Sends a stream's patch (program change), volume (controller 7) or pan (10), when it is set
    /// and the stream has a channel and an output port, and its sequence is the current one
    /// (current_). Loading a file does this for each one it sets. The channel is the forced one,
    /// or else the channel of the stream's first channel message; a stream with neither sends
    /// nothing.
    void sendStreamParam(const Stream& stream, int param, bool anySequence = false);
    /// The channel a stream's own messages go out on, -1 when it has none (sendStreamParam).
    [[nodiscard]] static int outputChannel(const Stream& stream) noexcept;
    /// SeqTxMidiMsg: sends a channel message at once, the channel replacing the status's low
    /// nibble; port 0xFFFF means every port, channel 0xFF every channel. The engine keeps the notes
    /// it sent this way: a note off goes out only for a note that is on, a note on for a note that
    /// is on first ends it with a note on of velocity 0. False, and nothing sent, for pitch wheel,
    /// a status that is not a channel message, a channel above 15 or a data byte above 0x7F; true
    /// otherwise, also when a port does not exist or the note off has nothing to end.
    bool txMidiMsg(std::uint16_t port, std::uint16_t channel, std::uint16_t status, std::uint16_t data1,
                   std::uint16_t data2);
    /// MIDI input: the host calls it for every message that comes in, a short message of 1 to 3
    /// bytes or a whole sysex message, when it comes in. The engine takes it at the timer's next
    /// run, as 1.01 does: the thru echoes it then, and a take records it at that run's position.
    void receive(std::span<const std::uint8_t> message);
    /// The timer: the host calls it every timerPeriod() milliseconds while the engine is open.
    void timer();

    /// SeqSetPlayingSeq: the sequence the transport plays (0 for none). It is 1.01's current
    /// sequence: the one whose stream parameters go out when set, and whose thru settings the
    /// input follows (thru_). A sequence created while there is none becomes it.
    bool setPlayingSeq(SeqHandle seq);
    /// SeqSetSeqParam: sets the parameter; a thru setting (7 to 12) set on the current sequence
    /// also goes to the thru.
    bool setSeqParam(SeqHandle seq, int param, std::int32_t value);
    /// SeqSetStandbySeq: kept; no effect found.
    bool setStandbySeq(SeqHandle seq);
    [[nodiscard]] SeqHandle playingSeq() const noexcept { return current_; }
    /// SeqPlayStart: plays the playing sequence from its Now; false when it plays already or there
    /// is no playing sequence. With MIDI sync or time code as the clock source it waits for it,
    /// calling `callback` (2 and 5, or 3, 4 and 5; see StartProc), and gives up when that says so.
    bool playStart(const StartProc& callback = {});
    /// SeqSetSrcClock: 0 the internal clock, 1 MIDI sync, 3 MIDI Time Code; false for another value,
    /// which changes nothing. One source for the engine in 1.01.
    bool setSrcClock(std::uint16_t source);
    [[nodiscard]] std::uint16_t srcClock() const noexcept { return clockSource_; }
    static constexpr std::uint16_t kSourceInternal = 0;
    static constexpr std::uint16_t kSourceMidi = 1;
    static constexpr std::uint16_t kSourceCode = 3;
    /// SeqPlayStop: stops the playback; false when it does not play.
    bool playStop();
    /// SeqSetNow: moves Now (sets parameter 3); while playing, the playback goes on from there.
    Tick setNow(SeqHandle seq, Tick tick);
    /// SeqIdle: 1 once when the playback has ended by itself (and its stop output goes out then).
    std::uint16_t idle();
    /// SeqExpired: true while the transport is stopped.
    [[nodiscard]] bool expired() const noexcept;
    /// SeqRecordStart: records into a new take of the playing sequence from its Now, after the
    /// count-in (parameter 16, measures), during which `callback` gets (1, beats left, 0) over and
    /// over. A `target` stream punches: its events from `in` up to `out` leave it at once (an
    /// `out` of 0 counts as 1). False when there is no playing sequence, it plays already, or
    /// the callback gives up.
    bool recordStart(StreamHandle target, Tick in, Tick out, const StartProc& callback = {});
    /// SeqRecordStop: the notes held end at the take's last event; the stop output goes out. False
    /// when nothing was recorded (or there is no recording).
    bool recordStop();
    /// SeqRecCount: the events of the take so far (also of a step recording).
    [[nodiscard]] std::uint32_t recCount() const noexcept { return recCount_; }
    /// SeqSetRecCount: keeps the first `count` events of the take; returns the count.
    std::uint32_t setRecCount(std::uint32_t count);
    /// What a step recording tells the program after each step: the number of events so far (1.01
    /// posts message 0x464 to the window SeqStepRecSetup got, wParam 0x100).
    using StepNotify = std::function<void(std::uint32_t count)>;
    /// SeqStepRecSetup: starts a step recording into a new stream of `seq`, whose handle it returns
    /// (0 when there is no such sequence).
    StreamHandle stepRecSetup(SeqHandle seq, StepNotify notify);
    /// SeqStepRecParams: where the next step goes, its notes' duration and velocity (0 keeps the
    /// velocity played).
    void stepRecParams(Tick position, std::uint16_t duration, std::uint16_t velocity);
    /// SeqStepRecTerm: ends the step recording; returns the number of events.
    std::uint32_t stepRecTerm();
    /// SeqRecordEvent: adds an event to the take at Now (its time is ignored).
    bool recordEvent(const Event& event);
    /// SeqRecordCleanup: ends the take. `stream` is read and written: kept with 0 there, the
    /// take becomes a new stream, whose handle goes there; kept with a stream there, the two are
    /// merged into a new one (the stream's events first at equal times). After a punch only the
    /// take's events with in <= time and time + 1 < out are kept, and discarding puts the cut
    /// events back into the stream there.
    bool recordCleanup(SeqHandle seq, bool keep, StreamHandle& stream);
    /// The program's callback of a sysex receive: the bytes received so far and the free memory.
    /// It returns 0 to go on receiving.
    using SysxReceiveProc = std::function<std::uint32_t(std::uint32_t received, std::uint32_t free)>;
    /// SeqReceiveSysxBank: appends every sysex message that comes in to the bank, until the
    /// callback returns non-zero. A message comes in through buffers of SysxReceiveBuffersSize
    /// bytes, and a buffer whose second byte is 7E or 7F is left out (universal sysex). 1, or -2
    /// when the bank is still empty; -4 when a message comes in with fewer than 2 receive buffers
    /// (SysxReceiveBuffers); 0 for no such bank.
    std::int16_t receiveSysxBank(StreamHandle bank, const SysxReceiveProc& callback);
    /// SeqSendSysxBank: sends the bank's bytes, in packets of SysxSendPacketSize bytes cut
    /// anywhere (0: one byte), 60 ms apart (the call waits).
    bool sendSysxBank(StreamHandle bank);
    /// `TimerPeriod` of TTSSEQ.INI (1.01: 10 ms).
    [[nodiscard]] unsigned timerPeriod() const noexcept { return timerPeriod_; }

    /// SeqRegisterEventKind: a kind of string event, numbered from 1 (the lowest free number); a
    /// name may be registered more than once. 0 when all 127 numbers are taken.
    std::uint8_t registerEventKind(std::string name, EventProc proc, std::uint32_t value = 0);
    /// SeqUnregisterEventKind: the first kind with that name goes.
    bool unregisterEventKind(const std::string& name);
    /// The registration value's bits [probe, suite eventkinds]: the procedure of a kind registered
    /// with kNoPlay gets no message when its events play, with kNoFree none when they go.
    static constexpr std::uint32_t kNoPlay = 1;
    static constexpr std::uint32_t kNoFree = 2;
    /// A string event plays: its kind's procedure gets message 1 with a copy of it and length 0.
    void playStringEvent(const Event& event);
    /// SeqCallEventProc: the procedure of the event's kind (its status), 0 when there is none.
    std::uint32_t callEventProc(std::uint16_t message, Event& event, std::uint16_t length);
    /// The names of the kinds, by number.
    [[nodiscard]] const std::map<std::uint8_t, std::string>& eventKindNames() const noexcept { return kindNames_; }

    /// SeqIOSeek, SeqIORead, SeqIOWrite: the file the engine has open during a load or a save,
    /// whatever handle the program passes. Seek: origin 0 the start, 1 here, 2 the end; returns the
    /// position. Read and write return the bytes moved; all give 0 with no file open.
    std::uint32_t ioSeek(std::int32_t offset, int origin);
    std::size_t ioRead(std::uint8_t* to, std::size_t count);
    std::size_t ioWrite(const std::uint8_t* from, std::size_t count);

    /// SeqLoadCakewalkFile: a new sequence from a `.WRK` file; 0 when it fails (fileError() says why).
    /// With a callback the program sees the file open, each chunk before the engine reads it (it
    /// returns 1 when it read the chunk itself), the VARS chunk done, and the end.
    SeqHandle loadCakewalkFile(const std::filesystem::path& path, const FileProc& callback = {});
    /// SeqSaveCakewalkFile: the sequence as 1.01 writes it. The callback gets 0xFE with 0 first and
    /// with a percentage after each stream and sysex bank, 3 after VARS (0 at either of the first
    /// two cancels the save), and 0xFF with the file before the end marker.
    bool saveCakewalkFile(SeqHandle seq, const std::filesystem::path& path, const FileProc& callback = {});
    /// SeqSaveMidiFile: the sequence as a format 1 Standard MIDI File, as 1.01 writes it.
    bool saveMidiFile(SeqHandle seq, const std::filesystem::path& path);
    /// SeqLoadMidiFile: a new sequence from a Standard MIDI File; 0 when it fails.
    SeqHandle loadMidiFile(const std::filesystem::path& path);

    /// SeqGetFileError: what the last file operation reported.
    [[nodiscard]] int fileError() const noexcept { return fileError_; }

private:
    struct EventKind {
        std::string name;
        EventProc proc;
        std::uint32_t value = 0; // the program's value: flags (kNoPlay, kNoFree); the procedures do not get it
    };
    /// The file of a load or save: its bytes and the position SeqIOSeek, SeqIORead and SeqIOWrite
    /// work at.
    struct OpenFile {
        std::vector<std::uint8_t> data;
        std::size_t pos = 0;
        bool open = false;
    };
    [[nodiscard]] SeqHandle freeHandle() const;
    /// The kind of a string event's status, nullptr when none is registered.
    [[nodiscard]] const EventKind* kindOf(std::uint8_t status) const noexcept;
    /// The free message (3) for the string events of a stream that goes (also when a load fails),
    /// but not for kinds registered with kNoFree.
    void freeStringEvents(Stream& stream);
    /// Writes a stream as chunk 18 to the open file, with the string events' procedures; false
    /// when one fails. `w` holds what is not written yet.
    bool stringStream(const Stream& stream, std::uint32_t number, bytes::Writer& w);
    std::map<SeqHandle, std::unique_ptr<Sequence>> sequences_;
    std::map<std::uint8_t, EventKind> kinds_;
    std::map<std::uint8_t, std::string> kindNames_;
    OpenFile io_;
    Platform* platform_ = nullptr;
    std::unique_ptr<Player> player_;
    SeqHandle standby_ = 0;
    unsigned timerPeriod_ = 10;
    Options options_; // TTSSEQ.INI as SeqOpen read it
    bool wired_ = false;
    int openCount_ = 0; // SeqOpen calls less SeqClose calls
    bool portsOpen_ = false;
    bool asleep_ = false;
    void openPorts();
    void closePorts();
    int fileError_ = 0;
    /// The event the MIDI file writer and reader work in. 1.01 keeps one for both and never clears
    /// it, so what a message does not fill (the second data byte and the duration of a program
    /// change or channel pressure that is read) is what the last message left there, from an
    /// earlier file too.
    Event midiScratch_;
    /// The parameters all sequences share, as a new sequence starts them.
    std::array<std::int32_t, qp::kCount> shared_{};
    /// 1.01's current sequence: the playing sequence (SeqSetPlayingSeq), or the sequence created
    /// while there is none, until it is destroyed. A `.WRK` file sets the shared metronome note
    /// only when its sequence becomes the current one (the midifiles suite).
    SeqHandle current_ = 0;
    /// The thru settings the input follows, parameters 7 to 12 (suite thru): the defaults of a new
    /// sequence at SeqOpen, all of them taken from the playing sequence by SeqSetPlayingSeq (mode 0
    /// for none), and one of them when it is set on the current sequence. Nothing else changes
    /// them: they stay when the current sequence is destroyed and another one becomes current.
    std::array<std::int32_t, 6> thru_{2, 0, -1, 0, 0, -1};
    /// Sends a channel message on a port, keeping the notes on (notes_) as SeqTxMidiMsg and the
    /// thru do: a note off goes only for a note that is on, a second note on ends the first.
    void sendTracked(int port, std::span<const std::uint8_t> message);
    /// External sync (src/sync.cpp).
    bool startOnClock(Sequence& s, const StartProc& callback);
    bool startOnCode(Sequence& s, const StartProc& callback);
    /// A system message that came in: MIDI sync or a quarter frame.
    void syncInput(std::span<const std::uint8_t> message);
    void quarterFrame(std::uint8_t data);
    /// The time code's time in seconds, minus the sequence's SMPTE offset, in ticks with their
    /// fraction (0 before it).
    [[nodiscard]] static double codeTicks(const Sequence& s, double seconds);
    [[nodiscard]] static double offsetSeconds(const Sequence& s);
    std::uint16_t clockSource_ = kSourceInternal;
    /// MIDI sync: what came in while the start waits, and the Song Position kept for the next
    /// Continue.
    struct ClockSync {
        bool waiting = false;
        int go = 0;        // 1 Start, 2 Continue
        unsigned held = 0; // clocks taken with the Start or Continue
        bool stopKept = false;
        std::optional<Tick> position;
    };
    ClockSync clockSync_;
    /// MIDI Time Code as it comes in: the quarter frames of the current two-frame group and the
    /// decoder's count (quarterFrame).
    struct CodeSync {
        std::array<std::uint8_t, 8> pieces{};
        int expect = -1;          // the next quarter frame of the group, -1 before a group begins
        bool inside = false;      // the group began inside another (a piece 0 came before its piece 7)
        bool known = false;       // a label was read
        int run = 0;              // labels read one after the other, without a jump
        int rate = -1;            // 0 24, 1 25, 2 30 drop frame, 3 30 fps
        std::int64_t group = 0;   // the two-frame group expected
        int bit = 0;              // the low bit of the frames, for the current group
        int parity = 0;           // the low bit of the frames of the last label
        std::int64_t count = 0;   // quarter frames taken
        std::int64_t jumpAt = -1; // following: the quarter frame a jump group began at
        double time = 0;          // the code's time at the last quarter frame, in seconds
        double label = 0;         // the code's time at the last quarter frame of the last label read
        std::int64_t labelAt = 0; // that quarter frame (its number in count)
        double first = 0;         // the code's time at the first quarter frame of the last run with code
        bool inRun = false;       // a quarter frame was taken at this run
        bool following = false;
    };
    CodeSync code_;
    unsigned chaseDelay_ = 20; // ChaseDelay, tenths of a second
    unsigned flywheel_ = 5;    // FlywheelAmount, tenths of a second
    /// The timer starts again (SeqSetPlayingSeq, a load).
    void restartTimer();
    /// MIDI Time Code: a quarter frame was taken at this run already (the first moves the playback).
    bool codeTaken_ = false;
    /// The input that came in since the last run, taken at this run.
    void takeInput();
    void takeChannelInput();
    std::vector<std::vector<std::uint8_t>> channelIn_;
    void input(std::span<const std::uint8_t> message);
    std::mutex inboxMutex_;
    std::vector<std::vector<std::uint8_t>> inbox_;
    /// A sysex message that came in: for the bank being received, if any.
    void receiveSysx(std::span<const std::uint8_t> message);
    /// MIDI thru: echoes a channel message with the current sequence's thru settings.
    void thru(std::span<const std::uint8_t> message);
    /// A take: the stream that collects it (a stream of the sequence from the start on), and the
    /// punch's cut events, kept in a stream of their own.
    struct Take {
        bool recording = false;
        bool pending = false; // stopped, not cleaned up yet
        SeqHandle seq = 0;
        StreamHandle stream = 0;
        StreamHandle cut = 0;
        Tick in = 0;
        Tick out = 0;
        std::vector<std::size_t> open; // notes not ended yet, in the order they started
    };
    /// A step recording.
    struct StepRecording {
        bool on = false;
        SeqHandle seq = 0;
        StreamHandle stream = 0;
        Tick position = 0;
        std::uint16_t duration = 0;
        std::uint16_t velocity = 0;
        std::set<std::pair<unsigned, std::uint8_t>> held; // channel and key of the keys down
        StepNotify notify;
    };
    void stepRecord(std::span<const std::uint8_t> message);
    /// The bank a sysex receive fills (0 for none) and the bytes it took so far.
    StreamHandle sysxReceive_ = 0;
    std::uint32_t sysxReceived_ = 0;
    bool sysxFailed_ = false; // a message came in with fewer than 2 receive buffers
    StepRecording step_;
    std::uint32_t recCount_ = 0;
    /// An input message for the take.
    void record(std::span<const std::uint8_t> message);
    Take take_;
    friend class Player;
    using NoteKey = std::tuple<int, int, int>; // port, channel, key
    /// The notes on, whoever sent them: the playback, SeqTxMidiMsg and the thru keep one table
    /// (suites midiout and thru). Stopping ends them all; SeqClose forgets them.
    std::set<NoteKey> notes_;
};

} // namespace libseq
