
#include "thread_safe_binary_file_writer.h"


using namespace vk_parse;

vk_parse::binary_file_writer::binary_file_writer(const std::string &filename) : out(filename, std::ios::out | std::ios::trunc) {}

void vk_parse::binary_file_writer::write(const std::vector<std::uint32_t> &vec, const std::array<int, 2> &post) {
    const std::uint32_t id(post[0]), size(vec.size()), nul{};
    std::scoped_lock<std::mutex> scopedLock(mut);
    out.write(reinterpret_cast<const char *>(&id), sizeof(id));
    out.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for (auto &i :vec) out.write(reinterpret_cast<const char *>(&i), sizeof(i));
    out.write(reinterpret_cast<const char *>(&nul), sizeof(nul));
}

