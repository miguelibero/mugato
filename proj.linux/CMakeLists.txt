cmake_minimum_required(VERSION 2.8) 

project(mugato)

add_definitions(-Wall -std=c++11 -Wno-unknown-pragmas)
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGORN_PLATFORM_LINUX)

add_subdirectory(../lib/gorn/proj.linux "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/gorn")

# library
include_directories(../src)
include_directories(../lib)
include_directories(../lib/gorn/src)
include_directories(../lib/gorn/lib)
set(MUGATO_SOURCE_DIR "../src/mugato")
set(MUGATO_INCLUDE_DIR "../src")

file(GLOB MUGATO_SOURCES
	"${MUGATO_SOURCE_DIR}/*.cpp"
	"${MUGATO_SOURCE_DIR}/base/*.cpp"
	"${MUGATO_SOURCE_DIR}/label/*.cpp"
	"${MUGATO_SOURCE_DIR}/sprite/*.cpp"
	"${MUGATO_SOURCE_DIR}/scene/*.cpp"
)
add_library(mugato ${MUGATO_SOURCES})
target_link_libraries(mugato gorn)
