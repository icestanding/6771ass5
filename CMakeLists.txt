cmake_minimum_required(VERSION 3.6)
project(ass5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp BucketSort.h BucketSort.cpp)
add_executable(ass5 ${SOURCE_FILES})
