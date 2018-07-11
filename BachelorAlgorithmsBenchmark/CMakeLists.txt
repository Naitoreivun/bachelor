cmake_minimum_required(VERSION 3.5.1)
project(BachelorAlgorithmsBenchmark)

SET(PROJECT_NAME bachelorAlgorithmsBenchmark)

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

set(ALL_FILES imports_and_configs.h "${ALT_FILES}" "${BIDI_FILES}" "${DIJKSTRA_FILES}" "${MLD_FILES}")

add_executable(run_custom_main main.cpp "${ALL_FILES}")

add_executable(${PROJECT_NAME} BachelorAlgorithmsBenchmark.cpp "${ALL_FILES}")

add_dependencies(${PROJECT_NAME} Celero)
target_link_libraries("${PROJECT_NAME}" celero)

if (MSVC)
    # Broiler Plate: VS2012 doesn't support true variadic templates
    target_compile_definitions(${PROJECT_NAME} PRIVATE /D_VARIADIC_MAX=10)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(${PROJECT_NAME} PRIVATE /D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)
    target_compile_options(${PROJECT_NAME} PRIVATE /nologo)
endif ()


# Broiler Plate: Set up folders for an IDE.
if (CELERO_ENABLE_FOLDERS)
    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "/")
endif ()
