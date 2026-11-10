import CLibSeq
import Foundation

/// A string the bridge writes into a buffer: it returns the whole length, so a second call gets
/// what did not fit the first time.
func bridgedString(_ fill: (UnsafeMutablePointer<CChar>, Int) -> Int) -> String {
    var buffer = [CChar](repeating: 0, count: 64)
    let length = buffer.withUnsafeMutableBufferPointer { fill($0.baseAddress!, $0.count) }
    if length >= buffer.count {
        buffer = [CChar](repeating: 0, count: length + 1)
        _ = buffer.withUnsafeMutableBufferPointer { fill($0.baseAddress!, $0.count) }
    }
    return buffer.withUnsafeBufferPointer { String(cString: $0.baseAddress!) }
}

/// The sequencer engine: the sequences it holds, their files, playback and MIDI. Each engine is
/// independent; the C interface (TTSSEQ.DLL's own, one engine per process) is not involved.
public final class SeqEngine {
    let cxx = libseq.SharedEngine()
    private var box: PlatformBox?

    /// TTSSEQ 1.01's version word, 0x0101.
    public static let version: UInt16 = 0x0101

    public init(platform: SeqPlatform? = nil) {
        self.platform = platform
        install()
    }

    deinit {
        cxx.close()
    }

    /// The host's MIDI ports, clock and timer. Set it before `open()`.
    public var platform: SeqPlatform? {
        didSet { install() }
    }

    private func install() {
        if let platform {
            let b = PlatformBox(platform)
            cxx.setPlatform(b.cxx)
            box = b
        } else {
            cxx.setPlatform(LIBSEQPLATFORM())
            box = nil
        }
    }

    // MARK: lifecycle, timer, input

    /// Reads the options and sends Local Control Off on every channel of every port.
    public func open() { cxx.open() }
    /// Stops the timer.
    public func close() { cxx.close() }
    /// True once when the playback has ended by itself; call it now and then while playing.
    public func idle() -> Bool { cxx.idle() }
    /// The host's timer: the engine sends what is due.
    public func timer() { cxx.timer() }
    /// The host's MIDI input: one message that came in.
    public func receive(_ message: [UInt8]) {
        message.withUnsafeBufferPointer { cxx.receive($0.baseAddress, $0.count) }
    }
    /// "1: device" for port 0.
    public func portName(_ port: Int) -> String { bridgedString { cxx.portName(Int32(port), $0, $1) } }

    // MARK: sequences

    public func createSequence() -> SeqSequence {
        SeqSequence(engine: self, handle: cxx.createSequence())
    }

    /// A sequence from a Cakewalk `.WRK` file.
    public func loadCakewalkFile(_ url: URL) throws -> SeqSequence {
        try loaded(url.path.withCString { cxx.loadCakewalkFile($0) })
    }

    /// A sequence from a Standard MIDI File.
    public func loadMidiFile(_ url: URL) throws -> SeqSequence {
        try loaded(url.path.withCString { cxx.loadMidiFile($0) })
    }

    private func loaded(_ handle: UInt16) throws -> SeqSequence {
        guard handle != 0 else { throw SeqError.cannotLoad(fileError: Int(cxx.fileError())) }
        return SeqSequence(engine: self, handle: handle)
    }

    // MARK: transport

    /// The sequence the transport plays; nil for none.
    public func setPlaying(_ sequence: SeqSequence?) throws {
        guard cxx.setPlayingSeq(sequence?.handle ?? 0) else { throw SeqError.rejected }
    }

    /// Plays the playing sequence from its Now; false when it plays already.
    @discardableResult public func play() -> Bool { cxx.playStart() }
    /// False when it does not play.
    @discardableResult public func stop() -> Bool { cxx.playStop() }
    public var isPlaying: Bool { !cxx.expired() }
}

/// A sequence: streams of events, a tempo map, a meter map, and Now.
public struct SeqSequence: Hashable {
    public let engine: SeqEngine
    /// The engine's handle: 1 for the first sequence.
    public let handle: UInt16

    public static func == (a: SeqSequence, b: SeqSequence) -> Bool {
        a.engine === b.engine && a.handle == b.handle
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(ObjectIdentifier(engine))
        hasher.combine(handle)
    }

    var cxx: libseq.SharedEngine { engine.cxx }

    public func destroy() { _ = cxx.destroySequence(handle) }

    public func saveCakewalkFile(_ url: URL) throws {
        guard url.path.withCString({ cxx.saveCakewalkFile(handle, $0) }) else { throw SeqError.cannotSave }
    }

    public func saveMidiFile(_ url: URL) throws {
        guard url.path.withCString({ cxx.saveMidiFile(handle, $0) }) else { throw SeqError.cannotSave }
    }

    /// A sequence parameter, as the engine numbers them.
    public func param(_ param: Int) -> Int32 { cxx.seqParam(handle, Int32(param)) }
    public func setParam(_ param: Int, _ value: Int32) throws {
        guard cxx.setSeqParam(handle, Int32(param), value) else { throw SeqError.rejected }
    }

    /// Ticks per quarter note, 48 to 480.
    public var ppq: UInt16 { cxx.ppq(handle) }
    public func setPPQ(_ ppq: UInt16) throws {
        guard cxx.setPpq(handle, ppq) else { throw SeqError.rejected }
    }

    /// The current position; while playing, the playback goes on from a new one.
    public var now: SeqTick {
        get { cxx.now(handle) }
        nonmutating set { _ = cxx.setNow(handle, newValue) }
    }

    /// The start of the last event.
    public var end: SeqTick { cxx.endTick(handle) }

    // MARK: tempo and meter

    /// The tempo at the tick, in bpm times 100.
    public func tempo(at tick: SeqTick) -> UInt16 { cxx.tempoAt(handle, tick) }
    /// Changes the tempo in effect at the tick.
    public func setTempo(_ tempo: UInt16, at tick: SeqTick) { _ = cxx.setTempo(handle, tick, tempo) }
    /// A new tempo from the tick on.
    public func insertTempo(_ tempo: UInt16, at tick: SeqTick) { _ = cxx.insertTempo(handle, tick, tempo) }

    public func meter(at tick: SeqTick) -> SeqMeter {
        let m = cxx.meterAt(handle, tick)
        return SeqMeter(numerator: m.numerator, denominator: UInt8(1) << m.denominatorLog2, key: m.key)
    }

    /// A new meter from the measure (from 1) on.
    public func insertMeter(_ meter: SeqMeter, atMeasure measure: UInt16) {
        _ = cxx.insertMeter(handle, measure, meter.key, meter.numerator, meter.denominatorLog2)
    }

    public func position(at tick: SeqTick) -> SeqPosition { SeqPosition(cxx.toMBT(handle, tick)) }
    public func tick(at position: SeqPosition) -> SeqTick { cxx.toTick(handle, position.cxx) }
    /// "2:1:033".
    public func positionText(at tick: SeqTick) -> String {
        bridgedString { cxx.formatPosition(handle, tick, $0, $1) }
    }

    // MARK: streams

    public var streams: [SeqStream] {
        (0..<cxx.streamCount(handle)).map { SeqStream(engine: engine, handle: cxx.streamAt(handle, $0)) }
    }

    public func createStream() -> SeqStream {
        SeqStream(engine: engine, handle: cxx.createStream(handle))
    }
}

/// A stream: events in time order, a name and parameters.
public struct SeqStream: Hashable {
    public let engine: SeqEngine
    /// The engine's handle: the sequence in the high byte.
    public let handle: UInt16

    public static func == (a: SeqStream, b: SeqStream) -> Bool {
        a.engine === b.engine && a.handle == b.handle
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(ObjectIdentifier(engine))
        hasher.combine(handle)
    }

    var cxx: libseq.SharedEngine { engine.cxx }

    public func destroy() { _ = cxx.destroyStream(handle) }

    public var name: String {
        get { bridgedString { cxx.streamName(handle, $0, $1) } }
        nonmutating set { _ = newValue.withCString { cxx.setStreamName(handle, $0) } }
    }

    /// A stream parameter, as the engine numbers them (5 patch, 6 volume, 10 pan, ...).
    public func param(_ param: Int) -> Int32 { cxx.streamParam(handle, Int32(param)) }
    /// A patch, volume or pan also goes out at once.
    public func setParam(_ param: Int, _ value: Int32) throws {
        guard cxx.setStreamParam(handle, Int32(param), value) else { throw SeqError.rejected }
    }

    public var count: Int { cxx.eventCount(handle) }

    public func event(at index: Int) -> SeqEvent { SeqEvent(cxx.eventAt(handle, index)) }

    public var events: [SeqEvent] { (0..<count).map(event(at:)) }

    /// Inserts the event after those at or before its time; returns its index.
    @discardableResult public func insert(_ event: SeqEvent) -> Int {
        cxx.insertEvent(handle, event.cxx)
    }

    public func remove(at index: Int) { _ = cxx.removeEvent(handle, index) }
}
