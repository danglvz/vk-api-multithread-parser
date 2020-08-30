

#ifndef VK_PARSER_MULTITHREAD_H
#define VK_PARSER_MULTITHREAD_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include "using_openssl.h"
#include "requests_maker.h"

namespace vk_parse {

    template<typename TYPE>
    class thread_safe_queue {
        std::queue<TYPE> queue;
        std::mutex mut;
        std::condition_variable cond;
    public:
        bool empty() {
            std::scoped_lock<std::mutex> sl(mut);
            return queue.empty();
        }

        void push(TYPE item) {
            std::scoped_lock<std::mutex> sl(mut);
            queue.push(std::move(item));
            cond.notify_one();
        }

        void wait_and_pop(TYPE &output) {
            std::unique_lock<std::mutex> ul(mut);
            cond.wait(ul, [this](){  return !this->queue.empty(); });
            output = std::move(queue.front());
            queue.pop();
        }

        bool try_pop(TYPE &output){
            std::scoped_lock<std::mutex> sl(mut);
            if ( !queue.empty() ) {
                output = std::move(queue.front());
                queue.pop();
                return true;
            }
            else {
                return false;
            }
        }
    };
/*
    class requests_pool{
        thread_safe_queue<std::string(simple_https::https_client)> req_queue;
        std::vector<std::thread> threads;
        std::string hostname_, output_file_name_;
        std::atomic_bool done;
    public:
        explicit requests_pool(std::size_t num_of_threads, const std::string &hostname);
        void submit(const std::string &request);
        ~requests_pool();
    };
*/


    class requests_pool{
        thread_safe_queue<std::function<void(simple_https::https_client&)>> task_queue;
        std::vector<std::thread> threads;
        std::atomic_bool done;
    public:
        explicit requests_pool(int threads_num, const std::string &group_id);
        std::future<std::string> submit(std::function<std::string(simple_https::https_client&)> func);
        ~requests_pool();
    };

    class thread_pool{
        thread_safe_queue<std::function<void()>> work_queue;
        std::vector<std::thread> threads;
        bool done;
    public:
        explicit thread_pool(int workers) : done(false) {
            for ( int i{}; i < workers; ++i ) {
                threads.emplace_back([this](){
                    while(!this->done) {
                        std::function<void()> task;
                        if (this->work_queue.try_pop(task) ) {
                            task();
                        } else {
                            std::this_thread::yield();
                        }
                    }
                });
            }
        }

        ~thread_pool() {
            done = true;
            for ( auto &thread: threads ) thread.join();
        }


        template<typename FuncType, typename ArgType>
        auto submit(FuncType f, ArgType arg){
            using result_type = typename std::result_of<FuncType(ArgType)>::type;
            auto task = std::make_shared<std::packaged_task<result_type()>>(
                    std::bind(std::forward<FuncType>(f),std::forward<ArgType>(arg)));
            std::future<result_type> res = task->get_future();
            work_queue.push([task](){ (*task)(); });
            return res;
        }
    };
}



#endif //VK_PARSER_MULTITHREAD_H
