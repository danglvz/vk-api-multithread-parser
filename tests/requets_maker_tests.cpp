



#include <gtest/gtest.h>
#include "requests_maker.h"


TEST(req_test, test_1){
    simple_https::https_client httpsClient("api.vk.com");
    vk_parse::requests_maker requestsMaker("-91050183", "tyty");
    vk_parse::posts posts(requestsMaker.parse_posts(0,httpsClient));
    requestsMaker.parse_who_liked(posts[1], httpsClient);
    ASSERT_TRUE(!posts.empty());
}
