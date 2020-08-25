#include "multithread.h"
#include <gtest/gtest.h>
#include <thread>



TEST(queue_tests, Sync_test_empty) {
    vk_parse::thread_safe_queue<int> queue;
    ASSERT_TRUE(queue.empty());
    queue.push(1);
    ASSERT_FALSE(queue.empty());
}

TEST(queue_tests, Sync_test_try_pop) {
    vk_parse::thread_safe_queue<int> queue;
    int temp{};
    ASSERT_FALSE(queue.try_pop(temp));
    queue.push(9);
    ASSERT_TRUE(queue.try_pop(temp));
    ASSERT_EQ(9, temp);
}

TEST(queue_tests, Sync_test_wait_and_pop) {
    vk_parse::thread_safe_queue<int> queue;
    int expecting_value{2}, result{};
    queue.push(expecting_value);
    queue.wait_and_pop(result);
    ASSERT_EQ(expecting_value, result);
}


TEST(queue_tests, Async_test_wait_and_pop) {
    vk_parse::thread_safe_queue<int> queue;
    int value{-1}, expected_value_1{-1}, expected_value_2{3};
    std::thread popping_thread([&queue, &value](){ queue.wait_and_pop(value); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(expected_value_1, value);
    queue.push(expected_value_2);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(expected_value_2, value);
    popping_thread.join();
}


TEST(queue_tests, Async_test_try_pop) {
    vk_parse::thread_safe_queue<int> queue;
    int value{-1}, expected_value_1{-1}, expected_value_2{3};
    bool isPopped{};
    std::thread popping_thread([&queue, &value, &isPopped](){
        while (!isPopped) isPopped = queue.try_pop(value);
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(expected_value_1, value);
    ASSERT_FALSE(isPopped);
    queue.push(expected_value_2);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(expected_value_2, value);
    ASSERT_TRUE(isPopped);
    popping_thread.join();
}

TEST(queue_tests, test1) {
    vk_parse::thread_safe_queue<int> queue;
    std::vector<int> expecting_result{1,2,3,4,5,6,7,7,8,9,0}, result;
    std::thread pusing_thread([&queue, &expecting_result](){
        for ( int i{}; i < expecting_result.size(); ++i ) {
            queue.push(expecting_result[i]);
        }
    });
    std::thread popper_thread([&]() {
        int temp{};
        for ( int i{}; i < expecting_result.size(); ++i ) {
            queue.wait_and_pop(temp);
            result.emplace_back(temp);
        }
    });
    pusing_thread.join();
    popper_thread.join();
    ASSERT_EQ(expecting_result, result);

}

