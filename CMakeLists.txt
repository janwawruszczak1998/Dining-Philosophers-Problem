cmake_minimum_required(VERSION 3.15)
project(Dining_Philosophers)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(Dining_Philosophers main.cpp Philosopher.h)