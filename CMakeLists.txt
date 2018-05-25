cmake_minimum_required(VERSION 3.10)
project(Bachelor)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

add_executable(Bachelor main.cpp imports_and_configs.h Edge.cpp Edge.h Vertex.cpp Vertex.h Level.cpp Level.h MultilevelGraph.cpp MultilevelGraph.h)