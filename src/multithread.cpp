



#include "multithread.h"
#include "using_openssl.h"


using namespace vk_parse;

requests_pool::requests_pool(int threads_num, const std::string &group_id):
done(false),
requestsMaker(group_id, group_id + ".bin")
{
    for (int i{}; i < threads_num; ++i ){
        threads.emplace_back([this](){
            simple_https::https_client httpsClient("api.vk.com");
            while(!this->done){
                posts_item post;
                if ( this->posts_queue.try_pop(post) ){
                    requestsMaker.parse_who_liked(post, httpsClient);
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
}

void requests_pool::submit(posts_item &post) {
    posts_queue.push(post);
}

requests_pool::~requests_pool() {
    done = true;
    for (auto &thread: threads) thread.join();
}
