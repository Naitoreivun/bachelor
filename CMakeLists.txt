cmake_minimum_required(VERSION 3.11.4)
#set(DCMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11 -Wall -O3")

set(CELERO_COMPILE_DYNAMIC_LIBRARIES OFF)

include(FetchContent)
FetchContent_Declare(
        celero
        GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
        GIT_TAG        v2.3.0
)

FetchContent_GetProperties(celero)
if(NOT celero_POPULATED)
    FetchContent_Populate(celero)
    add_subdirectory(${celero_SOURCE_DIR} ${celero_BINARY_DIR})
endif()

message(STATUS "Catch include dir: ${celero_SOURCE_DIR}/include")
add_subdirectory(BachelorAlgorithmsBenchmark)
