
#ifndef VK_PARSER_MAIN_THREAD_H
#define VK_PARSER_MAIN_THREAD_H

#include "multithread.h"
#include "thread_safe_binary_file_writer.h"
#include "work_with_sqlite/sqlite3_writer.h"

namespace vk_parse{

    class main_thread{
        std::string owner_id_, access_token_;
        binary_file_writer file;
        //sqlite::sqlite3_writer sql_writer;

        requests_pool req_pool;
    public:
        main_thread(const std::string &owner_id, const std::string &output_filename, int count_of_threads);
        void start(std::size_t count_of_posts);        
    };
}

#endif //VK_PARSER_MAIN_THREAD_H
