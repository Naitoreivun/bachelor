cmake_minimum_required(VERSION 3.10)
project(Bachelor)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

add_executable(foo tests/foo.cpp)
add_executable(makeQueries tests/makeQs.cpp)

add_executable(run_all main.cpp imports_and_configs.h

        alt/AltProgram.h alt/AltVertex.cpp alt/AltVertex.h alt/alt.cpp alt/alt.h

        bidirectional-dijkstra/BidiProgram.h bidirectional-dijkstra/BidirectionalDijkstra.cpp
        bidirectional-dijkstra/BidirectionalDijkstra.h bidirectional-dijkstra/BidiVertex.h bidirectional-dijkstra/BidiVertex.cpp

        dijkstra/DijkstraProgram.h dijkstra/RegularVertex.h dijkstra/Dijkstra.h

        multi-level-dijkstra/MLDProgram.h
        multi-level-dijkstra/MultilevelGraph.cpp multi-level-dijkstra/MultilevelGraph.h
        multi-level-dijkstra/ConnectedComponent.cpp multi-level-dijkstra/ConnectedComponent.h
        multi-level-dijkstra/Level.cpp multi-level-dijkstra/Level.h
        multi-level-dijkstra/Vertex.h multi-level-dijkstra/Vertex.cpp
        )