cmake_minimum_required(VERSION 3.5)

project(massivereader LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")

set(SOURCES
        main.c
        tools.c server.c)

set(HEADERS
        server.h
        tools.h
)


add_executable(massivereader ${SOURCES} ${HEADERS})