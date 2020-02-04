cmake_minimum_required(VERSION 3.5)

project(massivereader LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")
add_executable(massivereader main.c)