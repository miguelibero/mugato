cmake_minimum_required(VERSION 2.8) 

project(spine)

set(SPINE_INCLUDE_DIR "include")
set(SPINE_SOURCE_DIR "src/spine")
include_directories(${SPINE_INCLUDE_DIR})

file(GLOB SPINE_SOURCES
	"${SPINE_SOURCE_DIR}/*.c"
)
add_library(spine ${SPINE_SOURCES})
