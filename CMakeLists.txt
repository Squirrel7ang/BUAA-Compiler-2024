cmake_minimum_required(VERSION 3.10)

# project(tangc)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(bin)

add_subdirectory(libs)

add_subdirectory(tests)
