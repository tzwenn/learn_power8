project(learn_altivec)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

file(GLOB files_h "*.h")

set(CMAKE_C_FLAGS "-std=c99 -mcpu=power8 -m64 -mvsx -O2")
set(CMAKE_CXX_FLAGS "-std=gnu++11 -mcpu=power8 -m64 -lpthread -mvsx -O2 -Wall")

add_executable(test1 test1.c)
add_executable(test2 test2.cpp ${files_h})
add_executable(parsum parsum.cpp)

