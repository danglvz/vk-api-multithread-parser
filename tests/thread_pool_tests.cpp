


#include <gtest/gtest.h>
#include "multithread.h"


TEST(thread_pool_test, test_1) {
    vk_parse::thread_pool threadPool{10};
    std::vector<int> expected_results;
    std::vector<std::future<int>> results;
    for (int i{}; i < 60; ++i) {
        expected_results.emplace_back(i*2);
        results.emplace_back(threadPool.submit([](int a){return a*2;}, i));
    }
    for ( int i{}; i < 60; ++i) {
        ASSERT_EQ(expected_results[i], results[i].get());
    }
}



