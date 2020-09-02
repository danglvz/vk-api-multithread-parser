
#ifndef VK_PARSER_REQUESTS_MAKER_H
#define VK_PARSER_REQUESTS_MAKER_H

#include "parsers_from_json.h"
#include "using_openssl.h"
#include "thread_safe_binary_file_writer.h"


namespace vk_parse{
    using posts = std::vector<posts_item>;


    class requests_maker{
        std::string owner_id_, access_token_;
        binary_file_writer binaryFileWriter;
    public:
        explicit requests_maker(const std::string &owner_id, const std::string &output_filename);
        posts parse_posts(int offset, simple_https::https_client &httpsClient);
        void parse_who_liked(posts_item &post, simple_https::https_client &httpsClient);
    };
}



#endif //VK_PARSER_REQUESTS_MAKER_H
