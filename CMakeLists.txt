cmake_minimum_required(VERSION 3.0.2)
add_subdirectory(celero)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11 -Wall -O3")
add_executable(foo tests/foo.cpp)
add_executable(makeQueries tests/makeQs.cpp)
add_executable(normalize tests/normalize.cpp)
add_executable(countCC tests/countSCC.cpp tests/findSCC.h)