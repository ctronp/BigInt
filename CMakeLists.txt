cmake_minimum_required(VERSION 3.22)
project(big_int C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
add_executable(test
        Tests.c
        BigInt.c
        )