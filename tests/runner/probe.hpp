// The script runner: runs `ttsprobe` scripts (tools/ttsprobe/probe/ttsprobe.c) against libseq and
// prints what the probe prints, so the output can be compared with what TTSSEQ.DLL answered
// (tests/data/ttsseq/<version>/<suite>.out). The commands, the argument syntax and the output
// format are those of the probe; the calls go to an Api instead of a DLL.
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <deque>
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
    static constexpr std::uint16_t kWindowSeg = 0x4000;   // memory of the engine a callback is shown

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
    /// A place in segment kWindowSeg for bytes of the engine that a callback gets a pointer to (an
    /// event procedure's event). Each call gets a new place, as different events have different
    /// addresses in the DLL, so that the probe's merging of repeated callbacks stays as it was.
    FarPtr window(const std::vector<std::uint8_t>& bytes) {
        const auto off = static_cast<std::uint16_t>(0x20 + 8 * (windows_++ % 0x1000));
        if (window_.size() < off + bytes.size()) {
            window_.resize(off + bytes.size());
        }
        std::copy(bytes.begin(), bytes.end(), window_.begin() + off);
        return {kWindowSeg, off};
    }

private:
    std::map<int, std::vector<std::uint8_t>> bufs_;
    std::vector<std::vector<std::uint8_t>> strs_;
    std::vector<std::uint8_t> window_;
    unsigned windows_ = 0;
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
    /// The probe's window (`win`, the argument `hwnd`): a message posted to it, (message, wParam,
    /// lParam high, lParam low), which the probe gets when it next sleeps or idles, printed like
    /// a callback ("message(...)", answered with "cbdo msg MSG LINE").
    void post(const std::array<std::uint16_t, 4>& message);
    static constexpr std::uint16_t kMessage = 9; // the window procedure, as a callback id
    /// The functions called that the Api does not have, by name.
    std::map<std::string, int> missing;
    /// The script's time in milliseconds: calls take none, `sleep` and `idle` move it on.
    std::uint32_t clock = 0;
    /// Moves the clock on by that many milliseconds, running the engine's timer on the way.
    std::function<void(std::uint32_t)> advance;
    /// A callback yielded (cbyield 1) since the engine last waited: that wait takes no more time.
    bool yielded = false;
    /// Counts the top-level calls (the runner names the call a message came during or after).
    unsigned long calls = 0;

private:
    struct CbRec {
        int id = 0;
        int n = 0;
        std::array<std::uint16_t, 8> w{};
        unsigned long count = 1;
        int dlen = -1; // with cbptr: the bytes shown, -2 for a null pointer
        std::vector<std::uint8_t> data;
    };
    /// "cbdo": a line run inside a callback whose first word matches (-1: any).
    struct CbDo {
        int id = 0;
        long match = -1;
        std::string line;
    };
    void flushCallbacks();
    void dispatchPosted();
    std::deque<std::array<std::uint16_t, 4>> posted_;
    void printHead(int id, int n, const std::array<std::uint16_t, 8>& w);
    /// The calls of a call command: prints and runs it.
    void call(const std::vector<std::string>& t, const std::string& cmd);
    std::ostream& out_;
    std::map<std::string, Function> functions_;
    bool oracle_ = false;
    std::uint32_t r_ = 0;
    std::uint32_t var_[10]{};
    std::uint32_t cbret_ = 1;
    std::uint32_t callbackBody(FarPtr fn, const std::vector<std::uint16_t>& words);
    bool cbyield_ = false;
    // cbretafter: from cbswitch_ on, the callbacks return cbret2_
    bool cbswitching_ = false;
    std::uint32_t cbswitch_ = 0;
    std::uint32_t cbret2_ = 0;
    unsigned cbmask_ = 0;
    bool cbmerge_ = false;
    std::vector<CbRec> cbs_;
    std::vector<CbDo> cbdo_;
    int depth_ = 0;                     // callbacks running their commands
    std::array<std::uint16_t, 8> cw_{}; // the words of the innermost one (%wN)
    std::uint32_t doret_ = 0;           // what it returns ("ret")
    int cbptr_ = -1, cblen_ = -1;       // "cbptr K L"
};

} // namespace ttsrun
