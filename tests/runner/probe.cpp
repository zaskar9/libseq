#include "probe.hpp"

#include <array>
#include <cstdio>
#include <cstring>
#include <istream>

namespace ttsrun {

namespace {

std::string hex(const unsigned long v, const int width) {
    std::array<char, 32> b{};
    std::snprintf(b.data(), b.size(), "%0*lx", width, v);
    return b.data();
}

/// The probe's tokenizer: words separated by blanks, a quoted string as one token that keeps its
/// opening quote, '#' ends the line.
std::vector<std::string> tokens(const std::string& line) {
    std::vector<std::string> t;
    std::size_t i = 0;
    while (i < line.size()) {
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) {
            ++i;
        }
        if (i >= line.size() || line[i] == '#') {
            break;
        }
        if (line[i] == '"') {
            const std::size_t j = line.find('"', i + 1);
            const std::size_t end = j == std::string::npos ? line.size() : j;
            t.push_back(line.substr(i, end - i)); // keeps the opening quote as a marker
            i = j == std::string::npos ? line.size() : j + 1;
            continue;
        }
        std::size_t j = i;
        while (j < line.size() && line[j] != ' ' && line[j] != '\t') {
            ++j;
        }
        t.push_back(line.substr(i, j - i));
        i = j;
    }
    return t;
}

unsigned long num(const std::string& s, const int base) {
    return std::strtoul(s.c_str(), nullptr, base);
}

/// What follows the command word, as the probe's "echo" prints it.
std::string rest(const std::string& line) {
    std::size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) {
        ++i;
    }
    while (i < line.size() && line[i] != ' ' && line[i] != '\t') {
        ++i;
    }
    if (i < line.size()) {
        ++i; // the probe's tokenizer swallows one separator
    }
    return line.substr(i);
}

} // namespace

// ---------------------------------------------------------------- Memory

std::uint8_t* Memory::at(const FarPtr p, const std::size_t len) {
    if (p.seg >= kBufSeg && p.seg < kBufSeg + 16) {
        auto it = bufs_.find(p.seg - kBufSeg);
        if (it == bufs_.end() || p.off + len > it->second.size()) {
            return nullptr;
        }
        return it->second.data() + p.off;
    }
    if (p.seg >= kStrSeg && p.seg < kStrSeg + strs_.size()) {
        auto& s = strs_[p.seg - kStrSeg];
        return p.off + len <= s.size() ? s.data() + p.off : nullptr;
    }
    return nullptr;
}

std::string Memory::string(const FarPtr p) {
    std::string s;
    for (std::uint16_t o = p.off;; ++o) {
        const std::uint8_t* c = at({p.seg, o});
        if (c == nullptr || *c == 0) {
            break;
        }
        s.push_back(static_cast<char>(*c));
    }
    return s;
}

FarPtr Memory::allocString(const std::string& s) {
    std::vector<std::uint8_t> v(s.begin(), s.end());
    v.push_back(0);
    strs_.push_back(std::move(v));
    return {static_cast<std::uint16_t>(kStrSeg + strs_.size() - 1), 0};
}

void Memory::allocBuffer(const int n, const std::size_t size) {
    bufs_[n] = std::vector<std::uint8_t>(size, 0);
}

// ---------------------------------------------------------------- Probe

Probe::Probe(std::ostream& out) : out_(out) {}

void Probe::runScript(std::istream& in) {
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        run(line);
    }
    out_.flush();
}

std::uint32_t Probe::callback(const FarPtr fn, const std::vector<std::uint16_t>& words) {
    if (fn.seg != Memory::kCallbackSeg) {
        return 0;
    }
    CbRec r;
    r.id = fn.off;
    r.n = static_cast<int>(words.size());
    for (std::size_t i = 0; i < words.size() && i < 8; ++i) {
        r.w[i] = words[i];
    }
    if (!cbs_.empty()) {
        CbRec& last = cbs_.back();
        bool same = last.id == r.id;
        for (int k = 0; k < 8 && same; ++k) {
            same = (cbmerge_ && (cbmask_ & (1U << k)) != 0) || last.w[k] == r.w[k];
        }
        if (same) {
            ++last.count;
            return cbret_;
        }
    }
    cbs_.push_back(r);
    return cbret_;
}

void Probe::flushCallbacks() {
    for (const CbRec& r : cbs_) {
        if (r.id >= 10) {
            out_ << "callback cc" << r.id - 10 << '(';
        } else {
            out_ << "callback cb" << r.id << '(';
        }
        for (int i = 0; i < r.n; ++i) {
            out_ << (i != 0 ? "," : "") << ((cbmask_ & (1U << i)) != 0 ? std::string("----") : hex(r.w[i], 4));
        }
        if (oracle_) {
            out_ << (r.count > 1 ? "), repeated\n" : ")\n");
        } else if (r.count > 1) {
            out_ << ") at 0, " << r.count << " times\n";
        } else {
            out_ << ") at 0\n";
        }
    }
    cbs_.clear();
}

void Probe::run(const std::string& line) {
    const std::vector<std::string> t = tokens(line);
    if (t.empty()) {
        return;
    }
    const std::string& cmd = t[0];
    const auto arg = [&](const std::size_t i) { return i < t.size() ? t[i] : std::string(); };
    if (cmd == "echo") {
        out_ << rest(line) << '\n';
    } else if (cmd == "sleep" || cmd == "dxs" || cmd == "cbyield" || cmd == "win" || cmd == "cbptr" ||
               cmd == "cbretafter") {
        // nothing to wait for; the rest has no effect without a real DLL
    } else if (cmd == "idle") {
        if (const auto it = functions_.find("SEQIDLE"); it != functions_.end()) {
            Args a({});
            const std::uint32_t r = it->second(a, *this);
            if ((r & 0xffffU) != 0) {
                out_ << "SEQIDLE() -> " << hex(r & 0xffffU, 4) << '\n';
            }
        }
        flushCallbacks();
    } else if (cmd == "let") {
        var_[num(arg(1), 10) % 10] = r_;
    } else if (cmd == "mode") {
        oracle_ = arg(1) == "oracle";
    } else if (cmd == "add" || cmd == "set") {
        const auto v = num(arg(1), 10) % 10;
        const auto x = static_cast<std::uint32_t>(num(arg(2), 16));
        var_[v] = cmd == "add" ? var_[v] + x : x;
    } else if (cmd == "cbmask") {
        cbmask_ = static_cast<unsigned>(num(arg(1), 16));
        cbmerge_ = arg(2) == "merge";
    } else if (cmd == "cbret") {
        cbret_ = static_cast<std::uint32_t>(num(arg(1), 16));
    } else if (cmd == "time") {
        if (!oracle_) {
            out_ << "time 0\n";
        }
    } else if (cmd == "load") {
        out_ << "load " << arg(1) << (oracle_ ? " -> ok\n" : " -> 0001\n");
    } else if (cmd == "buf") {
        mem.allocBuffer(static_cast<int>(num(arg(1), 10)), num(arg(2), 0));
    } else if (cmd == "fill") {
        auto& b = mem.buffer(static_cast<int>(num(arg(1), 10)));
        auto off = num(arg(2), 0);
        for (std::size_t i = 3; i < t.size(); ++i, ++off) {
            if (off < b.size()) {
                b[off] = static_cast<std::uint8_t>(num(t[i], 16));
            }
        }
    } else if (cmd == "putw") {
        auto& b = mem.buffer(static_cast<int>(num(arg(1), 10)));
        const auto off = num(arg(2), 0);
        const std::string v = arg(3);
        const auto w = v[0] == '%' ? (var_[num(v.substr(1), 10) % 10] & 0xffffU) : num(v, 16) & 0xffffU;
        b.at(off) = static_cast<std::uint8_t>(w & 0xffU);
        b.at(off + 1) = static_cast<std::uint8_t>(w >> 8);
    } else if (cmd == "put") {
        auto& b = mem.buffer(static_cast<int>(num(arg(1), 10)));
        auto off = num(arg(2), 0);
        const std::string s = arg(3).substr(1);
        for (const char c : s) {
            b.at(off++) = static_cast<std::uint8_t>(c);
        }
        b.at(off) = 0;
    } else if (cmd == "dump") {
        const int n = static_cast<int>(num(arg(1), 10));
        const auto off = num(arg(2), 0);
        const auto len = num(arg(3), 0);
        out_ << "dump " << n << '+' << off << ":\n";
        const auto& b = mem.buffer(n);
        for (unsigned long i = 0; i < len; ++i) {
            const char* sep = " ";
            if (i % 16 == 0) {
                sep = i == 0 ? "  " : "\n  ";
            }
            out_ << sep << hex(off + i < b.size() ? b[off + i] : 0, 2);
        }
        out_ << '\n';
    } else if (cmd.starts_with("call") && (cmd.size() == 4 || cmd[4] == '.')) {
        const char type = cmd.size() > 5 ? cmd[5] : 'r';
        const std::string name = arg(1);
        std::vector<std::uint16_t> w;
        out_ << name << '(';
        for (std::size_t i = 2; i < t.size(); ++i) {
            const std::string& a = t[i];
            out_ << (i > 2 ? "," : "") << a << (a[0] == '"' ? "\"" : "");
            const auto push32 = [&w](const std::uint32_t v) {
                w.push_back(static_cast<std::uint16_t>(v >> 16));
                w.push_back(static_cast<std::uint16_t>(v & 0xffffU));
            };
            if (a[0] == '"') {
                const FarPtr p = mem.allocString(a.substr(1));
                w.push_back(p.seg);
                w.push_back(p.off);
            } else if (a[0] == '@') {
                const auto plus = a.find('+');
                const FarPtr p =
                    mem.bufferPtr(static_cast<int>(num(a.substr(1), 10)),
                                  plus == std::string::npos ? 0U : static_cast<unsigned>(num(a.substr(plus + 1), 0)));
                w.push_back(p.seg);
                w.push_back(p.off);
            } else if (a.starts_with("d:")) {
                push32(static_cast<std::uint32_t>(num(a.substr(2), 16)));
            } else if (a.size() == 3 && a[0] == 'c' && (a[1] == 'b' || a[1] == 'c') && a[2] >= '0' && a[2] <= '8') {
                w.push_back(Memory::kCallbackSeg);
                w.push_back(static_cast<std::uint16_t>(a[2] - '0' + (a[1] == 'c' ? 10 : 0)));
            } else if (a.starts_with("%b")) {
                const auto plus = a.find('+');
                const auto& b = mem.buffer(static_cast<int>(num(a.substr(2), 10)));
                const auto off = plus == std::string::npos ? 0UL : num(a.substr(plus + 1), 0);
                w.push_back(static_cast<std::uint16_t>(b.at(off) | (b.at(off + 1) << 8)));
            } else if (a.starts_with("%d")) {
                push32(var_[num(a.substr(2), 10) % 10]);
            } else if (a[0] == '%') {
                w.push_back(static_cast<std::uint16_t>(var_[num(a.substr(1), 10) % 10] & 0xffffU));
            } else if (a == "hwnd") {
                w.push_back(0x0123);
            } else if (a == "null") {
                w.push_back(0);
                w.push_back(0);
            } else if (a == "$r") {
                push32(r_);
            } else if (a == "$ax") {
                w.push_back(static_cast<std::uint16_t>(r_ & 0xffffU));
            } else if (a == "$dx") {
                w.push_back(static_cast<std::uint16_t>(r_ >> 16));
            } else {
                w.push_back(static_cast<std::uint16_t>(num(a, 16)));
            }
        }
        const auto it = functions_.find(name);
        if (it == functions_.end()) {
            ++missing[name];
            out_ << ") -> not implemented\n";
            return;
        }
        Args a(std::move(w));
        r_ = it->second(a, *this);
        if (type == 'w') {
            out_ << ") -> " << hex(r_ & 0xffffU, 4) << '\n';
        } else if (type == 'd') {
            out_ << ") -> " << hex(r_, 8) << '\n';
        } else if (type == 'v') {
            out_ << ")\n";
        } else if (type == 'p') {
            const FarPtr p{static_cast<std::uint16_t>(r_ >> 16), static_cast<std::uint16_t>(r_ & 0xffffU)};
            if (r_ == 0) {
                out_ << ") -> null\n";
            } else if (p.seg >= Memory::kBufSeg && p.seg < Memory::kBufSeg + 16 &&
                       mem.hasBuffer(p.seg - Memory::kBufSeg)) {
                out_ << ") -> @" << p.seg - Memory::kBufSeg << '+' << p.off << '\n';
            } else {
                out_ << ") -> ptr\n";
            }
        } else {
            out_ << ") -> " << hex(r_ >> 16, 4) << ':' << hex(r_ & 0xffffU, 4) << '\n';
        }
        flushCallbacks();
    } else {
        out_ << "? " << cmd << '\n';
    }
}

} // namespace ttsrun
