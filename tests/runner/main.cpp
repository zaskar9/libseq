// ttsrun: runs a ttsprobe script (as rendered for one TTSSEQ version) against libseq.
//
//   ttsrun --songs DIR SCRIPT                 prints what the probe would print
//   ttsrun --songs DIR SCRIPT EXPECTED        compares with the DLL's output; exit status 1 on a
//                                             difference, with the first differing line shown
//
// DIR holds the test songs (tests/data/songs): a path C:\PROBE\ORC\NAME.WRK in a script is the
// song NAME.WRK in any folder below DIR.
#include "api.hpp"

#include "libseq/engine.hpp"

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

} // namespace

int main(int argc, char** argv) {
    fs::path songs;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--songs" && i + 1 < argc) {
            songs = argv[++i];
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
    ttsrun::defineApi101(probe, engine);
    probe.mapPath = [&songs](const std::string& p) -> std::string {
        const std::string prefix = R"(C:\PROBE\ORC\)";
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
    for (const auto& [name, n] : probe.missing) {
        std::cout << "not implemented: " << name << " (" << n << " calls)\n";
    }
    if (diffs == 0) {
        std::cout << "ok: " << got.size() << " lines\n";
        return 0;
    }
    std::cout << diffs << " of " << want.size() << " lines differ; first at line " << first + 1 << ":\n";
    for (std::size_t i = first >= 2 ? first - 2 : 0; i < first + 3; ++i) {
        std::cout << "  want: " << (i < want.size() ? want[i] : "<end>") << '\n'
                  << "  got:  " << (i < got.size() ? got[i] : "<end>") << '\n';
    }
    return 1;
}
