// A libseq::Platform made of C function pointers (LIBSEQPLATFORM): the C interface installs one,
// and so does SharedEngine, for hosts such as Swift that can hand over C functions.
#pragma once

#include "libseq/engine.hpp"
#include "libseq/ttsseq.h"

#include <cstdint>
#include <span>
#include <string>

namespace libseq {

class CPlatform final : public Platform {
public:
    void install(const LIBSEQPLATFORM* p) noexcept {
        installed_ = p != nullptr;
        p_ = installed_ ? *p : LIBSEQPLATFORM{};
    }
    [[nodiscard]] bool installed() const noexcept { return installed_; }
    [[nodiscard]] const LIBSEQPLATFORM& c() const noexcept { return p_; }

    [[nodiscard]] int outputPorts() const override {
        return p_.outputPorts != nullptr ? p_.outputPorts(p_.context) : 0;
    }
    [[nodiscard]] std::string outputName(const int port) const override {
        const char* n = p_.outputName != nullptr ? p_.outputName(p_.context, port) : nullptr;
        return n != nullptr ? std::string(n) : std::string();
    }
    [[nodiscard]] Options options() const override {
        Options o;
        if (p_.options == nullptr) {
            return o;
        }
        LIBSEQOPTIONS c{o.syncOutputPort,  o.sendLocalOff,       o.timebase,           o.timerPeriod,
                        o.timerResolution, o.sysxSendPacketSize, o.sysxReceiveBuffers, o.sysxReceiveBuffersSize,
                        o.chaseDelay,      o.flywheelAmount};
        p_.options(p_.context, &c);
        o.syncOutputPort = c.syncOutputPort;
        o.sendLocalOff = c.sendLocalOff;
        o.timebase = c.timebase;
        o.timerPeriod = c.timerPeriod;
        o.timerResolution = c.timerResolution;
        o.sysxSendPacketSize = c.sysxSendPacketSize;
        o.sysxReceiveBuffers = c.sysxReceiveBuffers;
        o.sysxReceiveBuffersSize = c.sysxReceiveBuffersSize;
        o.chaseDelay = c.chaseDelay;
        o.flywheelAmount = c.flywheelAmount;
        return o;
    }
    void send(const int port, const std::span<const std::uint8_t> message) override {
        if (p_.send != nullptr) {
            p_.send(p_.context, port, message.data(), message.size());
        }
    }
    [[nodiscard]] std::uint32_t milliseconds() const override {
        return p_.milliseconds != nullptr ? p_.milliseconds(p_.context) : 0;
    }
    void opened(Engine& /*engine*/) override {
        if (p_.opened != nullptr) {
            p_.opened(p_.context);
        }
    }
    void closed() override {
        if (p_.closed != nullptr) {
            p_.closed(p_.context);
        }
    }
    void startTimer(const unsigned periodMs) override {
        if (p_.startTimer != nullptr) {
            p_.startTimer(p_.context, periodMs);
        }
    }
    void stopTimer() override {
        if (p_.stopTimer != nullptr) {
            p_.stopTimer(p_.context);
        }
    }
    void wait() override {
        if (p_.wait != nullptr) {
            p_.wait(p_.context);
        }
    }
    void postMessage(void* hwnd, const std::uint16_t message, const std::uint16_t wParam,
                     const std::uint32_t lParam) const {
        if (p_.postMessage != nullptr) {
            p_.postMessage(p_.context, hwnd, message, wParam, lParam);
        }
    }

private:
    LIBSEQPLATFORM p_{};
    bool installed_ = false;
};

} // namespace libseq
