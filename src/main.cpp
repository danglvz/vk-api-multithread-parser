



#include<iostream>
#include <cstring>
#include <fstream>
#include "main_thread.h"
#include "counting_and_sorting_results.h"


int main(int argc, char* argv[]){



    std::string access_token, group_id, help{"--access_token; -a;[set your access token]\n--group_id; -g;[set group id without \"-\"]\n--file; -f[file with groupes ids]\n--count_of_posts; -c[set count of post for parsing, default value=10000]\n"};
    std::string file_with_ids, output_db_file{"output.db"};
    size_t count_of_posts{10000};
    if (argc < 4) {
        std::cout << help;
        return 0;
    } 
    for ( int i{1}; i < argc; ++i ){
        if (strcmp(argv[i], "--access_token") == 0 || strcmp(argv[i], "-a")== 0){
            if ( (i+1) >= argc) {
                std::cout << help;
                return 0;
            }
            access_token.assign(argv[i+1]);
        }
        else if (strcmp(argv[i], "--group_id") == 0 || strcmp(argv[i], "-g" )==0) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            group_id.assign(argv[i+1]);
        }
        else if (strcmp(argv[i], "--count_of_posts") == 0 || strcmp(argv[i], "-c")==0) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            count_of_posts = std::stoi(std::string(argv[i+1]));
        }
        else if (strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f")==0) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            file_with_ids.assign(argv[i+1]);
        }
        else if (strcmp(argv[i], "--output_db") == 0 || strcmp(argv[i], "-o")==0) {
            if ( (i+1) >= argc ){
                std::cout << help;
                return 0;
            }
            output_db_file.assign(argv[i+1]);
        }
    }

    std::vector<std::string> ids;
    if ( file_with_ids.length() ){
        std::ifstream file(file_with_ids);
        std::string s;
        while(getline(file, s)){
            ids.emplace_back(s);
        }
        file.close();
    }
 
    if ( access_token.length() == 0 ){
        std::cout << "set your access_token" << std::endl;
        std::cout << std::endl << help;
        return 0;
    }

    if (ids.size() == 0 && group_id.length() == 0){
        std::cout << "set group_id or file with groups ids" << std::endl;
        std::cout << std::endl << help;
        return 0;

    }



    std::cout << "Your access_token: "<< access_token << "\nYour group" << group_id << "\nCount_of_posts: " << count_of_posts << '\n';

    std::string filename;
    if (ids.size()) {
        for (auto &id: ids){
            filename.assign("cache_"+id);

            vk_parse::main_thread p(id, filename, 30, access_token);
            p.start(count_of_posts);
            vk_parser::count_items(filename, id, output_db_file);
        }
    } else {
        filename.assign("cache_"+group_id);
        vk_parse::main_thread p(group_id, filename, 30, access_token);
        p.start(count_of_posts);

        vk_parser::count_items(filename, group_id, output_db_file);
    }
    

    return 0;
}