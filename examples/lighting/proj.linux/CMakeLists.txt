cmake_minimum_required(VERSION 2.8)

project(mugato_lighting)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas")
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGORN_PLATFORM_LINUX)

add_subdirectory(../../../proj.linux "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/mugato")

#TODO: find include dirs
include_directories(../../../src)
include_directories(../../../lib)
include_directories(../../../lib/gorn/src)
include_directories(../../../lib/gorn/lib)
include_directories(../../../lib/gorn/lib/buffer/src)

file(GLOB SOURCES "../src/*.cpp")
add_executable(lighting ${SOURCES})
target_link_libraries(lighting mugato)
