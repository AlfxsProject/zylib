add_executable(alloc src/alloc.c)
target_link_libraries(alloc zylib)

add_executable(dequeue src/dequeue.c)
target_link_libraries(dequeue zylib)

add_executable(error src/error.c)
target_link_libraries(error zylib)

add_executable(box src/box.c)
target_link_libraries(box zylib)
target_include_directories(box PRIVATE ../src/lib/include)

add_test(NAME alloc COMMAND alloc)
add_test(NAME dequeue COMMAND dequeue)
add_test(NAME error COMMAND error)
add_test(NAME box COMMAND box)