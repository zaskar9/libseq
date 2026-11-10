import CLibSeq

#if canImport(Darwin)
import Darwin
#elseif canImport(Glibc)
import Glibc
#endif

/// TTSSEQ.INI's [Options]; the engine passes 1.01's defaults.
public struct SeqOptions: Equatable, Sendable {
    /// The port (from 1) the Song Position goes to.
    public var syncOutputPort: Int
    /// 0 for no Local Control Off when the engine opens.
    public var sendLocalOff: Int
    public var timebase: Int
    /// Milliseconds.
    public var timerPeriod: UInt32
    public var timerResolution: UInt32
    public var sysxSendPacketSize: UInt32
    public var sysxReceiveBuffers: UInt32
    public var sysxReceiveBuffersSize: UInt32
    /// Tenths of a second.
    public var chaseDelay: UInt32
    public var flywheelAmount: UInt32

    init(_ o: LIBSEQOPTIONS) {
        syncOutputPort = Int(o.syncOutputPort)
        sendLocalOff = Int(o.sendLocalOff)
        timebase = Int(o.timebase)
        timerPeriod = UInt32(o.timerPeriod)
        timerResolution = UInt32(o.timerResolution)
        sysxSendPacketSize = UInt32(o.sysxSendPacketSize)
        sysxReceiveBuffers = UInt32(o.sysxReceiveBuffers)
        sysxReceiveBuffersSize = UInt32(o.sysxReceiveBuffersSize)
        chaseDelay = UInt32(o.chaseDelay)
        flywheelAmount = UInt32(o.flywheelAmount)
    }

    func write(to o: inout LIBSEQOPTIONS) {
        o.syncOutputPort = Int32(syncOutputPort)
        o.sendLocalOff = Int32(sendLocalOff)
        o.timebase = Int32(timebase)
        o.timerPeriod = timerPeriod
        o.timerResolution = timerResolution
        o.sysxSendPacketSize = sysxSendPacketSize
        o.sysxReceiveBuffers = sysxReceiveBuffers
        o.sysxReceiveBuffersSize = sysxReceiveBuffersSize
        o.chaseDelay = chaseDelay
        o.flywheelAmount = flywheelAmount
    }
}

/// The MIDI ports, the clock and the timer of the host. While the engine is open the host calls
/// `SeqEngine.timer()` every timer period, and `SeqEngine.receive(_:)` for every message that
/// comes in.
public protocol SeqPlatform: AnyObject {
    /// The devices of the output ports, from port 0.
    var outputNames: [String] { get }
    /// TTSSEQ.INI's [Options]: change what the host has. The default keeps 1.01's.
    func options(_ options: inout SeqOptions)
    /// A short message of 1 to 3 bytes, or a sysex message F0 ... F7, to send at once.
    func send(port: Int, message: UnsafeBufferPointer<UInt8>)
    /// A clock in milliseconds that only goes forward.
    var milliseconds: UInt32 { get }
    /// The engine opens and closes: start and stop the input.
    func opened()
    func closed()
    /// From now on call `SeqEngine.timer()` every `periodMs` milliseconds.
    func startTimer(periodMs: UInt32)
    func stopTimer()
    /// The engine waits in a loop (count-in, sync, sysex): let a little time pass.
    func wait()
}

extension SeqPlatform {
    public func options(_ options: inout SeqOptions) {}
    public func opened() {}
    public func closed() {}
    public func wait() {}
}

/// The platform behind the C function pointers the engine calls.
final class PlatformBox {
    let platform: SeqPlatform
    private var name: UnsafeMutablePointer<CChar>?

    init(_ platform: SeqPlatform) {
        self.platform = platform
    }

    deinit {
        free(name)
    }

    /// The device of a port, kept until the next call.
    func outputName(_ port: Int) -> UnsafePointer<CChar>? {
        let names = platform.outputNames
        guard port >= 0 && port < names.count else { return nil }
        free(name)
        name = strdup(names[port])
        return UnsafePointer(name)
    }

    static func from(_ context: UnsafeMutableRawPointer?) -> PlatformBox {
        Unmanaged<PlatformBox>.fromOpaque(context!).takeUnretainedValue()
    }

    var cxx: LIBSEQPLATFORM {
        var p = LIBSEQPLATFORM()
        p.context = Unmanaged.passUnretained(self).toOpaque()
        p.outputPorts = { c in Int32(PlatformBox.from(c).platform.outputNames.count) }
        p.outputName = { c, port in PlatformBox.from(c).outputName(Int(port)) }
        p.options = { c, o in
            guard let o else { return }
            var options = SeqOptions(o.pointee)
            PlatformBox.from(c).platform.options(&options)
            options.write(to: &o.pointee)
        }
        p.send = { c, port, message, length in
            guard let message else { return }
            PlatformBox.from(c).platform.send(port: Int(port),
                                              message: UnsafeBufferPointer(start: message, count: Int(length)))
        }
        p.milliseconds = { c in PlatformBox.from(c).platform.milliseconds }
        p.opened = { c in PlatformBox.from(c).platform.opened() }
        p.closed = { c in PlatformBox.from(c).platform.closed() }
        p.startTimer = { c, ms in PlatformBox.from(c).platform.startTimer(periodMs: UInt32(ms)) }
        p.stopTimer = { c in PlatformBox.from(c).platform.stopTimer() }
        p.wait = { c in PlatformBox.from(c).platform.wait() }
        return p
    }
}
