cmake_minimum_required(VERSION 3.14)
project(big_bool C)

set(CMAKE_CSTANDARD 99)

add_library(big_bool big_bool.c big_bool.h)
add_executable(test bb_tests.c)

target_link_libraries(test big_bool)