#include <gtest/gtest.h>
#include "parsers_from_json.h"


TEST(api_json_test, get_likes_ids_tests){
    std::string req{R"({"response":{"count":100,"items":[1111,2222,3333,4444]}})"};
    std::vector<std::uint32_t> expected_results{1111,2222,3333,4444},
    output(vk_parse::get_likes_array(req));
    ASSERT_EQ(expected_results, output);
    ASSERT_EQ(std::vector<std::uint32_t>(), vk_parse::get_likes_array(""));
}

using output_item = std::vector<vk_parse::posts_item>;

TEST(api_json_test, get_post_id_tests) {
    output_item expect_results{{111, 1},
                               {222, 2},
                               {333, 3}};
    output_item output{vk_parse::get_posts_ids(R"({"response":{"count":10,"items":[
                                                    {"id":111,"likes":{"count":1,"something":2}},
                                                    {"id":222,"likes":{"count":2,"something":2}},
                                                    {"id":333,"likes":{"count":3, "something":3}}]}})")};
    ASSERT_EQ(expect_results, output);
    output = vk_parse::get_posts_ids("");
    ASSERT_EQ(output_item{} , output);
    output = vk_parse::get_posts_ids(R"({"error":{
                                        "error_code":5,
                                        "error_msg":"User authorization failed: no access_token passed.",
                                        "request_params":[
                                            {"key":"method","value":"wall.get"},
                                            {"key":"oauth","value":"1"}]}})");
    ASSERT_EQ(output_item{}, output);
}
