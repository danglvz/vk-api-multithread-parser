
#include "requests_maker.h"



using namespace vk_parse;

requests_maker::requests_maker(const std::string &owner_id, const std::string &output_filename):
owner_id_(owner_id),
binaryFileWriter(output_filename),
access_token_("")
{}

posts requests_maker::parse_posts(int offset, simple_https::https_client &httpsClient) {
    return get_posts_ids(httpsClient.get_request("method/wall.get?count=100&access_token=" + access_token_ +"&v=5.110&owner_id=" + owner_id_ +
"&offset=" + std::to_string(offset)));
}

void requests_maker::parse_who_liked(posts_item &post, simple_https::https_client &httpsClient) {
    std::vector<uint32_t> output, temp;
    output.reserve(post[1] + 30);
    int count{post[1]};
    if ( post[1] == 0 ) {
        std::cout << "error\n";
        return;
    }

    for ( int offset = 0; count > 0; offset += 1000, count -= 1000 ) {
        temp = get_likes_array(httpsClient.get_request("method/likes.getList?type=post&count=1000&access_token=" + access_token_ + "&v=5.110&owner_id=" +
                                            owner_id_ + "&offset=" + std::to_string(offset) + "&item_id=" + std::to_string(post[0])));
        output.insert(output.end(), temp.begin(), temp.end());
    }
    binaryFileWriter.write(output, post);
}



