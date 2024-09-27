cmake_minimum_required(VERSION 3.10)

# Project name and version
project(GraphSearch VERSION 1.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB SRC "src/*.c")
include_directories(inc)

add_executable(graph_demo ${SRC})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})