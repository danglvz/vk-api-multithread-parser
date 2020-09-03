add_library(vk_likes_parser "")

add_subdirectory(https_client)
add_subdirectory(work_with_sqlite)
find_package(Threads REQUIRED) 

file(GLOB SRC_
        "*.h"
        "*.cpp")

target_sources(vk_likes_parser PRIVATE ${SRC_})
target_include_directories(vk_likes_parser PUBLIC
                                ${CMAKE_CURRENT_LIST_DIR}/https_client/src
                                ${CMAKE_CURRENT_LIST_DIR}/work_with_sqlite
                                 )
target_link_libraries(vk_likes_parser PUBLIC
                        https_client
                        sqlite3_writer
                        Threads::Threads)
