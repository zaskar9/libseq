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
#include "api.hpp"

#include "libseq/engine.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
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
    bool timer = false;     // sent by the engine's timer, not during a call
};

/// The fake port of the runner: it keeps what the engine sends, with the script's time.
class Capture final : public libseq::Platform {
public:
    explicit Capture(const ttsrun::Probe& probe) : probe_(&probe) {}
    [[nodiscard]] int outputPorts() const override { return ports; }
    void send(const int port, const std::span<const std::uint8_t> message) override {
        sent.push_back(
            {static_cast<double>(probe_->clock), port, {message.begin(), message.end()}, probe_->calls, inTimer});
    }
    bool inTimer = false;
    [[nodiscard]] std::uint32_t milliseconds() const override { return probe_->clock; }
    int ports = 1;
    std::vector<Message> sent;

private:
    const ttsrun::Probe* probe_;
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
        if (i == 0 || got[i].call != got[i - 1].call) {
            anchor = i;
        }
        // during a call the DLL takes the time it takes (a load); only the timer's output has a time
        if (!got[i].timer) {
            continue;
        }
        const double w = want[i].time - want[anchor].time;
        const double g = got[i].time - got[anchor].time;
        if (std::abs(w - g) > kSlackMs) {
            std::cout << "MIDI time differs at message " << i + 1 << " (" << bytesText(got[i]) << "): want +" << w
                      << " ms, got +" << g << " ms after message " << anchor + 1 << '\n';
            return false;
        }
    }
    return true;
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
    std::ostringstream out;
    ttsrun::Probe probe(out);
    libseq::Engine engine;
    Capture capture(probe);
    engine.setPlatform(&capture);
    // the engine's timer runs every period, on the script's clock, half a period out of step with
    // whole periods: the DLL's timer has no fixed phase to the script, and what it finds due waits
    // for it half a period on average
    std::uint32_t nextTimer = engine.timerPeriod() / 2;
    probe.advance = [&probe, &engine, &nextTimer, &capture](const std::uint32_t ms) {
        const std::uint32_t until = probe.clock + ms;
        while (nextTimer <= until) {
            probe.clock = nextTimer;
            capture.inTimer = true;
            engine.timer();
            capture.inTimer = false;
            nextTimer += engine.timerPeriod();
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
        if (g != w) {
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
