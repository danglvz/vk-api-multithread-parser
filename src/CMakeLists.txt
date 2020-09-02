add_subdirectory(likes_parser)


add_executable(vk_parser_main "")

find_package(Threads REQUIRED)

target_sources(vk_parser_main PRIVATE main.cpp)
target_include_directories(vk_parser_main PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/likes_parser 
    ${CMAKE_CURRENT_LIST_DIR}/likes_parser/https_client/src
    )

target_link_libraries(vk_parser_main PRIVATE
                        vk_likes_parser
                        Threads::Threads
    )
