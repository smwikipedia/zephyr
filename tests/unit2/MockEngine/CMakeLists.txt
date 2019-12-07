cmake_minimum_required(VERSION 3.13.1)
project("Mock Engine Lib" LANGUAGES C VERSION 0.1)

# add the -g flag so we can debug it
set(CMAKE_BUILD_TYPE "Debug")

# specify the source and lib name
set(sources "MockEngine.c")
set(libname "MockEngine")

set(mock_engine_c_flags "") # just in case
set(mock_engine_includes "\
 -I.\
 -I./subhook")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${mock_engine_c_flags} ${mock_engine_includes}")

# create the library target
add_library(${libname} STATIC ${sources})

