//
// Created by dd on 29.06.2020.
//

#include "multitreading.h"
#include <utility>
#include "custom_http_client.h"



thread_creator::thread_creator(const std::string &owner_id, int num) : parse(owner_id), done(false) {
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
    http_client httpClient {};
    for (;;) {
        std::array<int, 2> post {};
        {
            std::unique_lock<std::mutex> ul(mut);
            cond.wait(ul, [this](){ return !queue.empty() || done ;});
            if ( queue.empty() && done ) return;
            post = std::move(queue.front());
            queue.pop();
        }
        parse.parse_likes(post, httpClient);
    }
}

thread_creator::~thread_creator() {
    done = true;
    cond.notify_all();
    for ( auto &thread : threads ) {
        thread.join();
    }
    std::cout << "DONE!\n";
}

void thread_creator::start(int count) {
    http_client httpClient{};
    std::cout << "parsing....\n";
    for ( int  offset = 0; offset <= count; offset += 100 ) {
        for ( auto &post : parse.parse_posts(offset, httpClient)) {
            if ( post[1] != 0 ) {
                std::unique_lock<std::mutex> ul(mut);
                queue.push(post);
            }
            cond.notify_one();
        }
    }

}
