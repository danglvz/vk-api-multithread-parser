//
// Created by dd on 25.06.2020.
//

#ifndef UNTITLED7_PRARSE_FROM_STRING_H
#define UNTITLED7_PRARSE_FROM_STRING_H
#include "binary_file_writer.h"
#include "custom_http_client.h"


#include <iostream>
    class parse_from_api {
        const std::string owner_id_;
        binary_file_writer fileWriter;
    public:
        explicit parse_from_api(const std::string &owner_id_);
        std::vector<std::array<int, 2>> parse_posts(const int &offset, http_client &myHttp);
        void parse_likes(const std::array<int, 2> &post, http_client &myHttp);
    };

#endif //UNTITLED7_PRARSE_FROM_STRING_H
