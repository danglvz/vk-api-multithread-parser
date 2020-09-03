#ifndef VK_PARSER_SQLITE3_WRITER_H
#define VK_PARSER_SQLITE3_WRITER_H

#include "sqlite3.h"
#include <cstring>
#include <memory>
#include <vector>
#include <array>


namespace vk_parse::sqlite{

    struct sql_destructor{
        void operator()( sqlite3 *s_ptr )const { sqlite3_close(s_ptr); }
    };

    using Sql_u_ptr = std::unique_ptr<sqlite3, sql_destructor>;

    class sqlite3_writer{
        std::string table_name;
        Sql_u_ptr sql;
        void simple_queury(const std::string &query);
        


    public:
        explicit sqlite3_writer( const std::string &filename, const std::string &group_id );
        void create_new_group_table(  );
        void write_likes_ids(std::vector<std::string> likes_vec, std::array<int, 2> post);
        void start_transaction();
        void end_transaction();
        void write_pair(const std::pair<std::uint32_t, std::uint32_t> &pair);
    };





}








#endif