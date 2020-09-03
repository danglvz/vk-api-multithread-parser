#ifndef BINARY_READER_H
#define BINARY_READER_H
#include <iostream>
#include "fstream"
#include <vector>

class binary_file_reader{
    std::ifstream in;
public:
    explicit binary_file_reader(const std::string &filename);
    ~binary_file_reader();
    std::vector<std::uint32_t> read_to_vector();
};

#endif //BINARY_READER_H