cmake_minimum_required(VERSION 3.18)
project(BigFilesSorter)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(source)
