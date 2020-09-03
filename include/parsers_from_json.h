

#ifndef VK_PARSER_PARSERS_FROM_JSON_H
#define VK_PARSER_PARSERS_FROM_JSON_H

#include <vector>
#include <iostream>
#include <array>

namespace vk_parse {
    using posts_item = std::array<int, 2>;
    using posts = std::vector<posts_item>;
    std::vector<std::uint32_t> get_likes_array(const std::string &req);
    posts get_posts_ids( const std::string &req);
}



#endif //VK_PARSER_PARSERS_FROM_JSON_H
