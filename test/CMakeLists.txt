cmake_minimum_required(VERSION 3.7)

project(test-ctrl-c)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} "main.cpp" "../src/ctrl-c.h" "../src/ctrl-c.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE "../src")
