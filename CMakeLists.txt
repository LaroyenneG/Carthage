cmake_minimum_required(VERSION 3.9)
project(gl_library)

set(CMAKE_C_STANDARD 99)
add_definitions(-c -Wall)

add_executable(gl_library
        src/debug.c
        src/debug.h
        src/fifo.c
        src/fifo.h
        src/list.c
        src/list.h
        src/log.c
        src/log.h
        src/map.c
        src/map.h
        src/scanner.c
        src/scanner.h
        src/sublime_term.c
        src/sublime_term.h
        src/thread_lib.c
        src/thread_lib.h
        src/vector.c
        src/vector.h
        test/main.c)

target_link_libraries(gl_library pthread)