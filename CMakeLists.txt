cmake_minimum_required(VERSION 3.25)

set(PROJECT_N serialport)
project(${PROJECT_N} VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

target_include_directories(${PROJECT_N} PUBLIC include)
