project(learn_power8)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

set(ARCH_FLAGS "-mcpu=power8 -m64 -mvsx -fgnu-tm")

set(EXTRA_FLAGS "-O2 -Wall")

set(CMAKE_C_FLAGS "-std=c99 ${ARCH_FLAGS} ${EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=gnu++11 ${ARCH_FLAGS} ${EXTRA_FLAGS}")

file(GLOB files_h
		"altivecmm/*.h"
		"altivecmm/detail/*.h")

add_executable(test1 test1.c)
add_executable(test2 test2.cpp ${files_h})
add_executable(parsum parsum.cpp)
add_executable(stm stm.cpp
	stm4pwr.cpp
	stm4pwr.h
	ThreadPool.cpp
	ThreadPool.h)
