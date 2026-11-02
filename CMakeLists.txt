cmake_minimum_required(VERSION 4.4)
project(alignedVector)

set(CMAKE_CXX_STANDARD 20)

add_executable(alignedVector main.cpp
        AlignedVector.cpp
        AlignedVector.h)
