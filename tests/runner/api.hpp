// The script runner's TTSSEQ 1.01: each exported function the oracle scripts call, as the probe
// pushes its arguments (docs/reverse-engineering/signatures.md), handed to libseq's C interface
// (include/libseq/ttsseq.h). The runner turns the probe's far pointers into pointers and its
// callbacks into C functions, and returns what the DLL leaves in DX:AX.
#pragma once

#include "probe.hpp"

#include "libseq/ttsseq.h"

#include <bit>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace ttsrun {

// The probe's memory holds little-endian words, as the C types do on the hosts libseq runs on.
static_assert(std::endian::native == std::endian::little);
static_assert(sizeof(SEQEVENT) == 8);

/// TTSSEQ.DLL 1.01, all its functions the suites call.
void defineApi101(Probe& p);
/// Its event cursor and edit primitives (api101_events.cpp).
void defineEvents101(Probe& p);
/// Its markers and sysex banks (api101_marks.cpp).
void defineMarks101(Probe& p);
/// Its time base, tempo and meter maps and timecode functions (api101_time.cpp).
void defineTime101(Probe& p);
/// Its file functions, SeqIO and event kinds (api101_files.cpp).
void defineFiles101(Probe& p);
/// Its transport (api101_play.cpp).
void definePlay101(Probe& p);
/// Recording (api101_record.cpp).
void defineRecord101(Probe& p);

/// What the C callbacks need to reach the probe. The C interface passes no context, as the DLL
/// passed none, so the runner keeps the probe's callback of each kind while a call runs.
struct Glue {
    Probe* probe = nullptr;
    FarPtr enumCb;  // SeqEnumStreams, SeqEnumSysxBanks, SeqEnumTempos, SeqEnumMeters
    FarPtr startCb; // SeqPlayStart, SeqRecordStart
    FarPtr fileCb;  // SeqLoadCakewalkFile, SeqSaveCakewalkFile
    FarPtr sysxCb;  // SeqReceiveSysxBank
    /// The procedure of each event kind, by its number.
    std::map<std::uint16_t, FarPtr> kinds;
    /// SeqCallEventProc: the event it passes and where the probe has it.
    const SEQEVENT* callerEvent = nullptr;
    FarPtr callerFar;
};
Glue& glue();
/// The C callback that calls the probe's enumeration callback (glue().enumCb); null for none.
SEQENUMPROC enumCallback(FarPtr fn);
/// The C callback that calls the probe's start callback (glue().startCb); null for none.
SEQSTARTPROC startCallback(FarPtr fn);

/// Sets one of the glue's callbacks for the length of a call.
class Scoped {
public:
    Scoped(FarPtr& slot, const FarPtr fn) : slot_(&slot), old_(slot) { slot = fn; }
    ~Scoped() { *slot_ = old_; }
    Scoped(const Scoped&) = delete;
    Scoped& operator=(const Scoped&) = delete;
    Scoped(Scoped&&) = delete;
    Scoped& operator=(Scoped&&) = delete;

private:
    FarPtr* slot_;
    FarPtr old_;
};

/// A word or double word the engine reads and writes through a pointer: a copy of what the probe
/// has there, written back afterwards. ptr() is null when the pointer leads nowhere the probe knows.
template <typename T>
class Out {
public:
    Out(Probe& p, const FarPtr at) : p_(&p), at_(at) {
        if (const std::uint8_t* b = p.mem.at(at, sizeof(T))) {
            std::memcpy(&v_, b, sizeof(T));
            ok_ = true;
        }
    }
    ~Out() {
        if (std::uint8_t* b = ok_ ? p_->mem.at(at_, sizeof(T)) : nullptr) {
            std::memcpy(b, &v_, sizeof(T));
        }
    }
    Out(const Out&) = delete;
    Out& operator=(const Out&) = delete;
    Out(Out&&) = delete;
    Out& operator=(Out&&) = delete;
    [[nodiscard]] T* ptr() noexcept { return ok_ ? &v_ : nullptr; }

private:
    Probe* p_;
    FarPtr at_;
    T v_{};
    bool ok_ = false;
};

/// A buffer the engine writes a string into: the string goes to the probe's memory afterwards,
/// with its 0, if it fits there; nothing goes when the engine wrote nothing.
class Text {
public:
    Text(Probe& p, const FarPtr at) : p_(&p), at_(at), buf_(kSize, kUnwritten) { buf_.back() = 0; }
    ~Text() {
        // the string the engine wrote ends before the buffer's last byte, which is 0 already
        const std::size_t n = std::strlen(buf_.data());
        if (std::uint8_t* b = n + 1 < kSize ? p_->mem.at(at_, n + 1) : nullptr) {
            std::memcpy(b, buf_.data(), n + 1);
        }
    }
    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&&) = delete;
    Text& operator=(Text&&) = delete;
    [[nodiscard]] char* ptr() noexcept { return buf_.data(); }

private:
    static constexpr std::size_t kSize = 1U << 20U;
    static constexpr char kUnwritten = '\x01';
    Probe* p_;
    FarPtr at_;
    std::vector<char> buf_;
};

/// An event the probe passes by pointer.
inline bool readEvent(Probe& p, const FarPtr at, SEQEVENT& e) {
    const std::uint8_t* b = p.mem.at(at, sizeof(SEQEVENT));
    if (b == nullptr) {
        return false;
    }
    std::memcpy(&e, b, sizeof(SEQEVENT));
    return true;
}
inline void writeEvent(Probe& p, const FarPtr at, const SEQEVENT& e) {
    if (std::uint8_t* b = p.mem.at(at, sizeof(SEQEVENT))) {
        std::memcpy(b, &e, sizeof(SEQEVENT));
    }
}

inline std::uint16_t hi(const std::uint32_t v) {
    return static_cast<std::uint16_t>(v >> 16);
}
inline std::uint16_t lo(const std::uint32_t v) {
    return static_cast<std::uint16_t>(v & 0xffffU);
}

/// A far pointer as a function result: the segment in DX, the offset in AX.
inline std::uint32_t farResult(const FarPtr at) {
    return (std::uint32_t{at.seg} << 16) | at.off;
}

/// A signed 32-bit result as DX:AX.
inline std::uint32_t dx(const std::int32_t v) {
    return static_cast<std::uint32_t>(v);
}

} // namespace ttsrun
