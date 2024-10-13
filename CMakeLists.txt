cmake_minimum_required(VERSION 3.10)

project(Compiler)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(bin)

add_subdirectory(libs)