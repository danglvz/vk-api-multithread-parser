add_executable(vk_parser_tests "")

add_subdirectory(googletest)

target_sources(vk_parser_tests PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/runAll.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thread_safe_queue_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thread_pool_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/parsers_from_json_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/requets_maker_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/thread_safe_binary_file_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/requests_pool_tests.cpp)
target_include_directories(vk_parser_tests PRIVATE ${CMAKE_SOURCE_DIR}/src ${GTEST_INCLUDE_DIR})
target_link_libraries(vk_parser_tests PRIVATE vk_likes_parser gtest_main gtest)

