cmake_minimum_required(VERSION 3.26)
project(zylib LANGUAGES C)

set(CMAKE_C_STANDARD 23)

enable_testing()

set(SOURCES include/zylib_allocator.h
        src/zylib_allocator.c
        include/zylib_def.h
        include/zylib_dequeue.h
        src/zylib_dequeue.c
        include/zylib_log.h
        src/zylib_log.c
        include/zylib_error.h
        src/zylib_error.c)

add_library(zylib STATIC ${SOURCES})
target_include_directories(zylib PUBLIC include PRIVATE src/lib/include)

add_subdirectory(test)