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
#include <condition_variable>
#include "prarse_from_string.h"



class thread_creator{
    std::queue<std::array<int, 2>> queue;
    std::vector<std::thread> threads;
    parse_from_api parse;
    std::atomic_bool done;
    std::mutex mut;
    std::condition_variable cond;
    void work();
public:
    explicit thread_creator(const std::string &owner_id, int num);
    ~thread_creator();
    void start(int count);
    void submit(std::array<int, 2> &func);
};






#endif //UNTITLED7_MULTITREADING_H
