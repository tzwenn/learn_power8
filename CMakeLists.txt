project(learn_power8)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

set(ARCH_FLAGS "-mcpu=power8 -m64 -mvsx -mabi=altivec")

set(EXTRA_FLAGS "-fopenmp -O0 -Wall")

set(CMAKE_C_FLAGS "-std=c99 ${ARCH_FLAGS} ${EXTRA_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=gnu++11 ${ARCH_FLAGS} ${EXTRA_FLAGS} ${CMAKE_CXX_FLAGS}")

add_subdirectory(vmx)
add_subdirectory(htm)
add_subdirectory(crypt)
