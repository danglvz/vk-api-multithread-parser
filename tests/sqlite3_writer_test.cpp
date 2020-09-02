#include <gtest/gtest.h>
#include "sqlite3_writer.h"


TEST(sqlite3_writer_tests, test_1){
    vk_parse::sqlite::sqlite3_writer sql_w("test.db", "777");
    
    sql_w.create_new_group_table();

    std::vector<std::string> test_vec{"111111111", "222222222", "333333333"};
    std::array<int, 2> test_post_info{1010, 100}; 

    sql_w.write_likes_ids(test_vec, test_post_info);

}