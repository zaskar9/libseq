// ttsrun: runs a ttsprobe script (as rendered for one TTSSEQ version) against libseq.
//
//   ttsrun --songs DIR [--out OUT [--files FILES]] [--midi MIDI] SCRIPT [EXPECTED]
//
// prints what the probe would print, or with EXPECTED compares it with the DLL's output (exit
// status 1 on a difference, with the first differing line shown).
//
// DIR holds the test songs (tests/data/songs): a path C:\PROBE\ORC\NAME.WRK in a script is the
// song NAME.WRK in any folder below DIR. A path C:\PROBE\ORC\OUT\NAME is the file NAME in OUT,
// where the suite saves; FILES holds what the DLL saved there (tests/data/ttsseq/210/SUITE/), and
// each of its files must come out the same.
//
// The runner keeps time as the probe does in virtual time (tools/ttsprobe/wine/mmsystem16-vtime.
// patch): a millisecond clock that calls do not move; `sleep`, `idle` and every turn of a wait in
// the engine move it on one millisecond at a time. At each millisecond the engine's timer runs
// when due (every period from the moment the engine last started it), then the input due is
// handed over. The suite's header names its MIDI input ("#! input FILE", in tests/data/ttsseq/
// input), times counted from SeqOpen, as the probe's input, and its TTSSEQ.INI ("#! ini FILE"):
// the output devices in port order and the [Options]. The probe's window gets the messages
// the engine posts (step recording) when the script sleeps or idles.
//
// MIDI is what the DLL sent (SUITE.midi: milliseconds and bytes, recorded in virtual time).
// libseq's output must be the same messages at the same milliseconds.
#include "api.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <span>
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

struct Message {
    double time = 0;
    int port = 0;
    std::vector<std::uint8_t> bytes;
    unsigned long call = 0; // the call it came during or after
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

/// The runner's MIDI port and timer, in virtual time: it keeps what the engine sends, with the
/// clock's time, and plays the input script from SeqOpen on. The engine gets it as its platform
/// (LibSeqSetPlatform).
class Capture {
public:
    explicit Capture(ttsrun::Probe& probe) : probe_(&probe) {}
    [[nodiscard]] int outputPorts() const { return static_cast<int>(devices.size()); }
    [[nodiscard]] const char* outputName(const int port) const {
        return port >= 0 && port < outputPorts() ? devices[static_cast<std::size_t>(port)].c_str() : nullptr;
    }
    /// The fake port's outputs are "Fake - MIDI Out" (logged as port 0) and "Fake - MIDI Out N"
    /// (port N); the capture names a message's port that way.
    void send(const int port, const std::span<const std::uint8_t> message) {
        const char* n = outputName(port);
        const std::string name = n != nullptr ? n : "";
        const std::string base = "Fake - MIDI Out";
        const int fake = name.size() > base.size() + 1 ? std::stoi(name.substr(base.size() + 1)) : 0;
        sent.push_back({static_cast<double>(probe_->clock), fake, {message.begin(), message.end()}, probe_->calls});
    }
    [[nodiscard]] std::uint32_t milliseconds() const { return probe_->clock; }
    void opened() {
        open_ = true;
        base_ = probe_->clock;
        next_ = 0;
    }
    void closed() { open_ = false; }
    void startTimer(const unsigned periodMs) {
        period_ = periodMs;
        nextRun_ = probe_->clock + periodMs;
        timer_ = true;
    }
    void stopTimer() { timer_ = false; }
    /// The engine waits: a millisecond goes by, unless a callback took it already.
    void wait() {
        if (probe_->yielded) {
            probe_->yielded = false;
        } else if (probe_->advance) {
            probe_->advance(1);
        }
    }
    /// The step recording's message to the probe's window.
    void post(const std::uint16_t message, const std::uint16_t wParam, const std::uint32_t lParam) {
        probe_->post({message, wParam, ttsrun::hi(lParam), ttsrun::lo(lParam)});
    }
    /// One millisecond on: the timer when due, then the input due.
    void step() {
        ++probe_->clock;
        if (timer_ && open_ && probe_->clock >= nextRun_) {
            nextRun_ += period_;
            LibSeqTimer();
        }
        while (open_ && next_ < input.size() &&
               static_cast<double>(base_) + input[next_].time <= static_cast<double>(probe_->clock)) {
            const std::vector<std::uint8_t>& b = input[next_++].bytes;
            LibSeqReceive(b.data(), b.size());
        }
    }
    /// The C platform that leads here.
    [[nodiscard]] LIBSEQPLATFORM platform() {
        LIBSEQPLATFORM p{};
        p.context = this;
        p.outputPorts = [](void* c) { return self(c).outputPorts(); };
        p.outputName = [](void* c, const int port) { return self(c).outputName(port); };
        p.options = [](void* c, LIBSEQOPTIONS* o) { self(c).options(*o); };
        p.send = [](void* c, const int port, const std::uint8_t* m, const std::size_t n) {
            self(c).send(port, {m, n});
        };
        p.milliseconds = [](void* c) { return self(c).milliseconds(); };
        p.opened = [](void* c) { self(c).opened(); };
        p.closed = [](void* c) { self(c).closed(); };
        p.startTimer = [](void* c, const unsigned ms) { self(c).startTimer(ms); };
        p.stopTimer = [](void* c) { self(c).stopTimer(); };
        p.wait = [](void* c) { self(c).wait(); };
        p.postMessage = [](void* c, void* /*hwnd*/, const std::uint16_t m, const std::uint16_t w,
                           const std::uint32_t l) { self(c).post(m, w, l); };
        return p;
    }
    /// TTSSEQ.INI's [Options]: the engine passes its defaults, the INI changes those it has.
    void options(LIBSEQOPTIONS& o) const {
        for (const auto& [key, value] : opts) {
            const auto u = static_cast<unsigned>(value);
            if (key == "SyncOutputPort") {
                o.syncOutputPort = value;
            } else if (key == "SendLocalOff") {
                o.sendLocalOff = value;
            } else if (key == "Timebase") {
                o.timebase = value;
            } else if (key == "TimerPeriod") {
                o.timerPeriod = u;
            } else if (key == "TimerResolution") {
                o.timerResolution = u;
            } else if (key == "SysxSendPacketSize") {
                o.sysxSendPacketSize = u;
            } else if (key == "SysxReceiveBuffers") {
                o.sysxReceiveBuffers = u;
            } else if (key == "SysxReceiveBuffersSize") {
                o.sysxReceiveBuffersSize = u;
            } else if (key == "ChaseDelay") {
                o.chaseDelay = u;
            } else if (key == "FlywheelAmount") {
                o.flywheelAmount = u;
            }
        }
    }
    std::vector<std::string> devices{"Fake - MIDI Out"};
    std::vector<std::pair<std::string, int>> opts; // [Options], key and value
    std::vector<Message> sent;
    std::vector<Input> input;

private:
    static Capture& self(void* c) { return *static_cast<Capture*>(c); }
    ttsrun::Probe* probe_;
    bool open_ = false;
    std::uint32_t base_ = 0; // SeqOpen: the time 0 of the input
    std::uint32_t nextRun_ = 0;
    unsigned period_ = 10;
    bool timer_ = false;
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

/// A capture of the fake port: what the DLL sent, and the input the fake port delivered to it
/// ("ms < bytes"), on one clock.
struct CaptureFile {
    std::vector<Message> sent;
    std::vector<Input> input;
};

CaptureFile readCapture(const fs::path& path) {
    CaptureFile c;
    std::ifstream in(path);
    std::string l;
    while (std::getline(in, l)) {
        std::istringstream f(l);
        Message m;
        std::string tok;
        if (!(f >> m.time)) {
            continue;
        }
        bool input = false;
        while (f >> tok) {
            if (tok == "<") {
                input = true;
            } else if (tok[0] == '@') {
                m.port = std::stoi(tok.substr(1));
            } else {
                m.bytes.push_back(static_cast<std::uint8_t>(std::stoul(tok, nullptr, 16)));
            }
        }
        if (input) {
            c.input.push_back({m.time, m.bytes});
        } else {
            c.sent.push_back(m);
        }
    }
    return c;
}

/// Compares libseq's MIDI with the DLL's, message by message: the same bytes on the same port at
/// the same millisecond. Prints the first difference; true when they agree.
bool compareMidi(const std::vector<Message>& want, const std::vector<Message>& got) {
    for (std::size_t i = 0; i < std::max(want.size(), got.size()); ++i) {
        if (i < want.size() && i < got.size() && want[i].port == got[i].port && want[i].bytes == got[i].bytes &&
            want[i].time == got[i].time) {
            continue;
        }
        std::cout << "MIDI differs at message " << i + 1 << " of " << want.size() << ":\n";
        for (std::size_t k = i >= 3 ? i - 3 : 0; k < i + 3; ++k) {
            std::cout << "  want: "
                      << (k < want.size() ? std::to_string(want[k].time) + " " + bytesText(want[k]) : "<end>")
                      << "\n  got:  "
                      << (k < got.size() ? std::to_string(got[k].time) + " " + bytesText(got[k]) + " (call " +
                                               std::to_string(got[k].call) + ")"
                                         : "<end>")
                      << '\n';
        }
        return false;
    }
    return true;
}

struct Setup {
    fs::path script;
    fs::path songs;
    fs::path outDir;
    std::string inputName;
    std::string iniName;
};

/// The suite's TTSSEQ.INI ("#! ini FILE", in tests/data/ttsseq/input): the output devices in
/// port order and the [Options] the engine reads.
void readIni(const fs::path& path, Capture& capture) {
    std::ifstream f(path);
    std::string section;
    std::vector<std::string> devices;
    for (std::string l; std::getline(f, l);) {
        if (!l.empty() && l.back() == '\r') {
            l.pop_back();
        }
        if (l.starts_with("[")) {
            section = l;
            continue;
        }
        const std::size_t eq = l.find('=');
        if (eq == std::string::npos) {
            continue;
        }
        const std::string key = l.substr(0, eq);
        const std::string value = l.substr(eq + 1);
        if (section == "[MIDI Output Devices]") {
            devices.push_back(value);
            continue;
        }
        if (section != "[Options]") {
            continue;
        }
        capture.opts.emplace_back(key, std::stoi(value));
    }
    if (!devices.empty()) {
        capture.devices = devices;
    }
}

struct Pass {
    std::string out;
    std::vector<Message> sent;
    std::map<std::string, int> missing;
};

Pass runPass(const Setup& setup) {
    std::ifstream script(setup.script);
    std::ostringstream out;
    ttsrun::Probe probe(out);
    Capture capture(probe);
    // a new engine for the pass, as when Windows loads the DLL
    LibSeqUnload();
    if (!setup.inputName.empty()) {
        capture.input = readInput(setup.script.parent_path().parent_path() / "input" / setup.inputName);
    }
    if (!setup.iniName.empty()) {
        readIni(setup.script.parent_path().parent_path() / "input" / setup.iniName, capture);
    }
    probe.advance = [&capture](std::uint32_t ms) {
        while (ms-- > 0) {
            capture.step();
        }
    };
    const LIBSEQPLATFORM platform = capture.platform();
    LibSeqSetPlatform(&platform);
    ttsrun::defineApi(probe);
    if (!setup.outDir.empty()) {
        fs::remove_all(setup.outDir);
        fs::create_directories(setup.outDir);
    }
    probe.mapPath = [&setup](const std::string& p) -> std::string {
        const std::string prefix = R"(C:\PROBE\ORC\)";
        const std::string outPrefix = R"(C:\PROBE\ORC\OUT\)";
        if (!setup.outDir.empty() && upper(p).starts_with(outPrefix)) {
            return (setup.outDir / upper(p.substr(outPrefix.size()))).string();
        }
        if (!upper(p).starts_with(prefix) || setup.songs.empty()) {
            return p;
        }
        const std::string name = upper(p.substr(prefix.size()));
        for (const auto& f : fs::recursive_directory_iterator(setup.songs)) {
            if (f.is_regular_file() && upper(f.path().filename().string()) == name) {
                return f.path().string();
            }
        }
        return p;
    };
    probe.runScript(script);
    LibSeqUnload();
    LibSeqSetPlatform(nullptr);
    return {out.str(), capture.sent, probe.missing};
}

} // namespace

int main(int argc, char** argv) {
    Setup setup;
    fs::path files;
    fs::path midi;
    fs::path dumpMidi; // where libseq's MIDI goes, in the capture format
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--songs" && i + 1 < argc) {
            setup.songs = argv[++i];
        } else if (a == "--out" && i + 1 < argc) {
            setup.outDir = argv[++i];
        } else if (a == "--files" && i + 1 < argc) {
            files = argv[++i];
        } else if (a == "--midi" && i + 1 < argc) {
            midi = argv[++i];
        } else if (a == "--dump-midi" && i + 1 < argc) {
            dumpMidi = argv[++i];
        } else {
            args.push_back(a);
        }
    }
    if (args.empty()) {
        std::cerr << "usage: ttsrun --songs DIR SCRIPT [EXPECTED]\n";
        return 2;
    }
    setup.script = args[0];
    std::ifstream script(setup.script);
    if (!script) {
        std::cerr << "cannot open " << args[0] << '\n';
        return 2;
    }
    // the suite's header: "#! input FILE" names the MIDI input, in tests/data/ttsseq/input
    for (std::string l; std::getline(script, l) && (l.empty() || l[0] == '#' || l.starts_with("mode"));) {
        if (l.starts_with("#! input ")) {
            setup.inputName = l.substr(9);
        }
        if (l.starts_with("#! ini ")) {
            setup.iniName = l.substr(7);
        }
    }
    const Pass pass = runPass(setup);
    bool midiOk = true;
    if (!midi.empty()) {
        const CaptureFile capture = fs::exists(midi) ? readCapture(midi) : CaptureFile{};
        midiOk = compareMidi(capture.sent, pass.sent);
    }
    if (!dumpMidi.empty()) {
        std::ofstream d(dumpMidi);
        for (const Message& m : pass.sent) {
            d << std::fixed << std::setprecision(3) << m.time << ' ' << bytesText(m) << " #" << m.call << '\n';
        }
    }
    if (args.size() < 2) {
        std::cout << pass.out;
        return 0;
    }
    std::ifstream exp(args[1], std::ios::binary);
    std::stringstream e;
    e << exp.rdbuf();
    const auto got = lines(pass.out);
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
            const fs::path mine = setup.outDir / upper(f.path().filename().string());
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
    for (const auto& [name, n] : pass.missing) {
        std::cout << "not implemented: " << name << " (" << n << " calls)\n";
    }
    if (diffs == 0 && fileDiffs == 0 && midiOk) {
        std::cout << "ok: " << got.size() << " lines"
                  << (midi.empty() ? "" : ", " + std::to_string(pass.sent.size()) + " MIDI") << '\n';
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
