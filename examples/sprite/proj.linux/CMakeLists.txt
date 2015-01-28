cmake_minimum_required(VERSION 2.8) 

project(mugato_sprite)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-Wall -std=c++11 -Wno-unknown-pragmas)
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGORN_PLATFORM_LINUX)

add_subdirectory(../../../proj.linux "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/mugato")

#TODO: find include dirs
include_directories(../../../src)
include_directories(../../../lib)
include_directories(../../../lib/gorn/src)
include_directories(../../../lib/gorn/lib)

file(GLOB SOURCES "../src/*.cpp")
add_executable(sprite ${SOURCES})
target_link_libraries(sprite mugato)
