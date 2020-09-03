



#include<iostream>
#include <cstring>
#include <fstream>
#include "main_thread.h"
#include "counting_and_sorting_results.h"


int main(int argc, char* argv[]){
    //std::cout << "count_of_threads: "  << std::stoi(argv[1]) << std::endl;
    //std::cout << "count_of posts: " << std::stoi(argv[2]) << std::endl;



    std::string access_token, group_id, help{"--access_token; -a;[set your access token]\n--group_id; -g;[set group id without \"-\"]\n--file; -f[file with groupes ids]"};
    std::string file_with_ids;
    size_t count_of_posts;
    if (argc < 7) {
        std::cout << help;
        return 0;
    } 
    for ( int i{1}; i < argc; ++i ){
        if (strcmp(argv[i], "--access_token") != 0 || strcmp(argv[i], "-a")!= 0){
            if ( (i+1) >= argc) {
                std::cout << help;
                return 0;
            }
            access_token.assign(argv[i+1]);
        }
        if (strcmp(argv[i], "--group_id") != 0 || strcmp(argv[i], "-g")) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            group_id.assign(argv[i+1]);
        }
        if (strcmp(argv[i], "--count_of_posts") != 0 || strcmp(argv[i], "-c")) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            count_of_posts = std::stoi(argv[i+1]);
        }
        if (strcmp(argv[i], "--file") != 0 || strcmp(argv[i], "-f")) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            file_with_ids.assign(argv[i+1]);
        }
    }

    std::vector<std::string> ids;
    if ( file_with_ids.length() ){
        std::ifstream file(file_with_ids);
        std::string s;
        while(getline(file, s)){
            ids.emplace_back(s);
        }
    }


    std::cout << "Your access_token: "<< access_token << "\nYour group" << group_id << "\nCount_of_posts: " << count_of_posts << '\n';

    std::string filename;
    if (ids.size()) {
        for (auto &id: ids){
            filename.assign("cache_"+group_id);

            vk_parse::main_thread p(id, filename, 30, access_token);
            p.start(count_of_posts);
            vk_parser::count_items(filename);
        }
    } else {
        vk_parse::main_thread p(group_id, filename, 30, access_token);
        p.start(count_of_posts);

        vk_parser::count_items(filename);
    }
    



    return 0;
}