// libseq: the engine's model of sequences and streams, as TTSSEQ 1.01 keeps them.
//
// The engine hands out handles: sequences are small numbers, streams carry their sequence in the
// high byte (see types.hpp). Parameters are numbered as the engine numbers them, and each one takes
// values the way the engine does (a flag reads back 1, a byte keeps 8 bits, and so on). The
// behaviour comes from the observations in docs/reverse-engineering/; the tests compare it with
// what TTSSEQ.DLL 1.01 answered (tests/data/ttsseq/101).
#pragma once

#include "event.hpp"
#include "timemap.hpp"
#include "types.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace libseq {

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

/// The engine: the sequences it holds.
class Engine {
public:
    static constexpr std::uint16_t kVersion = 0x0101;

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

    /// SeqLoadCakewalkFile: a new sequence from a `.WRK` file; 0 when it fails (fileError() says why).
    SeqHandle loadCakewalkFile(const std::filesystem::path& path);
    /// SeqSaveCakewalkFile without a program callback: the sequence as 1.01 writes it.
    bool saveCakewalkFile(SeqHandle seq, const std::filesystem::path& path);
    /// SeqSaveMidiFile: the sequence as a format 1 Standard MIDI File, as 1.01 writes it.
    bool saveMidiFile(SeqHandle seq, const std::filesystem::path& path);
    /// SeqLoadMidiFile: a new sequence from a Standard MIDI File; 0 when it fails.
    SeqHandle loadMidiFile(const std::filesystem::path& path);

    /// SeqGetFileError: what the last file operation reported.
    [[nodiscard]] int fileError() const noexcept { return fileError_; }

private:
    [[nodiscard]] SeqHandle freeHandle() const;
    std::map<SeqHandle, std::unique_ptr<Sequence>> sequences_;
    int fileError_ = 0;
    /// The event the MIDI file writer and reader work in. 1.01 keeps one for both and never clears
    /// it, so what a message does not fill (the second data byte and the duration of a program
    /// change or channel pressure that is read) is what the last message left there, from an
    /// earlier file too.
    Event midiScratch_;
    /// The parameters all sequences share, as a new sequence starts them.
    std::array<std::int32_t, qp::kCount> shared_{};
    /// 1.01's current sequence, as far as the probes show it: the sequence created while there is
    /// none, until it is destroyed. A `.WRK` file sets the shared metronome note only when its
    /// sequence becomes the current one (the midifiles suite).
    SeqHandle current_ = 0;
};

} // namespace libseq
