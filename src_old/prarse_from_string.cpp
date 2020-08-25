//
// Created by dd on 25.06.2020.
//

#include "prarse_from_string.h"
#include "custom_http_client.h"
#include "multitreading.h"
#include "binary_file_writer.h"
#include <vector>
#include <array>



using MyItem = std::array<int, 2>;



parse_from_api::parse_from_api(const std::string &owner_id) : owner_id_(owner_id), fileWriter(std::string(owner_id) + ".bin"), access_token_() {
    std::ifstream config("config", std::ios::in);
    std::string line;
    while (std::getline(config, line)) {
        if (line.find("access_token") != std::string::npos) {
            access_token_ = line.substr(line.find_first_of('=') + 1);
        }
    }
    if (access_token_.empty())
        throw std::runtime_error("Reading config error: access_token value not found");
    config.close();
}

std::vector<std::array<int, 2>> parse_from_api::parse_posts(const int &offset, http_client &myHttp) {
    std::vector<MyItem> output;
    std::string str{myHttp.get_request("GET /method/wall.get?count=100&access_token=" + access_token_ +"&v=5.110&owner_id=" + owner_id_ +
                                       "&offset=" + std::to_string(offset) + " HTTP/1.1\r\nHost: api.vk.com\r\n\r\n")};
    output.reserve(100);
    MyItem temp_item{};
    std::string id_key{"\"id\""}, likes_key{"\"likes\""},count_key("\"count\"") ,digits{"0123456789"};
    int scopes{}, quotes{}, quad_scopes{1};
    std::size_t start;
    bool isIn{};

    if( (start = str.find("\"response\"")) == std::string::npos) {
        std::cout << str << '\n';
        return std::vector<MyItem>();
    }
    start = str.find_first_of('[', start);
    ++start;
    for ( std::size_t i = start; quad_scopes >= 1 && i < str.length(); ++i  ) {
        switch (str[i]) {
            case '\"':
                if ( quotes == 0 ) ++quotes;
                else if ( quotes == 1 ) --quotes;
                break;
            case '[':
                if ( quotes == 0 ) ++quad_scopes;
                break;
            case ']':
                if ( quotes == 0) --quad_scopes;
                break;
            case '{':
                if ( quotes == 0 ) ++scopes;
                break;
            case '}':
                if ( quotes == 0 ) {
                    if ( isIn ) isIn = false;
                    --scopes;
                }
                break;
            case ':':
                if ( quotes == 0 && quad_scopes == 1 && scopes == 1 ) {
                    if ( !str.compare(i - id_key.length(), id_key.length(), id_key) ) {
                        start = str.find_first_of(digits, i);
                        i = str.find_first_not_of(digits, start);
                        temp_item[0] = std::stoi(str.substr(start, i - start));
                    }
                    else if ( !str.compare(i - likes_key.length(), likes_key.length(), likes_key) ) isIn = true;

                } else if ( isIn && !str.compare(i - count_key.length(), count_key.length(), count_key) ) {
                    start = str.find_first_of(digits, i);
                    i = str.find_first_not_of(digits, start);
                    temp_item[1] = std::stoi(str.substr(start, i - start));
                    output.push_back(temp_item);
                }
        }

    }
    return output;
}

void parse_from_api::parse_likes(const std::array<int, 2> &post, http_client &myHttp) {
    std::vector<uint32_t> output;
    output.reserve(post[1] + 30);
    std::size_t start;
    std::string str;
    int count{post[1]}, quad_scopes;

    if ( post[1] == 0 ) {
        std::cout << "error\n";
        return;
    }

    for ( int offset = 0; count > 0; offset += 1000, count -= 1000 ) {
        start = 0;
        str = myHttp.get_request(
                "GET /method/likes.getList?type=post&count=1000&access_token=" + access_token_ + "&v=5.110&owner_id=" +
                owner_id_ + "&offset=" + std::to_string(offset) + "&item_id=" + std::to_string(post[0]) +
                " HTTP/1.1\r\nHost: api.vk.com\r\n\r\n");
        quad_scopes = 1;
        if ( (start = str.find("\"response\"")) == std::string::npos ) {
            std::cout << str << "\n";
            continue;
        }
        if ( (start = str.find_first_of('[', start)) == std::string::npos ) {
            std::cout << str << '\n';
            continue;
        }
        ++start;
        for ( std::size_t i = start; quad_scopes > 0 && i < str.length(); ++i ) {
            if ( str[i] >= '0' && str[i] <= '9' ) {
                start = i;
                for ( ;str[i] >= '0' && str[i] <= '9'; ++i );
                output.emplace_back(std::stoi(str.substr(start, i - start)));
            } else if ( str[i] == ']' ) --quad_scopes;
        }

    }
    fileWriter.write(output, post);
}


