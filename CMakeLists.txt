cmake_minimum_required(VERSION 3.10)
project(Bachelor)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

add_subdirectory(multi-level-dijkstra)
add_subdirectory(bidirectional-dijkstra)
add_subdirectory(alt)
add_executable(foo tests/foo.cpp)
add_executable(makeQueries tests/makeQs.cpp)