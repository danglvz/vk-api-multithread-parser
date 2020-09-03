#include "binary_reader.h"

binary_file_reader::binary_file_reader(const std::string &filename) : in(filename, std::ios::in) {}

std::vector<std::uint32_t> binary_file_reader::read_to_vector() {
    in.seekg(0, std::ios_base::end);
    std::size_t size(in.tellg());
    in.seekg(0, std::ios_base::beg);
    std::vector<std::uint32_t> output(size/sizeof(std::uint32_t));
    in.read(reinterpret_cast<char *>(&output[0]), size);
    return output;
}

binary_file_reader::~binary_file_reader() {
    in.close();
}
