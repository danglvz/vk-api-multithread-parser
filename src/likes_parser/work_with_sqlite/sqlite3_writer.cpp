
#include <iostream>
#include "sqlite3_writer.h"





using namespace vk_parse;


void sqlite::sqlite3_writer::simple_queury(const std::string &query){
    if ( sqlite3_exec(sql.get(), query.c_str(), nullptr, nullptr, nullptr) ){
        throw std::runtime_error(sqlite3_errmsg(sql.get()));
    }   

}

sqlite::sqlite3_writer::sqlite3_writer(const std::string &filename, const std::string &group_id):
table_name("group_" + group_id)
{
    sqlite3 *temp_sql_ptr;
    if ( sqlite3_open(filename.c_str(), &temp_sql_ptr) ){
        throw std::runtime_error("open sqlite3 db error");
    }
    sql.reset(temp_sql_ptr);
    simple_queury("PRAGMA synchronous = OFF");
    simple_queury("PRAGMA journal_mode = MEMORY");
}


void sqlite::sqlite3_writer::create_new_group_table(  ){
    if ( sqlite3_exec(sql.get(), ("DROP TABLE " + table_name + ";").c_str(), nullptr, nullptr, nullptr) ){
        if (strstr(sqlite3_errmsg(sql.get()), "no such table") == nullptr){
            throw std::runtime_error(sqlite3_errmsg(sql.get()));
        }
    }

    if (sqlite3_exec(sql.get(), ("CREATE TABLE " + table_name + " (user_id INTEGER, likes_count INTEGER);").c_str(), nullptr, nullptr, nullptr )){
        throw std::runtime_error(sqlite3_errmsg(sql.get()));
    } 
}


void sqlite::sqlite3_writer::start_transaction(){
    simple_queury("BEGIN TRANSACTION");
}

void sqlite::sqlite3_writer::end_transaction(){
    simple_queury("END TRANSACTION");
}


void sqlite::sqlite3_writer::write_likes_ids(std::vector<std::string> likes_vec, std::array<int, 2> post){
    for ( auto &id: likes_vec ){
        if (sqlite3_exec(sql.get(), 
           ("INSERT INTO " + table_name + " (post_id, user_id) VALUES (" + std::to_string(post[0])+","+ id + ");").c_str(),
           nullptr, nullptr, nullptr) )
           {
                throw std::runtime_error(sqlite3_errmsg(sql.get()));
           }
        std::cout << id << "\n";
    }
}

void sqlite::sqlite3_writer::write_pair(const std::pair<std::uint32_t, std::uint32_t> &pair){
    simple_queury("INSERT INTO " + table_name + " (user_id,likes_count) VALUES (" + std::to_string(pair.first)+","+std::to_string(pair.second)+");");
}
