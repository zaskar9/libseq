// libseq: the engine's model of sequences and streams, as TTSSEQ 1.01 keeps them.
//
// The engine hands out handles: sequences are small numbers, streams carry their sequence in the
// high byte (see types.hpp). Parameters are numbered as the engine numbers them, and each one takes
// values the way the engine does (a flag reads back 1, a byte keeps 8 bits, and so on). The
// behaviour comes from the observations in docs/reverse-engineering/; the tests compare it with
// what TTSSEQ.DLL 1.01 answered (tests/data/ttsseq/101).
#pragma once

#include "event.hpp"
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

private:
    friend class Sequence;
    friend class Engine;
    StreamHandle handle_;
    std::string name_;
    std::array<std::int32_t, sp::kCount> params_{};
    std::vector<Event> events_;
};

struct TempoEntry {
    Tick time = 0;
    std::uint16_t bpm100 = 10000; // beats per minute * 100
};

struct MeterEntry {
    std::uint16_t measure = 0; // 0-based
    std::uint8_t numerator = 4;
    std::uint8_t denominatorLog2 = 2;
    std::int8_t key = 0; // sharps (positive) or flats (negative)
};

struct Marker {
    std::string name;
    Tick time = 0;
    bool smpteLocked = false;
};

struct SysxBank {
    std::uint8_t number = 0;
    bool autoSend = false;
    std::string name;
    std::vector<std::uint8_t> data;
};

/// A sequence: streams, parameters, maps, markers and sysex banks.
class Sequence {
public:
    explicit Sequence(SeqHandle handle);

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

    std::uint16_t ppq = 120;
    Tick now = 0;
    Tick from = 0;
    Tick thru = 0;
    std::vector<TempoEntry> tempoMap{TempoEntry{}};
    std::vector<MeterEntry> meterMap{MeterEntry{}};
    std::vector<Marker> markers;
    std::vector<SysxBank> sysxBanks;
    std::uint16_t fps = 30;
    std::uint32_t frameOffset = 0;

private:
    friend class Engine;
    [[nodiscard]] std::uint8_t freeId() const;
    SeqHandle handle_;
    std::array<std::int32_t, qp::kCount> params_{};
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

    /// SeqLoadCakewalkFile: a new sequence from a `.WRK` file; 0 when it fails (fileError() says why).
    SeqHandle loadCakewalkFile(const std::filesystem::path& path);
    /// SeqGetFileError: what the last file operation reported.
    [[nodiscard]] int fileError() const noexcept { return fileError_; }

private:
    [[nodiscard]] SeqHandle freeHandle() const;
    std::map<SeqHandle, std::unique_ptr<Sequence>> sequences_;
    int fileError_ = 0;
};

} // namespace libseq
