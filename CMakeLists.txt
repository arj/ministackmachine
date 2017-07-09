cmake_minimum_required(VERSION 3.6)
project(stackmachine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp instructions.h instructions.cpp interpreter.cpp interpreter.h)
add_executable(stackmachine ${SOURCE_FILES})