project(learn_power8)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

set(ARCH_FLAGS "-mcpu=power8 -m64 -mvsx -fgnu-tm")

set(EXTRA_FLAGS "-O2 -Wall")

set(CMAKE_C_FLAGS "-std=c99 ${ARCH_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=gnu++11 ${ARCH_FLAGS} ${EXTRA_FLAGS}")

add_subdirectory(vmx)
add_subdirectory(stm)
