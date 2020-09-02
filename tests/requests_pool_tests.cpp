



#include <gtest/gtest.h>
#include "multithread.h"


TEST(requests_pool_test, destruct_test){
    vk_parse::requests_pool requestsPool(200);
}


TEST(requests_pool_test, web_test){
    vk_parse::requests_pool reqPoll(25);
    simple_https::https_client httpclient("api.vk.com");
    std::string expect_str{httpclient.get_request("method/wall.get?")};
    std::vector<std::future<std::string>> result_vec;
    for ( int i{}; i < 30; ++i ){
        result_vec.emplace_back(reqPoll.submit([](simple_https::https_client &client ) -> std::string {
            return client.get_request("method/wall.get?");
        }));
    }
    
    for ( auto &i: result_vec ){
        ASSERT_STREQ(expect_str.c_str(), i.get().c_str());
    }
    

}

