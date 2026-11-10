import Foundation
import LibSeq
import Testing

/// A platform with one port that counts what goes out.
final class FakePlatform: SeqPlatform {
    var sent: [[UInt8]] = []
    var outputNames: [String] { ["Test Out"] }
    var milliseconds: UInt32 { 0 }
    func send(port: Int, message: UnsafeBufferPointer<UInt8>) { sent.append(Array(message)) }
    func startTimer(periodMs: UInt32) {}
    func stopTimer() {}
}

/// The same checks as tests/cpp/shared_engine.cpp, through Swift (Swift Testing, which the
/// Command Line Tools have; XCTest comes only with Xcode).
struct LibSeqTests {
    /// The repository's song corpus.
    let songs = URL(fileURLWithPath: #filePath)
        .deletingLastPathComponent().deletingLastPathComponent().deletingLastPathComponent()
        .deletingLastPathComponent().appendingPathComponent("tests/data/songs/corpus")

    @Test func engine() throws {
        let platform = FakePlatform()
        let engine = SeqEngine(platform: platform)
        engine.open()
        #expect(platform.sent.count == 16, "Local Control Off on 16 channels")
        #expect(platform.sent.first == [0xB0, 122, 0])
        #expect(engine.portName(0) == "1: Test Out")

        let seq = engine.createSequence()
        #expect(seq.handle == 1)
        #expect(seq.ppq == 120)
        let stream = seq.createStream()
        #expect(stream.handle == 0x0100)
        #expect(seq.streams == [stream])
        stream.name = "Piano"
        #expect(stream.name == "Piano")

        #expect(stream.insert(.note(at: 480, key: 64, velocity: 100, duration: 60)) == 0)
        #expect(stream.insert(.note(at: 240, key: 60, velocity: 100, duration: 60)) == 0)
        #expect(stream.events.map(\.data1) == [60, 64])
        #expect(seq.positionText(at: 240) == "1:3:000")
        #expect(seq.tick(at: SeqPosition(measure: 2)) == 480)
        #expect(seq.position(at: 480) == SeqPosition(measure: 2))
        seq.insertTempo(9000, at: 480)
        #expect(seq.tempo(at: 480) == 9000)
        #expect(seq.tempo(at: 0) == 10000)
        seq.insertMeter(SeqMeter(numerator: 3, denominator: 4, key: -2), atMeasure: 3)
        #expect(seq.meter(at: 960) == SeqMeter(numerator: 3, denominator: 4, key: -2))
        stream.remove(at: 0)
        #expect(stream.count == 1)
        engine.close()
    }

    @Test func files() throws {
        let engine = SeqEngine()
        let song = try engine.loadCakewalkFile(songs.appendingPathComponent("JINGLE.WRK"))
        #expect(!song.streams.isEmpty)
        #expect(song.streams[0].count > 0)
        let out = FileManager.default.temporaryDirectory.appendingPathComponent("libseq-swift.mid")
        try song.saveMidiFile(out)
        let again = try engine.loadMidiFile(out)
        #expect(!again.streams.isEmpty)
        try? FileManager.default.removeItem(at: out)
        #expect(throws: SeqError.cannotLoad(fileError: 7)) {
            try engine.loadCakewalkFile(songs.appendingPathComponent("MISSING.WRK"))
        }
    }

    @Test func options() {
        final class NoLocalOff: SeqPlatform {
            var sent = 0
            var outputNames: [String] { ["A", "B"] }
            var milliseconds: UInt32 { 0 }
            func options(_ options: inout SeqOptions) { options.sendLocalOff = 0 }
            func send(port: Int, message: UnsafeBufferPointer<UInt8>) { sent += 1 }
            func startTimer(periodMs: UInt32) {}
            func stopTimer() {}
        }
        let platform = NoLocalOff()
        let engine = SeqEngine(platform: platform)
        engine.open()
        #expect(platform.sent == 0)
        #expect(engine.portName(1) == "2: B")
        engine.close()
    }
}
