cmake_minimum_required(VERSION 3.13.1)
project("Zephyr Kernel Unit Test with Cmocka" LANGUAGES C VERSION 0.1)

add_subdirectory(cmocka-1.1.5)
add_subdirectory(MockEngine)
add_subdirectory(kernel/stack_test)
