// libseq::SharedEngine (include/libseq/swift.hpp), the class the Swift package builds on. The Swift
// tests (swift/Tests) make the same checks through Swift.
#include "libseq/swift.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <string>

namespace {

int failures = 0;

/// A string result through a buffer, as Swift reads it.
template <typename F>
std::string text(F fill) {
    std::array<char, 8> small{};
    const std::size_t n = fill(small.data(), small.size());
    std::string s(n + 1, '\0');
    fill(s.data(), s.size());
    s.resize(n);
    return s;
}

void check(const bool ok, const char* what) {
    if (!ok) {
        std::printf("failed: %s\n", what);
        ++failures;
    }
}

struct Fake {
    int sent = 0;
    std::array<std::uint8_t, 3> first{};
};

} // namespace

int main(const int argc, char** argv) {
    // the song corpus, from the command line
    const std::filesystem::path songs = argc > 1 ? argv[1] : "tests/data/songs/corpus";
    Fake fake;
    LIBSEQPLATFORM p{};
    p.context = &fake;
    p.outputPorts = [](void*) { return 1; };
    p.outputName = [](void*, int) { return "Test Out"; };
    p.send = [](void* c, int, const std::uint8_t* m, const std::size_t n) {
        auto* f = static_cast<Fake*>(c);
        if (f->sent++ == 0 && n == 3) {
            std::copy(m, m + 3, f->first.begin());
        }
    };
    p.milliseconds = [](void*) { return std::uint32_t{0}; };

    const libseq::SharedEngine engine;
    const libseq::SharedEngine copy = engine; // a copy is the same engine
    engine.setPlatform(p);
    engine.open();
    check(fake.sent == 16 && fake.first[0] == 0xb0 && fake.first[1] == 122 && fake.first[2] == 0,
          "Local Control Off on 16 channels");
    check(text([&](char* b, std::size_t n) { return copy.portName(0, b, n); }) == "1: Test Out", "portName");

    const libseq::SeqHandle seq = copy.createSequence();
    check(seq == 1 && engine.ppq(seq) == 120, "createSequence");
    const libseq::StreamHandle stream = engine.createStream(seq);
    check(stream == 0x0100 && engine.streamCount(seq) == 1 && engine.streamAt(seq, 0) == stream, "createStream");
    engine.setStreamName(stream, "Piano");
    check(text([&](char* b, std::size_t n) { return engine.streamName(stream, b, n); }) == "Piano", "streamName");
    libseq::Event late;
    late.time = 480;
    late.status = 0x90;
    late.data1 = 64;
    late.data2 = 100;
    late.duration = 60;
    libseq::Event early = late;
    early.time = 240;
    early.data1 = 60;
    check(engine.insertEvent(stream, late) == 0 && engine.insertEvent(stream, early) == 0, "insertEvent");
    check(engine.eventCount(stream) == 2 && engine.eventAt(stream, 1).data1 == 64, "the events in time order");
    check(text([&](char* b, std::size_t n) { return engine.formatPosition(seq, 240, b, n); }) == "1:3:000",
          "formatPosition");
    libseq::MBT m;
    m.measure = 2;
    check(engine.toTick(seq, m) == 480 && engine.toMBT(seq, 480).measure == 2, "toTick, toMBT");
    check(engine.insertTempo(seq, 480, 9000) && engine.tempoAt(seq, 480) == 9000 && engine.tempoAt(seq, 0) == 10000,
          "tempo");
    check(engine.insertMeter(seq, 3, -2, 3, 2) && engine.meterAt(seq, 960).numerator == 3 &&
              engine.meterAt(seq, 960).key == -2,
          "meter");
    check(engine.removeEvent(stream, 0) && engine.eventCount(stream) == 1, "removeEvent");

    const libseq::SeqHandle song = engine.loadCakewalkFile((songs / "JINGLE.WRK").string().c_str());
    check(song == 2, "loadCakewalkFile");
    check(engine.streamCount(song) > 0 && engine.eventCount(engine.streamAt(song, 0)) > 0, "the song's events");
    const std::filesystem::path out = std::filesystem::temp_directory_path() / "libseq-shared-engine.mid";
    check(engine.saveMidiFile(song, out.string().c_str()), "saveMidiFile");
    const libseq::SeqHandle again = engine.loadMidiFile(out.string().c_str());
    check(again == 3 && engine.streamCount(again) > 0, "loadMidiFile");
    std::filesystem::remove(out);
    check(engine.loadCakewalkFile((songs / "MISSING.WRK").string().c_str()) == 0 && engine.fileError() == 7,
          "a missing file");

    check(engine.setPlayingSeq(song) && engine.expired(), "setPlayingSeq");
    engine.close();
    if (failures == 0) {
        std::printf("ok\n");
    }
    return failures == 0 ? 0 : 1;
}
