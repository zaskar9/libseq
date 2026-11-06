// ttsrun: runs a ttsprobe script (as rendered for one TTSSEQ version) against libseq.
//
//   ttsrun --songs DIR [--out OUT [--files FILES]] [--midi MIDI] SCRIPT [EXPECTED]
//
// prints what the probe would print, or with EXPECTED compares it with the DLL's output (exit
// status 1 on a difference, with the first differing line shown).
//
// DIR holds the test songs (tests/data/songs): a path C:\PROBE\ORC\NAME.WRK in a script is the
// song NAME.WRK in any folder below DIR. A path C:\PROBE\ORC\OUT\NAME is the file NAME in OUT,
// where the suite saves; FILES holds what the DLL saved there (tests/data/ttsseq/101/SUITE/), and
// each of its files must come out the same.
//
// MIDI is what the DLL sent to the fake port (SUITE.midi: milliseconds and bytes). libseq's output
// must be the same messages in the same order; their times count from the call they came during or
// after (the start of a playback, say), and must lie within kSlackMs of the DLL's. Under Wine the
// DLL's timer comes up to about 30 ms late now and then, and the auto-shuttle's wraps add such
// delays up.
//
// The suite's header names its MIDI input ("#! input FILE", in tests/data/ttsseq/input): the
// runner plays it to the engine from SeqOpen on, as the fake port does, with the timer in between.
// The thru's echoes count from SeqOpen too. "#! slack N" lets recorded events (8-byte dumps)
// differ by N ticks in their time and duration: when Wine hands the input to the DLL varies by a
// few milliseconds. The probe's window gets the messages the engine posts (step recording) when
// the script sleeps or idles.
#include "api.hpp"

#include "libseq/engine.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

namespace fs = std::filesystem;

namespace {

std::string upper(std::string s) {
    for (char& c : s) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

std::vector<std::string> lines(const std::string& text) {
    std::vector<std::string> v;
    std::istringstream in(text);
    std::string l;
    while (std::getline(in, l)) {
        if (!l.empty() && l.back() == '\r') {
            l.pop_back();
        }
        v.push_back(l);
    }
    return v;
}

constexpr double kSlackMs = 40;

struct Message {
    double time = 0;
    int port = 0;
    std::vector<std::uint8_t> bytes;
    unsigned long call = 0; // the call it came during or after
    bool timer = false;     // sent by the engine's timer or for an input message, not during a call
    bool input = false;     // sent for an input message (the thru)
};

/// A message of the script's MIDI input (`#! input FILE`): its time after SeqOpen.
struct Input {
    double time = 0;
    std::vector<std::uint8_t> bytes;
};

/// The input script, as the fake port of tools/ttsprobe/wine/fakeseq.c reads it: "<ms> <hex
/// bytes>" lines, # for comments.
std::vector<Input> readInput(const fs::path& path) {
    std::vector<Input> v;
    std::ifstream in(path);
    std::string l;
    while (std::getline(in, l)) {
        std::istringstream f(l);
        Input m;
        std::string tok;
        if (l.empty() || l[0] == '#' || !(f >> m.time)) {
            continue;
        }
        while (f >> tok) {
            m.bytes.push_back(static_cast<std::uint8_t>(std::stoul(tok, nullptr, 16)));
        }
        v.push_back(m);
    }
    std::stable_sort(v.begin(), v.end(), [](const Input& a, const Input& b) { return a.time < b.time; });
    return v;
}

/// The fake port of the runner: it keeps what the engine sends, with the script's time, and plays
/// the input script from SeqOpen on.
class Capture final : public libseq::Platform {
public:
    explicit Capture(ttsrun::Probe& probe) : probe_(&probe) {}
    [[nodiscard]] int outputPorts() const override { return ports; }
    void send(const int port, const std::span<const std::uint8_t> message) override {
        sent.push_back({static_cast<double>(probe_->clock) - base_,
                        port,
                        {message.begin(), message.end()},
                        probe_->calls,
                        inTimer || inInput_,
                        inInput_});
    }
    bool inTimer = false;
    [[nodiscard]] std::uint32_t milliseconds() const override { return probe_->clock; }
    void opened(libseq::Engine& engine) override {
        engine_ = &engine;
        base_ = probe_->clock;
        next_ = 0;
    }
    void closed() override { engine_ = nullptr; }
    /// The engine waits: a millisecond goes by.
    void wait() override {
        if (probe_->advance) {
            probe_->advance(1);
        }
    }
    /// The time of the next input message, if there is one.
    [[nodiscard]] std::optional<std::uint32_t> nextInput() const {
        if (engine_ == nullptr || next_ >= input.size()) {
            return std::nullopt;
        }
        return base_ + static_cast<std::uint32_t>(input[next_].time);
    }
    /// Hands the next input message to the engine.
    void deliver() {
        inInput_ = true;
        engine_->receive(input[next_++].bytes);
        inInput_ = false;
    }
    int ports = 1;
    std::vector<Message> sent;
    std::vector<Input> input;

private:
    ttsrun::Probe* probe_;
    libseq::Engine* engine_ = nullptr;
    std::uint32_t base_ = 0; // SeqOpen: the time 0 of the input and of the capture
    bool inInput_ = false;
    std::size_t next_ = 0;
};

std::string bytesText(const Message& m) {
    std::string t = m.port != 0 ? "@" + std::to_string(m.port) + " " : "";
    for (std::size_t i = 0; i < m.bytes.size(); ++i) {
        static constexpr const char* kHex = "0123456789abcdef";
        t += i != 0 ? " " : "";
        t += kHex[m.bytes[i] >> 4];
        t += kHex[m.bytes[i] & 0xfU];
    }
    return t;
}

std::vector<Message> readCapture(const fs::path& path) {
    std::vector<Message> v;
    std::ifstream in(path);
    std::string l;
    while (std::getline(in, l)) {
        std::istringstream f(l);
        Message m;
        std::string tok;
        if (!(f >> m.time)) {
            continue;
        }
        while (f >> tok) {
            if (tok[0] == '@') {
                m.port = std::stoi(tok.substr(1));
            } else {
                m.bytes.push_back(static_cast<std::uint8_t>(std::stoul(tok, nullptr, 16)));
            }
        }
        v.push_back(m);
    }
    return v;
}

/// Wine's ALSA driver loses the bytes of system common messages (the Song Position Pointer): the
/// fake port logs them as 00 00 00.
bool sameBytes(const Message& want, const Message& got) {
    if (want.port != got.port) {
        return false;
    }
    if (want.bytes == std::vector<std::uint8_t>{0, 0, 0}) {
        return !got.bytes.empty() && got.bytes[0] >= 0xf1 && got.bytes[0] <= 0xf6;
    }
    return want.bytes == got.bytes;
}

/// Compares libseq's MIDI with the DLL's; prints the first difference. True when they agree.
bool compareMidi(const std::vector<Message>& want, const std::vector<Message>& got) {
    std::size_t anchor = 0;
    for (std::size_t i = 0; i < std::max(want.size(), got.size()); ++i) {
        if (i >= want.size() || i >= got.size() || !sameBytes(want[i], got[i])) {
            std::cout << "MIDI differs at message " << i + 1 << " of " << want.size() << ":\n";
            for (std::size_t k = i >= 3 ? i - 3 : 0; k < i + 3; ++k) {
                std::cout << "  want: "
                          << (k < want.size() ? std::to_string(want[k].time) + " " + bytesText(want[k]) : "<end>")
                          << '\n'
                          << "  got:  "
                          << (k < got.size() ? std::to_string(got[k].time) + " " + bytesText(got[k]) : "<end>") << '\n';
            }
            return false;
        }
        if (i == 0 || got[i].call != got[i - 1].call || (got[anchor].input && !got[i].input)) {
            anchor = i;
        }
        // during a call the DLL takes the time it takes (a load); only the timer's output has a time
        if (!got[i].timer) {
            continue;
        }
        // the echo of an input message comes when the input does: both count from SeqOpen, as
        // the input script and the fake port's log do; the timer's output counts from the first
        // message of the call (or after it) it came during
        const bool absolute = got[i].input;
        const double w = want[i].time - (absolute ? 0 : want[anchor].time);
        const double g = got[i].time - (absolute ? 0 : got[anchor].time);
        if (std::abs(w - g) > kSlackMs) {
            std::cout << "MIDI time differs at message " << i + 1 << " (" << bytesText(got[i]) << "): want +" << w
                      << " ms, got +" << g << " ms after "
                      << (absolute ? std::string("SeqOpen") : "message " + std::to_string(anchor + 1)) << '\n';
            return false;
        }
    }
    return true;
}

/// The bytes of a dump line ("  00 01 ..."), empty for another line.
std::vector<unsigned> dumpBytes(const std::string& line) {
    std::vector<unsigned> v;
    if (!line.starts_with("  ")) {
        return {};
    }
    std::istringstream f(line);
    std::string tok;
    while (f >> tok) {
        if (tok.size() != 2 || std::isxdigit(static_cast<unsigned char>(tok[0])) == 0 ||
            std::isxdigit(static_cast<unsigned char>(tok[1])) == 0) {
            return {};
        }
        v.push_back(static_cast<unsigned>(std::stoul(tok, nullptr, 16)));
    }
    return v;
}

/// A recorded event's time depends on when Wine delivered the input to the DLL: with `slack`, two
/// dumps of an 8-byte event agree when they differ only in the time (bytes 0 to 2) and the
/// duration (6 and 7), by at most `slack` ticks each.
bool closeEnough(const std::string& want, const std::string& got, const long slack) {
    if (slack <= 0) {
        return false;
    }
    const std::vector<unsigned> w = dumpBytes(want);
    const std::vector<unsigned> g = dumpBytes(got);
    if (w.size() != libseq::Event::kSize || g.size() != libseq::Event::kSize) {
        return false;
    }
    const auto time = [](const std::vector<unsigned>& b) {
        return static_cast<long>(b[0] | (b[1] << 8) | (b[2] << 16));
    };
    const auto dur = [](const std::vector<unsigned>& b) { return static_cast<long>(b[6] | (b[7] << 8)); };
    return std::equal(w.begin() + 3, w.begin() + 6, g.begin() + 3) && std::abs(time(w) - time(g)) <= slack &&
           std::abs(dur(w) - dur(g)) <= slack;
}

} // namespace

int main(int argc, char** argv) {
    fs::path songs;
    fs::path outDir;
    fs::path files;
    fs::path midi;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--songs" && i + 1 < argc) {
            songs = argv[++i];
        } else if (a == "--out" && i + 1 < argc) {
            outDir = argv[++i];
        } else if (a == "--files" && i + 1 < argc) {
            files = argv[++i];
        } else if (a == "--midi" && i + 1 < argc) {
            midi = argv[++i];
        } else {
            args.push_back(a);
        }
    }
    if (args.empty()) {
        std::cerr << "usage: ttsrun --songs DIR SCRIPT [EXPECTED]\n";
        return 2;
    }
    std::ifstream script(args[0]);
    if (!script) {
        std::cerr << "cannot open " << args[0] << '\n';
        return 2;
    }
    // the suite's header: "#! input FILE" names the MIDI input, in tests/data/ttsseq/input;
    // "#! slack N" lets the times of recorded events differ (closeEnough)
    std::string inputName;
    long slack = 0;
    for (std::string l; std::getline(script, l) && (l.empty() || l[0] == '#' || l.starts_with("mode"));) {
        if (l.starts_with("#! input ")) {
            inputName = l.substr(9);
        } else if (l.starts_with("#! slack ")) {
            slack = std::stol(l.substr(9));
        }
    }
    script.clear();
    script.seekg(0);
    std::ostringstream out;
    ttsrun::Probe probe(out);
    libseq::Engine engine;
    Capture capture(probe);
    engine.setPlatform(&capture);
    if (!inputName.empty()) {
        capture.input = readInput(fs::path(args[0]).parent_path().parent_path() / "input" / inputName);
    }
    // the engine's timer runs every period, on the script's clock, half a period out of step with
    // whole periods: the DLL's timer has no fixed phase to the script, and what it finds due waits
    // for it half a period on average
    std::uint32_t nextTimer = engine.timerPeriod() / 2;
    probe.advance = [&probe, &engine, &nextTimer, &capture](const std::uint32_t ms) {
        const std::uint32_t until = probe.clock + ms;
        for (;;) {
            const std::optional<std::uint32_t> in = capture.nextInput();
            if (in && *in <= until && *in < nextTimer) {
                probe.clock = std::max(probe.clock, *in);
                capture.deliver();
            } else if (nextTimer <= until) {
                probe.clock = nextTimer;
                capture.inTimer = true;
                engine.timer();
                capture.inTimer = false;
                nextTimer += engine.timerPeriod();
            } else {
                break;
            }
        }
        probe.clock = until;
    };
    ttsrun::defineApi101(probe, engine);
    if (!outDir.empty()) {
        fs::remove_all(outDir);
        fs::create_directories(outDir);
    }
    probe.mapPath = [&songs, &outDir](const std::string& p) -> std::string {
        const std::string prefix = R"(C:\PROBE\ORC\)";
        const std::string outPrefix = R"(C:\PROBE\ORC\OUT\)";
        if (!outDir.empty() && upper(p).starts_with(outPrefix)) {
            return (outDir / upper(p.substr(outPrefix.size()))).string();
        }
        if (!upper(p).starts_with(prefix) || songs.empty()) {
            return p;
        }
        const std::string name = upper(p.substr(prefix.size()));
        for (const auto& f : fs::recursive_directory_iterator(songs)) {
            if (f.is_regular_file() && upper(f.path().filename().string()) == name) {
                return f.path().string();
            }
        }
        return p;
    };
    probe.runScript(script);

    if (args.size() < 2) {
        std::cout << out.str();
        return 0;
    }
    std::ifstream exp(args[1], std::ios::binary);
    std::stringstream e;
    e << exp.rdbuf();
    const auto got = lines(out.str());
    const auto want = lines(e.str());
    std::size_t diffs = 0;
    std::size_t first = 0;
    for (std::size_t i = 0; i < std::max(got.size(), want.size()); ++i) {
        const std::string g = i < got.size() ? got[i] : "<end>";
        const std::string w = i < want.size() ? want[i] : "<end>";
        if (g != w && !closeEnough(w, g, slack)) {
            if (diffs++ == 0) {
                first = i;
            }
        }
    }
    // the files the suite saved
    std::size_t fileDiffs = 0;
    if (!files.empty() && fs::is_directory(files)) {
        for (const auto& f : fs::directory_iterator(files)) {
            const fs::path mine = outDir / upper(f.path().filename().string());
            std::ifstream a(f.path(), std::ios::binary);
            std::ifstream b(mine, std::ios::binary);
            const std::string wantBytes((std::istreambuf_iterator<char>(a)), std::istreambuf_iterator<char>());
            const std::string gotBytes((std::istreambuf_iterator<char>(b)), std::istreambuf_iterator<char>());
            if (!b || wantBytes != gotBytes) {
                std::cout << "saved file differs: " << f.path().filename().string() << '\n';
                ++fileDiffs;
            }
        }
    }
    bool midiOk = true;
    if (!midi.empty()) {
        midiOk = compareMidi(fs::exists(midi) ? readCapture(midi) : std::vector<Message>{}, capture.sent);
    }
    for (const auto& [name, n] : probe.missing) {
        std::cout << "not implemented: " << name << " (" << n << " calls)\n";
    }
    if (diffs == 0 && fileDiffs == 0 && midiOk) {
        std::cout << "ok: " << got.size() << " lines"
                  << (midi.empty() ? "" : ", " + std::to_string(capture.sent.size()) + " MIDI") << '\n';
        return 0;
    }
    if (diffs == 0) {
        return 1;
    }
    std::cout << diffs << " of " << want.size() << " lines differ; first at line " << first + 1 << ":\n";
    for (std::size_t i = first >= 2 ? first - 2 : 0; i < first + 3; ++i) {
        std::cout << "  want: " << (i < want.size() ? want[i] : "<end>") << '\n'
                  << "  got:  " << (i < got.size() ? got[i] : "<end>") << '\n';
    }
    return 1;
}
