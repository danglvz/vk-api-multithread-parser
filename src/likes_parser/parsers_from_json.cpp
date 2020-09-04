

#include "parsers_from_json.h"


using namespace vk_parse;

posts vk_parse::get_posts_ids(const std::string &str) {
    posts output;
    output.reserve(100);
    posts_item temp_item{};
    std::string id_key{"\"id\""}, likes_key{"\"likes\""},count_key("\"count\"") ,digits{"0123456789"};
    int scopes{}, quotes{}, quad_scopes{1};
    std::size_t start;
    bool isIn{};

    if( (start = str.find("\"response\"")) == std::string::npos) {
        std::cout << str << '\n';
        return std::vector<posts_item>();
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
                        if (i == std::string::npos){
                            return std::vector<posts_item>();
                        }
                        temp_item[0] = std::stoi(str.substr(start, i - start));
                    }
                    else if ( !str.compare(i - likes_key.length(), likes_key.length(), likes_key) ) isIn = true;

                } else if ( isIn && !str.compare(i - count_key.length(), count_key.length(), count_key) ) {
                    start = str.find_first_of(digits, i);
                    i = str.find_first_not_of(digits, start);
                    if (i == std::string::npos){
                        return std::vector<posts_item>();
                    }
                    temp_item[1] = std::stoi(str.substr(start, i - start));
                    output.push_back(temp_item);
                }
        }

    }
    return output;
}

std::vector<std::uint32_t> vk_parse::get_likes_array(const std::string &str) {
    std::vector<std::uint32_t> output;
    output.reserve(1000);
    std::size_t start{};
    int quad_scopes{1};
    if ( (start = str.find("\"response\"")) == std::string::npos ) {
        std::cout << str << "\n";
        return output;
    }
    if ( (start = str.find_first_of('[', start)) == std::string::npos ) {
        std::cout << str << '\n';
        return output;
    }
    ++start;
    for ( std::size_t i = start; quad_scopes > 0 && i < str.length(); ++i ) {
        if ( str[i] >= '0' && str[i] <= '9' ) {
            start = i;
            for ( ;str[i] >= '0' && str[i] <= '9' && i < str.length(); ++i );
            output.emplace_back(std::stoi(str.substr(start, i - start)));
        } else if ( str[i] == ']' ) --quad_scopes;
    }

    return output;
}