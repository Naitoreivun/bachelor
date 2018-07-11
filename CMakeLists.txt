cmake_minimum_required(VERSION 3.5.1)
#set(DCMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=gnu++11 -Wall -O3")

include(ExternalProject)

set(CELERO_COMPILE_DYNAMIC_LIBRARIES OFF)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(Celero
        GIT_REPOSITORY https://github.com/DigitalInBlue/Celero.git
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_subdirectory(BachelorAlgorithmsBenchmark)
