add_library(sqlite3_writer "")


find_package(SQLite3)
if (NOT SQLITE3_FOUND)
    MESSAGE("SQLite3 not found!")
    RETURN()
ENDIF()    



file(GLOB sqlite3_writer_src
        "*.h"
        "*.cpp")


target_sources(sqlite3_writer PRIVATE  ${sqlite3_writer_src})
target_include_directories(sqlite3_writer PUBLIC 
                            ${SQLITE3_INCLUDE_DIR} 
                            ${CMAKE_CURRENT_LIST_DIR}
                            ${vk_parser_SOURCE_DIR}/include
                            )
target_link_libraries(sqlite3_writer PUBLIC sqlite3)

        