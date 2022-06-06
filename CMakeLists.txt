cmake_minimum_required(VERSION 3.22)
project(big_int VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-Og -g -flto")

include(CTest)
add_library(BigInt
        BigInt.c
        BigInt.h
        )
add_executable(tests
        Tests.c)
target_link_libraries(tests
        BigInt)
