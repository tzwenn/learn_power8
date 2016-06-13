file(GLOB files_h
		"altivecmm/*.h"
		"altivecmm/detail/*.h")

add_executable(test1 test1.c)
add_executable(test2 test2.cpp ${files_h})
add_executable(parsum parsum.cpp)
