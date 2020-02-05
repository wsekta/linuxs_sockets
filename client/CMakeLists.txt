cmake_minimum_required(VERSION 3.5)

project(multiwriter LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")

set(SOURCES
        main.c
        tools.c local_server.c inet_sock.c)

set(HEADERS
        tools.h
        local_server.h inet_sock.h timer.h)

add_executable(multiwriter ${SOURCES} ${HEADERS})