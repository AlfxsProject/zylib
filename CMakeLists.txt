cmake_minimum_required(VERSION 3.26)
project(zylib LANGUAGES C)

set(CMAKE_C_STANDARD 23)

enable_testing()

set(SOURCES public/include/zylib_allocator.h
        public/src/zylib_allocator.c
        public/include/zylib_def.h
        public/include/zylib_dequeue.h
        public/src/zylib_dequeue.c
        public/include/zylib_log.h
        public/src/zylib_log.c
        public/include/zylib_error.h
        public/src/zylib_error.c
        private/include/zylib_private_box.h
        private/src/zylib_private_box.c
        private/include/zylib_private_allocator.h
        private/src/zylib_private_allocator.c
        private/include/zylib_private_dequeue.h
        private/src/zylib_private_dequeue.c)

add_library(zylib STATIC ${SOURCES})
target_include_directories(zylib PUBLIC public/include PRIVATE private/include)

add_subdirectory(test)