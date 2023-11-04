cmake_minimum_required(VERSION 3.26)
project(zylib C)

set(CMAKE_C_STANDARD 23)

enable_testing()

add_compile_definitions(DO_MUTEX DO_PTHREAD)

set(SOURCES include/zylib_alloc.h
        src/zylib_alloc.c
        include/zylib_def.h
        include/zylib_dequeue.h
        src/zylib_dequeue.c
        include/zylib_log.h
        src/zylib_log.c
        include/zylib_thread.h
        src/zylib_thread.c
        include/zylib_error.h
        src/zylib_error.c)

add_library(zylib STATIC ${SOURCES})
target_include_directories(zylib PUBLIC include)

add_subdirectory(test)