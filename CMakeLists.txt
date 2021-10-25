cmake_minimum_required(VERSION 3.12)
project(6.887Lab4)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/O2 /arch:AVX2 /W1 /EHsc")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "-O3 -xHost")
else()
    # clang and gcc
    set(CMAKE_CXX_FLAGS "-O3 -march=native -Wall -Wextra")
endif()

include_directories(src)

add_executable(sanity_check src/sanity_check.cpp)
add_executable(benchmark_learned_index src/benchmark_learned_index.cpp)
add_executable(benchmark_binary_search src/benchmark_binary_search.cpp)