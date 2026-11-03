// The script runner: runs `ttsprobe` scripts (tools/ttsprobe/probe/ttsprobe.c) against libseq and
// prints what the probe prints, so the output can be compared with what TTSSEQ.DLL answered
// (tests/data/ttsseq/<version>/<suite>.out). The commands, the argument syntax and the output
// format are those of the probe; the calls go to an Api instead of a DLL.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace ttsrun {

/// A far pointer, as the probe passes it: segment and offset.
struct FarPtr {
    std::uint16_t seg = 0;
    std::uint16_t off = 0;
    [[nodiscard]] bool null() const noexcept { return seg == 0 && off == 0; }
};

/// The probe's memory: its buffers and string copies, each in a segment of its own, and its
/// callbacks, which are far pointers too.
class Memory {
public:
    static constexpr std::uint16_t kBufSeg = 0x1000;      // buffer N lives in segment 0x1000 + N
    static constexpr std::uint16_t kStrSeg = 0x2000;      // string copies from 0x2000 on
    static constexpr std::uint16_t kCallbackSeg = 0x3000; // callback cbN: 0x3000:N, ccN: 0x3000:10+N

    /// The bytes at a far pointer, or nullptr when it points nowhere the probe knows.
    [[nodiscard]] std::uint8_t* at(FarPtr p, std::size_t len = 1);
    [[nodiscard]] std::string string(FarPtr p);
    FarPtr allocString(const std::string& s);
    void allocBuffer(int n, std::size_t size);
    [[nodiscard]] std::vector<std::uint8_t>& buffer(int n) { return bufs_[n]; }
    [[nodiscard]] bool hasBuffer(int n) const { return bufs_.contains(n); }
    [[nodiscard]] FarPtr bufferPtr(int n, unsigned off) const {
        return {static_cast<std::uint16_t>(kBufSeg + n), static_cast<std::uint16_t>(off)};
    }

private:
    std::map<int, std::vector<std::uint8_t>> bufs_;
    std::vector<std::vector<std::uint8_t>> strs_;
};

/// The arguments of a call as the probe pushed them, read in order.
class Args {
public:
    explicit Args(std::vector<std::uint16_t> w) : w_(std::move(w)) {}
    std::uint16_t word() { return i_ < w_.size() ? w_[i_++] : 0; }
    std::int16_t sword() { return static_cast<std::int16_t>(word()); }
    std::uint32_t dword() {
        const std::uint32_t hi = word();
        return (hi << 16) | word();
    }
    FarPtr ptr() {
        FarPtr p;
        p.seg = word();
        p.off = word();
        return p;
    }
    [[nodiscard]] std::size_t count() const noexcept { return w_.size(); }

private:
    std::vector<std::uint16_t> w_;
    std::size_t i_ = 0;
};

class Probe;

/// What a call into the engine can do with the probe: read and write its memory and call its
/// callbacks. The result is DX:AX.
using Function = std::function<std::uint32_t(Args&, Probe&)>;

class Probe {
public:
    explicit Probe(std::ostream& out);

    /// The functions the "DLL" exports.
    void define(const std::string& name, Function f) { functions_[name] = std::move(f); }
    /// Maps the probe's paths (C:\PROBE\...) to files on this machine.
    std::function<std::string(const std::string&)> mapPath = [](const std::string& p) { return p; };

    void run(const std::string& line);
    void runScript(std::istream& in);

    Memory mem;
    /// Calls a callback the way the engine would; returns what the probe's callback returns.
    std::uint32_t callback(FarPtr fn, const std::vector<std::uint16_t>& words);
    /// The functions called that the Api does not have, by name.
    std::map<std::string, int> missing;

private:
    struct CbRec {
        int id = 0;
        int n = 0;
        std::uint16_t w[8]{};
        unsigned long count = 1;
    };
    void flushCallbacks();
    std::ostream& out_;
    std::map<std::string, Function> functions_;
    bool oracle_ = false;
    std::uint32_t r_ = 0;
    std::uint32_t var_[10]{};
    std::uint32_t cbret_ = 1;
    unsigned cbmask_ = 0;
    bool cbmerge_ = false;
    std::vector<CbRec> cbs_;
};

} // namespace ttsrun
