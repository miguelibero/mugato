cmake_minimum_required(VERSION 2.8) 

project(mugato)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas")
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DGORN_PLATFORM_LINUX)

add_subdirectory(../lib/gorn/proj.linux "${CMAKE_CURRENT_BINARY_DIR}/gorn")
add_subdirectory(../lib/spine-c "${CMAKE_CURRENT_BINARY_DIR}/spine")

include_directories(../src)
include_directories(../lib)
include_directories(../lib/gorn/src)
include_directories(../lib/gorn/lib)
include_directories(../lib/gorn/lib/buffer/src)
include_directories(../lib/spine-c/include)

set(MUGATO_SOURCE_DIR "../src/mugato")
set(MUGATO_INCLUDE_DIR "../src")

file(GLOB MUGATO_SOURCES
	"${MUGATO_SOURCE_DIR}/*.cpp"
	"${MUGATO_SOURCE_DIR}/base/*.cpp"
	"${MUGATO_SOURCE_DIR}/label/*.cpp"
	"${MUGATO_SOURCE_DIR}/sprite/*.cpp"
	"${MUGATO_SOURCE_DIR}/spine/*.cpp"
	"${MUGATO_SOURCE_DIR}/scene/*.cpp"
)
add_library(mugato ${MUGATO_SOURCES})
target_link_libraries(mugato spine gorn)
