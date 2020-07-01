//
// Created by dd on 29.06.2020.
//

#ifndef UNTITLED7_MULTITREADING_H
#define UNTITLED7_MULTITREADING_H

#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include "custom_http_client.h"
#include <atomic>
#include "prarse_from_string.h"

template<typename T>
class treadsafe_queue {
    mutable std::mutex mutex;
    std::queue<T> my_queue;
public:
    explicit treadsafe_queue() = default;
    void push(T new_value) {
        std::scoped_lock<std::mutex> sl(mutex);
        my_queue.push(std::move(new_value));
    }
    void push(T &&new_value) {
        std::scoped_lock<std::mutex> sl(mutex);
        my_queue.push(new_value);
    }
    bool pop(T& value){
        std::scoped_lock<std::mutex> sl(mutex);
        if ( my_queue.empty() ) return false;
        value = std::move(my_queue.front());
        my_queue.pop();
        return true;
    }
    bool isEmpty() const{
        std::scoped_lock<std::mutex> sl(mutex);
        return my_queue.empty();
    }
};

class threads_joiner{
    std::vector<std::thread> &threads;
public:
    explicit threads_joiner(std::vector<std::thread> &threads_) : threads(threads_) {}
    ~threads_joiner() {
        for (auto &thread : threads) {
            if (thread.joinable()) thread.join();
        }
    }
};


class thread_creator{
    treadsafe_queue<std::array<int, 2>> queue;
    std::vector<std::thread> threads;
    parse_from_api parse;
    threads_joiner joiner;
    std::atomic_bool done;
    void work();
public:
    explicit thread_creator(const std::string &owner_id, int num);
    ~thread_creator();
    void start();
    void submit(std::array<int, 2> &func);
};






#endif //UNTITLED7_MULTITREADING_H
