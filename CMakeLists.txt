cmake_minimum_required(VERSION 3.22)

set(PROJECT_N serialport)
project(${PROJECT_N} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(cpp)

target_include_directories(${PROJECT_N} PUBLIC include)
