add_executable(allocator src/test_zylib_allocator.c)
target_link_libraries(allocator zylib)

add_executable(dequeue src/test_zylib_dequeue.c)
target_link_libraries(dequeue zylib)

add_executable(error src/test_zylib_error.c)
target_link_libraries(error zylib)

add_executable(box src/test_zylib_private_box.c)
target_link_libraries(box zylib)
target_include_directories(box PRIVATE ../private/include)

add_test(NAME allocator COMMAND allocator)
add_test(NAME dequeue COMMAND dequeue)
add_test(NAME error COMMAND error)
add_test(NAME box COMMAND box)