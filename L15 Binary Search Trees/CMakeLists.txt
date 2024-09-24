cmake_minimum_required(VERSION 3.10)

# Project name and version
project(BeerDatabase VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SRC "src/*.cpp")
include_directories(inc)

add_executable(beer_database ${SRC})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
