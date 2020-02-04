cmake_minimum_required(VERSION 3.5)

project(multiwriter LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")
add_executable(multiwriter main.c)