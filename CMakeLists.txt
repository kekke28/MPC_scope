cmake_minimum_required(VERSION 2.8)

project(test CXX)

set(CMAKE_CXX_FLAGS "-g -maes -msse4")

add_subdirectory(header)
add_subdirectory(src)
add_subdirectory(test)