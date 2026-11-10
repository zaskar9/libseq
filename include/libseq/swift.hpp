// libseq::SharedEngine: the C++ interface in a form Swift's C++ interoperability imports. Swift
// imports a C++ class as a value when it can copy it, and calls C++ most reliably with plain types,
// C strings and C function pointers: std::string does not convert in every toolchain, and a
// function named begin or end is taken for an iterator. A SharedEngine is a copyable handle to one libseq::Engine; its
// platform is a LIBSEQPLATFORM (include/libseq/ttsseq.h) of C functions. The Swift package
// (Package.swift, swift/) builds its Swift interface on this class.
#pragma once

#include "libseq/engine.hpp"
#include "libseq/ttsseq.h"

#include <cstddef>
#include <cstdint>
#include <memory>

namespace libseq {

class CPlatform;

// The functions are const, the engine is not: Swift calls const functions on a value it does not
// mutate (`let`). So const functions that change the engine are not marked [[nodiscard]].
// NOLINTBEGIN(modernize-use-nodiscard)
class SharedEngine {
public:
    /// A new engine without a platform.
    SharedEngine();

    /// Installs the platform (copied; it replaces the one before).
    void setPlatform(const LIBSEQPLATFORM& platform) const;
    /// The engine itself, for what this class does not offer.
    [[nodiscard]] Engine& engine() const noexcept { return *engine_; }

    // ---- lifecycle, timer, input
    void open() const;
    void close() const;
    /// True once when the playback has ended by itself (SeqIdle).
    bool idle() const;
    void timer() const;
    void receive(const std::uint8_t* message, std::size_t length) const;
    /// "1: device" for port 0. A string result goes into the buffer, as much as fits with its 0;
    /// the function returns its whole length, without the 0.
    std::size_t portName(int port, char* buffer, std::size_t size) const;
    [[nodiscard]] int fileError() const;

    // ---- sequences
    SeqHandle createSequence() const;
    bool destroySequence(SeqHandle seq) const;
    /// 0 when the file cannot be read (fileError says why).
    SeqHandle loadCakewalkFile(const char* path) const;
    bool saveCakewalkFile(SeqHandle seq, const char* path) const;
    SeqHandle loadMidiFile(const char* path) const;
    bool saveMidiFile(SeqHandle seq, const char* path) const;
    [[nodiscard]] std::int32_t seqParam(SeqHandle seq, int param) const;
    bool setSeqParam(SeqHandle seq, int param, std::int32_t value) const;
    [[nodiscard]] std::uint16_t ppq(SeqHandle seq) const;
    bool setPpq(SeqHandle seq, std::uint16_t ppq) const;
    [[nodiscard]] Tick now(SeqHandle seq) const;
    Tick setNow(SeqHandle seq, Tick tick) const;
    /// The start of the last event (SeqGetEnd).
    [[nodiscard]] Tick endTick(SeqHandle seq) const;

    // ---- tempo and meter: tempos in bpm times 100; meters as numerator and power of two
    [[nodiscard]] std::uint16_t tempoAt(SeqHandle seq, Tick tick) const;
    bool setTempo(SeqHandle seq, Tick tick, std::uint16_t tempo) const;
    bool insertTempo(SeqHandle seq, Tick tick, std::uint16_t tempo) const;
    [[nodiscard]] MeterEntry meterAt(SeqHandle seq, Tick tick) const;
    /// Measures from 1.
    bool insertMeter(SeqHandle seq, std::uint16_t measure, std::int8_t key, std::uint8_t numerator,
                     std::uint8_t denominatorLog2) const;
    [[nodiscard]] MBT toMBT(SeqHandle seq, Tick tick) const;
    [[nodiscard]] Tick toTick(SeqHandle seq, const MBT& position) const;
    /// "2:1:033".
    std::size_t formatPosition(SeqHandle seq, Tick tick, char* buffer, std::size_t size) const;

    // ---- streams
    [[nodiscard]] std::size_t streamCount(SeqHandle seq) const;
    /// The stream at the index, in the sequence's order; 0 past the end.
    [[nodiscard]] StreamHandle streamAt(SeqHandle seq, std::size_t index) const;
    StreamHandle createStream(SeqHandle seq) const;
    bool destroyStream(StreamHandle stream) const;
    std::size_t streamName(StreamHandle stream, char* buffer, std::size_t size) const;
    bool setStreamName(StreamHandle stream, const char* name) const;
    [[nodiscard]] std::int32_t streamParam(StreamHandle stream, int param) const;
    bool setStreamParam(StreamHandle stream, int param, std::int32_t value) const;
    [[nodiscard]] std::size_t eventCount(StreamHandle stream) const;
    /// The event at the index; an empty event past the end.
    [[nodiscard]] Event eventAt(StreamHandle stream, std::size_t index) const;
    /// Inserts the event after those at or before its time; returns its index.
    std::size_t insertEvent(StreamHandle stream, const Event& event) const;
    bool removeEvent(StreamHandle stream, std::size_t index) const;

    // ---- transport
    bool setPlayingSeq(SeqHandle seq) const;
    bool playStart() const;
    bool playStop() const;
    /// True while stopped.
    [[nodiscard]] bool expired() const;

private:
    std::shared_ptr<Engine> engine_;
    std::shared_ptr<CPlatform> platform_;
};
// NOLINTEND(modernize-use-nodiscard)

} // namespace libseq
