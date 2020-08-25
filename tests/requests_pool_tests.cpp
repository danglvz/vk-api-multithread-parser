



#include <gtest/gtest.h>
#include "multithread.h"


TEST(request_pool_test, destruct_test){
    vk_parse::requests_pool requestsPool(200, "test");
}



