cmake_minimum_required(VERSION 3.13)
project(CPP-Heap)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O0 -g")

add_executable(cpp-heap heap.h heap.cpp main.cpp)
