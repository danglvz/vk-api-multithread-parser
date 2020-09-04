#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "counting_and_sorting_results.h"
#include "binary_reader.h"
#include "sqlite3_writer.h"



void vk_parser::count_items(const std::string &filename, const std::string &group_id){
    binary_file_reader binred(filename);
    std::unordered_map<std::uint32_t, std::uint32_t> map{};
    
    std::cout << "create temp vec\n";
    std::vector<std::uint32_t> temp(std::move(binred.read_to_vector()));
    std::cout << "start adding in map\n";
    int cch{};
    std::uint32_t null{};
    for ( size_t i{}; i < temp.size(); ++i ){
        if (temp[i] == 0 || temp[i] < 100000){
            continue;
        }
        ++map[temp[i]];
    }
    
    std::cout << "adding in map complited\n";
    std::cout <<"prepare adding in vec\n";
    std::vector<std::pair<std::uint32_t, std::uint32_t>> output;

    for ( auto &i: map ){
        output.emplace_back(std::move(i));
    }   
    std::cout << "adding in vec complite\n";    
    map.clear();
    std::cout << "prepera sorting\n";
    std::sort(output.begin(), output.end(), [](const  std::pair<std::uint32_t, std::uint32_t> &a,
                                               const  std::pair<std::uint32_t,std::uint32_t> &b){
                                                    return a.second > b.second;
                                                });

    std::cout << "sorted\n";

    for ( int i{}; i < 100; ++i ){
        std::cout<< output[i].first << "  " << output[i].second << "\n";
    }                                            

    vk_parse::sqlite::sqlite3_writer sql3{"output.db", group_id};
    sql3.create_new_group_table();
    sql3.start_transaction();
    for ( auto &i:output ){
        sql3.write_pair(i);
    }
    sql3.end_transaction();

    std::cout << temp[0] << " " << temp[1] << "\n";
}
