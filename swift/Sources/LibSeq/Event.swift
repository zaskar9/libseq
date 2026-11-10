import CLibSeq

/// A position in ticks from the start of a sequence.
public typealias SeqTick = UInt32

/// An event: the 8-byte record of a stream. A time (24 bits are kept), a MIDI status with its
/// channel (or 0xF0 for a sysex bank event, or a string event's kind), two data bytes, and a
/// duration for notes.
public struct SeqEvent: Equatable, Hashable, Sendable {
    public var time: SeqTick
    public var status: UInt8
    public var data1: UInt8
    public var data2: UInt8
    public var duration: UInt16

    public init(time: SeqTick, status: UInt8, data1: UInt8 = 0, data2: UInt8 = 0, duration: UInt16 = 0) {
        self.time = time
        self.status = status
        self.data1 = data1
        self.data2 = data2
        self.duration = duration
    }

    /// A note on channel 0 to 15.
    public static func note(at time: SeqTick, channel: UInt8 = 0, key: UInt8, velocity: UInt8,
                            duration: UInt16) -> SeqEvent {
        SeqEvent(time: time, status: 0x90 | (channel & 0x0F), data1: key, data2: velocity, duration: duration)
    }

    init(_ e: libseq.Event) {
        self.init(time: e.time, status: e.status, data1: e.data1, data2: e.data2, duration: e.duration)
    }

    var cxx: libseq.Event {
        var e = libseq.Event()
        e.time = time
        e.status = status
        e.data1 = data1
        e.data2 = data2
        e.duration = duration
        return e
    }
}

/// A position as measure, beat and tick, from 1, 1, 0.
public struct SeqPosition: Equatable, Hashable, Sendable {
    public var measure: UInt16
    public var beat: UInt16
    public var tick: UInt16

    public init(measure: UInt16, beat: UInt16 = 1, tick: UInt16 = 0) {
        self.measure = measure
        self.beat = beat
        self.tick = tick
    }

    init(_ p: libseq.MBT) {
        self.init(measure: p.measure, beat: p.beat, tick: p.tick)
    }

    var cxx: libseq.MBT {
        var p = libseq.MBT()
        p.measure = measure
        p.beat = beat
        p.tick = tick
        return p
    }
}

/// A meter and key from the meter map.
public struct SeqMeter: Equatable, Hashable, Sendable {
    public var numerator: UInt8
    /// 4 for quarters, 8 for eighths.
    public var denominator: UInt8
    /// Sharps, or flats when negative.
    public var key: Int8

    public init(numerator: UInt8, denominator: UInt8, key: Int8 = 0) {
        self.numerator = numerator
        self.denominator = denominator
        self.key = key
    }

    var denominatorLog2: UInt8 {
        UInt8(max(denominator, 1).trailingZeroBitCount)
    }
}

/// What went wrong.
public enum SeqError: Error, Equatable {
    /// The file could not be loaded; the code is the engine's file error (7: not found).
    case cannotLoad(fileError: Int)
    case cannotSave
    /// The engine does not take the value.
    case rejected
}
