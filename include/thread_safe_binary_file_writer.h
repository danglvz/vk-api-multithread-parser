
#ifndef VK_PARSER_THREAD_SAFE_BINARY_FILE_WRITER_H
#define VK_PARSER_THREAD_SAFE_BINARY_FILE_WRITER_H

#include <fstream>
#include <vector>
#include <iterator>
#include <array>
#include <mutex>

namespace vk_parse{
    class binary_file_writer{
        std::ofstream out;
        std::mutex mut;
    public:
        explicit binary_file_writer(const std::string &filename);
        void write(const std::vector<std::uint32_t> &vec, const std::array<int, 2> &post);
        
    };
}


#endif //VK_PARSER_THREAD_SAFE_BINARY_FILE_WRITER_H
