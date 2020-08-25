
#include <gtest/gtest.h>
#include "requests_maker.h"


TEST(binary_file_writer_test, test_1){
    std::string filename("temp_file");
    std::fstream file(filename, std::ios::out | std::ios::trunc);
    file.close();
    vk_parse::posts_item expected_post{1,3}, result_post{};
    std::vector<uint32_t> expected_result_vec{111,222,333}, result_vec;
    {
        vk_parse::binary_file_writer binaryFileWriter(filename);
        binaryFileWriter.write(expected_result_vec, expected_post);
    }
    std::ifstream file_1(filename, std::ios::binary);
    file_1.read(reinterpret_cast<char*>(&result_post[0]), sizeof(std::uint32_t));
    file_1.read(reinterpret_cast<char*>(&result_post[1]), sizeof(std::uint32_t));
    for(std::uint32_t temp{};;){
        file_1.read(reinterpret_cast<char*>(&temp), sizeof(std::uint32_t));
        if ( !temp ) break;
        else result_vec.emplace_back(temp);
    }
    ASSERT_EQ(expected_result_vec, result_vec);
    ASSERT_EQ(expected_post, result_post);
}

