// libseq: byte-level helpers shared by the readers and writers (not installed): narrowing to the
// integer sizes the file formats use, little-endian assembly, a byte cursor (Reader), a byte
// builder (Writer), and whole-file read/write.
#pragma once

#include "libseq/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace libseq::bytes {

/// Explicit narrowing to the byte-sized integer types used in the file formats.
template <class T>
[[nodiscard]] constexpr std::uint8_t u8(const T v) noexcept {
    return static_cast<std::uint8_t>(v);
}
template <class T>
[[nodiscard]] constexpr std::uint8_t s8(const T v) noexcept {
    return static_cast<std::uint8_t>(static_cast<std::int8_t>(v));
}
template <class T>
[[nodiscard]] constexpr std::uint16_t u16(const T v) noexcept {
    return static_cast<std::uint16_t>(v);
}
template <class T>
[[nodiscard]] constexpr std::uint32_t u32(const T v) noexcept {
    return static_cast<std::uint32_t>(v);
}

/// Assembles up to four bytes, least significant first, without signed integer promotion.
[[nodiscard]] constexpr std::uint32_t le(const std::uint8_t b0, const std::uint8_t b1, const std::uint8_t b2,
                                         const std::uint8_t b3) noexcept {
    return std::uint32_t{b0} | (std::uint32_t{b1} << 8) | (std::uint32_t{b2} << 16) | (std::uint32_t{b3} << 24);
}

/// Reads little-endian (WRK) and big-endian (SMF) integers from a byte range with bounds checks.
class Reader {
public:
    Reader(const std::uint8_t* data, const std::size_t size) : d_(data), n_(size) {}
    explicit Reader(const std::vector<std::uint8_t>& v) : d_(v.data()), n_(v.size()) {}

    [[nodiscard]] std::size_t pos() const noexcept { return p_; }
    [[nodiscard]] std::size_t size() const noexcept { return n_; }
    [[nodiscard]] std::size_t remaining() const noexcept { return n_ - p_; }
    [[nodiscard]] bool atEnd() const noexcept { return p_ >= n_; }
    [[nodiscard]] const std::uint8_t* ptr() const noexcept { return d_ + p_; }
    void skip(const std::size_t k) {
        need(k);
        p_ += k;
    }

    std::uint8_t u8() {
        need(1);
        return d_[p_++];
    }
    std::int8_t s8() { return static_cast<std::int8_t>(u8()); }
    std::uint16_t u16() {
        need(2);
        const auto v = static_cast<std::uint16_t>(d_[p_] | (d_[p_ + 1] << 8));
        p_ += 2;
        return v;
    }
    std::int16_t s16() { return static_cast<std::int16_t>(u16()); }
    std::uint32_t u24() {
        need(3);
        const std::uint32_t v = le(d_[p_], d_[p_ + 1], d_[p_ + 2], 0);
        p_ += 3;
        return v;
    }
    std::uint32_t u32() {
        need(4);
        const std::uint32_t v = le(d_[p_], d_[p_ + 1], d_[p_ + 2], d_[p_ + 3]);
        p_ += 4;
        return v;
    }
    std::uint16_t be16() {
        need(2);
        const auto v = static_cast<std::uint16_t>((d_[p_] << 8) | d_[p_ + 1]);
        p_ += 2;
        return v;
    }
    std::uint32_t be32() {
        need(4);
        const std::uint32_t v = le(d_[p_ + 3], d_[p_ + 2], d_[p_ + 1], d_[p_]);
        p_ += 4;
        return v;
    }
    std::vector<std::uint8_t> bytes(const std::size_t k) {
        need(k);
        std::vector<std::uint8_t> v(d_ + p_, d_ + p_ + k);
        p_ += k;
        return v;
    }
    std::string str(const std::size_t k) {
        need(k);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast): byte buffer viewed as characters
        std::string s(reinterpret_cast<const char*>(d_ + p_), k);
        p_ += k;
        return s;
    }
    /// Pascal string: one length byte followed by the characters.
    std::string pstr() {
        const std::size_t k = u8();
        return str(k);
    }
    template <std::size_t N>
    void array(std::array<std::uint8_t, N>& a) {
        need(N);
        std::memcpy(a.data(), d_ + p_, N);
        p_ += N;
    }
    /// SMF variable-length quantity.
    std::uint32_t vlq() {
        std::uint32_t v = 0;
        for (int i = 0; i < 4; ++i) {
            const std::uint8_t c = u8();
            v = (v << 7) | (c & 0x7fU);
            if ((c & 0x80U) == 0) {
                return v;
            }
        }
        throw Error("variable-length quantity too long");
    }

private:
    void need(const std::size_t k) const {
        if (p_ + k > n_) {
            throw Error("unexpected end of data at offset " + std::to_string(p_));
        }
    }
    const std::uint8_t* d_;
    std::size_t n_;
    std::size_t p_ = 0;
};

/// Appends little-endian (WRK) and big-endian (SMF) integers to a byte vector.
class Writer {
public:
    std::vector<std::uint8_t> out;

    [[nodiscard]] std::size_t size() const noexcept { return out.size(); }
    void u8(const std::uint32_t v) { out.push_back(bytes::u8(v)); }
    void s8(const int v) { out.push_back(bytes::s8(v)); }
    void u16(const std::uint32_t v) {
        u8(v);
        u8(v >> 8);
    }
    void s16(const int v) { u16(bytes::u16(static_cast<std::int16_t>(v))); }
    void u24(const std::uint32_t v) {
        u8(v);
        u8(v >> 8);
        u8(v >> 16);
    }
    void u32(const std::uint32_t v) {
        u8(v);
        u8(v >> 8);
        u8(v >> 16);
        u8(v >> 24);
    }
    void be16(const std::uint32_t v) {
        u8(v >> 8);
        u8(v);
    }
    void be32(const std::uint32_t v) {
        u8(v >> 24);
        u8(v >> 16);
        u8(v >> 8);
        u8(v);
    }
    void bytes(const std::vector<std::uint8_t>& v) { out.insert(out.end(), v.begin(), v.end()); }
    void bytes(const std::uint8_t* p, const std::size_t k) { out.insert(out.end(), p, p + k); }
    void str(const std::string& s) { out.insert(out.end(), s.begin(), s.end()); }
    void pstr(const std::string& s) {
        if (s.size() > 255) {
            throw Error("string too long for a Pascal string");
        }
        u8(bytes::u32(s.size()));
        str(s);
    }
    template <std::size_t N>
    void array(const std::array<std::uint8_t, N>& a) {
        out.insert(out.end(), a.begin(), a.end());
    }
    void vlq(std::uint32_t v) {
        std::array<std::uint8_t, 5> buf{};
        std::size_t n = 0;
        do {
            buf[n++] = bytes::u8(v & 0x7fU);
            v >>= 7;
        } while (v != 0);
        while (n-- > 0) {
            u8(buf[n] | (n != 0 ? 0x80U : 0U));
        }
    }
    void patchU32(const std::size_t at, const std::uint32_t v) {
        out[at] = bytes::u8(v);
        out[at + 1] = bytes::u8(v >> 8);
        out[at + 2] = bytes::u8(v >> 16);
        out[at + 3] = bytes::u8(v >> 24);
    }
};

[[nodiscard]] std::vector<std::uint8_t> readFileBytes(const std::string& path);
void writeFileBytes(const std::string& path, const std::vector<std::uint8_t>& bytes);

} // namespace libseq::bytes
