add_executable(vk_parser_tests "")


find_package(GTest REQUIRED)

file(GLOB TESTS_
        "*.cpp")

message(${TESTS_})
message(${CMAKE_CURRENT_LIST_DIR})
list(FILTER TESTS_  EXCLUDE  REGEX ".*/requets_maker_tests.cpp$" )        
message(${TESTS_})

target_sources(vk_parser_tests PRIVATE ${TESTS_})
target_include_directories(vk_parser_tests PRIVATE
                                ${CMAKE_SOURCE_DIR}/src/likes_parser
                                ${GTEST_INCLUDE_DIR}
                                ${vk_parser_SOURCE_DIR}/include
                                )
target_link_libraries(vk_parser_tests PRIVATE
                        vk_likes_parser
                        gtest_main 
                        gtest
                        )

