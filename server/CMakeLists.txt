cmake_minimum_required(VERSION 3.5)

project(massivereader LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")

set(SOURCES
        main.c
        tools.c server.c time_lib.c)

set(HEADERS
        server.h
        tools.h
        time_lib.h)


add_executable(massivereader ${SOURCES} ${HEADERS})