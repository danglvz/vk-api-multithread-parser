//
// Created by dd on 29.06.2020.
//

#include "multitreading.h"

#include <utility>
#include "custom_http_client.h"





thread_creator::thread_creator(const std::string &owner_id, int num) : parse(owner_id), done(false), joiner(threads) {
    try {
        for (int i = 0; i < num; ++i) {
            threads.emplace_back(&thread_creator::work, this);
        }
    } catch (...) {
        done = true;
        throw;
    }

}

void thread_creator::submit(std::array<int, 2> &func) {
    queue.push(func);
}

void thread_creator::work() {
    http_client httpClient{};
    std::array<int, 2> post{};
    while (!done) {
        if (queue.pop(post)) {
            parse.parse_likes(post, httpClient);
        } else {
            std::this_thread::yield();
        }
    }
}

thread_creator::~thread_creator() {
    done = true;
}

void thread_creator::start() {
    http_client httpClient{};
    for ( int  offset = 0; offset < 1000; offset += 100 ) {
        for ( auto post : parse.parse_posts(offset, httpClient)) queue.push(post);
    }
    while ( !queue.isEmpty() ) std::this_thread::sleep_for(std::chrono::seconds(5));
}
