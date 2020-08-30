add_executable(vk_parser_tests "")

add_subdirectory(googletest)

file(GLOB TESTS_
        *.cpp)

target_sources(vk_parser_tests PRIVATE ${TESTS_})
target_include_directories(vk_parser_tests PRIVATE ${CMAKE_SOURCE_DIR}/src ${GTEST_INCLUDE_DIR})
target_link_libraries(vk_parser_tests PRIVATE vk_likes_parser gtest_main gtest)

