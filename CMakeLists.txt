cmake_minimum_required(VERSION 3.13)
project(CPP-Heap)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O3 -fno-exceptions -fno-rtti")

add_executable(cpp-heap debug.h debug.cpp heap.h heap.cpp main.cpp)
