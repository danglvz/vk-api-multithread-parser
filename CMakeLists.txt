cmake_minimum_required(VERSION 3.16)
project(untitled7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lssl -lcrypto -pthread")

add_executable(untitled7 main.cpp src/custom_http_client.cpp src/custom_http_client.h src/prarse_from_string.cpp src/prarse_from_string.h src/binary_file_writer.cpp src/binary_file_writer.h src/multitreading.cpp src/multitreading.h src/likes_parser.cpp src/likes_parser.h)
target_link_libraries(untitled7 ssl crypto)