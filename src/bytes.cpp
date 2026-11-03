// libseq: whole-file reading and writing for the file formats.
#include "bytes.hpp"

#include <fstream>
#include <iterator>

namespace libseq::bytes {

std::vector<std::uint8_t> readFileBytes(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw Error("cannot open " + path);
    }
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void writeFileBytes(const std::string& path, const std::vector<std::uint8_t>& bytes) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw Error("cannot create " + path);
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast): byte buffer passed to a char stream
    out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
}

} // namespace libseq::bytes
