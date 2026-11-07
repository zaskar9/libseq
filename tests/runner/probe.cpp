#include "probe.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <istream>
#include <iterator>

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
    if (p.seg == kWindowSeg) {
        return p.off + len <= window_.size() ? window_.data() + p.off : nullptr;
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
    if (cbyield_ && advance && fn.seg == Memory::kCallbackSeg) {
        // the probe's callbacks yield first (then look at the time): in virtual time a millisecond goes by
        advance(1);
        yielded = true;
    }
    return callbackBody(fn, words);
}

std::uint32_t Probe::callbackBody(const FarPtr fn, const std::vector<std::uint16_t>& words) {
    if (fn.seg != Memory::kCallbackSeg) {
        return 0;
    }
    if (cbswitching_ && clock >= cbswitch_) {
        cbret_ = cbret2_;
        cbswitching_ = false;
    }
    CbRec r;
    r.id = fn.off;
    r.n = static_cast<int>(words.size());
    for (std::size_t i = 0; i < words.size() && i < 8; ++i) {
        r.w[i] = words[i];
    }
    const auto matches = [&r](const CbDo& d) { return d.id == r.id && (d.match < 0 || d.match == r.w[0]); };
    if (depth_ < 4 && std::any_of(cbdo_.begin(), cbdo_.end(), matches)) {
        // the callback runs its commands: it prints at once, with them indented below
        const std::array<std::uint16_t, 8> saved = cw_;
        const std::uint32_t savedRet = doret_;
        cw_ = r.w;
        doret_ = cbret_;
        flushCallbacks();
        printHead(r.id, r.n, r.w);
        out_ << " {\n";
        ++depth_;
        const std::vector<CbDo> dos = cbdo_; // a command may change them
        for (const CbDo& d : dos) {
            if (!matches(d)) {
                continue;
            }
            static const std::array<const char*, 17> kQuiet = {
                "letw", "ret",   "fill",    "fillcb", "put",   "putw", "buf",        "let",  "add",
                "set",  "cbret", "cbyield", "cbmask", "cbptr", "dxs",  "cbretafter", "sleep"};
            const std::vector<std::string> t = tokens(d.line);
            const bool loud =
                t.empty() || std::none_of(kQuiet.begin(), kQuiet.end(), [&t](const char* q) { return t[0] == q; });
            if (loud) {
                out_ << "  ";
            }
            run(d.line);
        }
        --depth_;
        out_ << "} -> " << hex(doret_, 1) << '\n';
        const std::uint32_t ret = doret_;
        doret_ = savedRet;
        cw_ = saved;
        return ret;
    }
    if (cbptr_ >= 0 && cbptr_ + 1 < r.n) {
        const FarPtr q{r.w.at(static_cast<std::size_t>(cbptr_)), r.w.at(static_cast<std::size_t>(cbptr_) + 1)};
        int len = cblen_ >= 0 && cblen_ < r.n ? r.w.at(static_cast<std::size_t>(cblen_)) : 16;
        len = std::min(len, 64);
        if (q.null()) {
            r.dlen = -2;
        } else {
            const std::uint8_t* b = mem.at(q, static_cast<std::size_t>(len));
            r.data.assign(b, b == nullptr ? b : b + len);
            r.dlen = static_cast<int>(r.data.size());
        }
    }
    if (!cbs_.empty()) {
        CbRec& last = cbs_.back();
        bool same = last.id == r.id;
        for (std::size_t k = 0; k < 8 && same; ++k) {
            same = (cbmerge_ && (cbmask_ & (1U << k)) != 0) || last.w.at(k) == r.w.at(k);
        }
        if (same) {
            ++last.count;
            return cbret_;
        }
    }
    cbs_.push_back(r);
    return cbret_;
}

void Probe::post(const std::array<std::uint16_t, 4>& message) {
    posted_.push_back(message);
}

void Probe::dispatchPosted() {
    while (!posted_.empty()) {
        const std::array<std::uint16_t, 4> m = posted_.front();
        posted_.pop_front();
        callback({Memory::kCallbackSeg, kMessage}, {m[0], m[1], m[2], m[3]});
    }
    flushCallbacks();
}

void Probe::printHead(const int id, const int n, const std::array<std::uint16_t, 8>& w) {
    if (id == kMessage) {
        out_ << "message(";
    } else if (id >= 10) {
        out_ << "callback cc" << id - 10 << '(';
    } else {
        out_ << "callback cb" << id << '(';
    }
    for (int i = 0; i < n; ++i) {
        out_ << (i != 0 ? "," : "")
             << ((cbmask_ & (1U << i)) != 0 ? std::string("----") : hex(w.at(static_cast<std::size_t>(i)), 4));
    }
    out_ << ')';
}

void Probe::flushCallbacks() {
    for (const CbRec& r : cbs_) {
        if (r.id == kMessage) {
            out_ << "message(";
        } else if (r.id >= 10) {
            out_ << "callback cc" << r.id - 10 << '(';
        } else {
            out_ << "callback cb" << r.id << '(';
        }
        for (int i = 0; i < r.n; ++i) {
            if (r.dlen != -1 && i == cbptr_) {
                if (r.dlen == -2) {
                    out_ << (i != 0 ? "," : "") << "null";
                } else {
                    out_ << (i != 0 ? "," : "") << '[';
                    for (std::size_t k = 0; k < r.data.size(); ++k) {
                        out_ << (k != 0 ? " " : "") << hex(r.data[k], 2);
                    }
                    out_ << ']';
                }
                ++i;
                continue;
            }
            out_ << (i != 0 ? "," : "")
                 << ((cbmask_ & (1U << i)) != 0 ? std::string("----") : hex(r.w.at(static_cast<std::size_t>(i)), 4));
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
    } else if (cmd == "sleep") {
        // the probe dispatches window messages while it sleeps
        // (as the probe's loop: the messages first, then a millisecond)
        for (auto ms = static_cast<std::uint32_t>(num(arg(1), 0)); ms > 0; --ms) {
            dispatchPosted();
            if (advance) {
                advance(1);
            }
        }
    } else if (cmd == "cbretafter") {
        // from MS ms after this command on, the callbacks return HEX
        cbswitch_ = clock + static_cast<std::uint32_t>(num(arg(1), 10));
        cbret2_ = static_cast<std::uint32_t>(num(arg(2), 16));
        cbswitching_ = true;
    } else if (cmd == "cbyield") {
        cbyield_ = num(arg(1), 10) != 0;
    } else if (cmd == "dxs" || cmd == "win") {
        // no effect without a real DLL
    } else if (cmd == "idle") {
        // SeqIdle between messages, as Cakewalk's message loop calls it, once a millisecond
        const auto ms = static_cast<std::uint32_t>(num(arg(1), 0));
        const auto it = functions_.find("SEQIDLE");
        // (as the probe's loop: the messages, SeqIdle, the callbacks' output, then a millisecond)
        for (std::uint32_t i = 0; i < ms; ++i) {
            dispatchPosted();
            if (it != functions_.end()) {
                Args a({});
                const std::uint32_t r = it->second(a, *this);
                if ((r & 0xffffU) != 0) {
                    out_ << "SEQIDLE() -> " << hex(r & 0xffffU, 4) << '\n';
                }
            }
            flushCallbacks();
            if (advance) {
                advance(1);
            }
        }
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
    } else if (cmd == "cbdo") {
        if (arg(1) == "-") {
            cbdo_.clear();
        } else if (cbdo_.size() < 32) {
            const std::string& c = arg(1);
            CbDo d;
            d.id = -1;
            if (c == "msg") {
                d.id = kMessage;
            } else if (c.size() == 3) {
                d.id = c[2] - '0' + (c[1] == 'c' ? 10 : 0);
            }
            d.match = arg(2) == "*" ? -1L : static_cast<long>(num(arg(2), 16) & 0xffffU);
            // the rest of the line after the callback and the word
            std::size_t k = 0;
            for (int f = 0; f < 3 && k < line.size(); ++f) {
                while (k < line.size() && (line[k] == ' ' || line[k] == '\t')) {
                    ++k;
                }
                while (k < line.size() && line[k] != ' ' && line[k] != '\t') {
                    ++k;
                }
            }
            while (k < line.size() && (line[k] == ' ' || line[k] == '\t')) {
                ++k;
            }
            d.line = line.substr(k);
            cbdo_.push_back(d);
        }
    } else if (cmd == "letw") {
        var_[num(arg(1), 10) % 10] = cw_[num(arg(2), 10) & 7U];
    } else if (cmd == "ret") {
        doret_ = static_cast<std::uint32_t>(num(arg(1), 16));
    } else if (cmd == "cbptr") {
        cbptr_ = static_cast<int>(std::strtol(arg(1).c_str(), nullptr, 10));
        cblen_ = t.size() > 2 ? static_cast<int>(std::strtol(arg(2).c_str(), nullptr, 10)) : -1;
    } else if (cmd == "dumpcb" || cmd == "fillcb") {
        const auto k = num(arg(1), 10) & 7U;
        const FarPtr q{cw_[k], cw_[(k + 1) & 7U]};
        const auto off = static_cast<std::uint16_t>(num(arg(2), 0));
        const FarPtr at{q.seg, static_cast<std::uint16_t>(q.off + off)};
        if (cmd == "dumpcb") {
            const auto len = num(arg(3), 0);
            out_ << "dump w" << k << '+' << off << ":\n";
            if (!q.null()) {
                const std::uint8_t* b = mem.at(at, len);
                for (unsigned long i = 0; i < len; ++i) {
                    const char* sep = " ";
                    if (i % 16 == 0) {
                        sep = i == 0 ? "  " : "\n  ";
                    }
                    out_ << sep << hex(b != nullptr ? b[i] : 0, 2);
                }
                out_ << '\n';
            }
        } else if (std::uint8_t* b = mem.at(at, t.size() - 3)) {
            for (std::size_t i = 3; i < t.size(); ++i) {
                b[i - 3] = static_cast<std::uint8_t>(num(t[i], 16));
            }
        }
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
        call(t, cmd);
    } else {
        out_ << "? " << cmd << '\n';
    }
}

void Probe::call(const std::vector<std::string>& t, const std::string& cmd) {
    if (depth_ == 0) {
        ++calls;
        yielded = false;
    }
    const auto arg = [&](const std::size_t i) { return i < t.size() ? t[i] : std::string(); };
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
        } else if (a.starts_with("%w")) {
            w.push_back(cw_[num(a.substr(2), 10) & 7U]);
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
    const bool dots = !cbdo_.empty() && depth_ == 0; // callbacks with commands print in between
    if (dots) {
        out_ << ") ...\n";
    }
    Args a(std::move(w));
    r_ = it->second(a, *this);
    if (dots) {
        out_ << name << "(...";
    }
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
        } else if (p.seg >= Memory::kBufSeg && p.seg < Memory::kBufSeg + 16 && mem.hasBuffer(p.seg - Memory::kBufSeg)) {
            out_ << ") -> @" << p.seg - Memory::kBufSeg << '+' << p.off << '\n';
        } else {
            out_ << ") -> ptr\n";
        }
    } else {
        out_ << ") -> " << hex(r_ >> 16, 4) << ':' << hex(r_ & 0xffffU, 4) << '\n';
    }
    flushCallbacks();
}

} // namespace ttsrun
