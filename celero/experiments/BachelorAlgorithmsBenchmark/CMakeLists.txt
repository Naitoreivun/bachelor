cmake_minimum_required(VERSION 3.0.2)
project(BachelorAlgorithmsBenchmark)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11 -Wall -O3")
#set(DCMAKE_BUILD_TYPE Release)

message("${CMAKE_CXX_FLAGS}")


set(ALT_DIR alt)
set(ALT_FILES
        "${ALT_DIR}/AltProgram.h" "${ALT_DIR}/AltVertex.cpp" "${ALT_DIR}/AltVertex.h"
        "${ALT_DIR}/alt.cpp" "${ALT_DIR}/alt.h")

set(BIDI_DIR bidirectional-dijkstra)
set(BIDI_FILES
        "${BIDI_DIR}/BidiProgram.h" "${BIDI_DIR}/BidirectionalDijkstra.cpp"
        "${BIDI_DIR}/BidirectionalDijkstra.h" "${BIDI_DIR}/BidiVertex.h" "${BIDI_DIR}/BidiVertex.cpp")

set(MLD_DIR multi-level-dijkstra)
set(MLD_FILES
        "${MLD_DIR}/MLDProgram.h"
        "${MLD_DIR}/MultilevelGraph.cpp" "${MLD_DIR}/MultilevelGraph.h"
        "${MLD_DIR}/ConnectedComponent.cpp" "${MLD_DIR}/ConnectedComponent.h"
        "${MLD_DIR}/Level.cpp" "${MLD_DIR}/Level.h"
        "${MLD_DIR}/Vertex.h" "${MLD_DIR}/Vertex.cpp")

set(DIJKSTRA_FILES dijkstra/DijkstraProgram.h dijkstra/RegularVertex.h dijkstra/Dijkstra.h)

add_executable(run_all main.cpp imports_and_configs.h "${ALT_FILES}" "${BIDI_FILES}" "${DIJKSTRA_FILES}" "${MLD_FILES}")