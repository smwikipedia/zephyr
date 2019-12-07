cmake_minimum_required(VERSION 3.13.1)
project("Zephyr Kernel Unit Test with Cmocka (stack.c)" LANGUAGES C VERSION 0.1)

macro(assert test comment)
  if(NOT ${test})
    message(FATAL_ERROR "Assertion failed: ${comment}")
  endif()
endmacro()

set(test_name "stack_test")
set(CMAKE_BUILD_TYPE "Debug") # only this way can we debug it

#must provide -Dzephyr_dir when configuring the project
assert(zephyr_dir "<zephyr_dir> must be defined first. It is the full path of zephyr src tree.")
set(mock_engine_dir "${zephyr_dir}/tests/unit2/MockEngine")
set(cmocka_dir "${zephyr_dir}/tests/unit2/cmocka-1.1.5")

#specify test sources
#don't build mocks.c directly, build mocks.c as included into the stack_test.c.
#becuase there's no includes in mocks.c, types used in mocks.c are only valid in the context of stack_test.c
set(test_sources "${test_name}.c")

#set compilation flags for sources
set(zephyr_c_flags "\
 -imacros${zephyr_dir}/build/hello_world_qemu_x86_64/zephyr/include/generated/autoconf.h\
 -imacros${zephyr_dir}/include/toolchain/zephyr_stdint.h")
set(zephyr_includes "\
 -I${zephyr_dir}/kernel/include\
 -I${zephyr_dir}/arch/x86/include\
 -I${zephyr_dir}/include\
 -I${zephyr_dir}/include/drivers\
 -I${zephyr_dir}/build/hello_world_qemu_x86_64/zephyr/include\
 -I${zephyr_dir}/build/hello_world_qemu_x86_64/zephyr/include/generated\
 -I${zephyr_dir}/soc/x86/ia32")

set(mockengine_includes "-I${mock_engine_dir}")
set(zephyr_kernel_dir "-I${zephyr_dir}/kernel") #include "stack.c"
set(cmocka_header_dir "-I${cmocka_dir}/include") #include "cmocka.h"

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${zephyr_c_flags} ${zephyr_includes} ${zephyr_kernel_dir} ${cmocka_header_dir} ${mockengine_includes}")

set(libcmocka "${CMAKE_CURRENT_BINARY_DIR}/../../cmocka-1.1.5/src/libcmocka.so")
set(libMockEngine "${CMAKE_CURRENT_BINARY_DIR}/../../MockEngine/libMockEngine.a")

#create test executable
add_executable(${test_name} ${test_sources})

#set additional link libraries
target_link_libraries(${test_name} ${libcmocka} ${libMockEngine})

