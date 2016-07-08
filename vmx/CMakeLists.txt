file(GLOB altivecmm_files
		"altivecmm/*.h"
		"altivecmm/detail/*.h")

add_executable(test1 test1.c)
add_executable(test2 test2.cpp ${altivecmm_files})
add_executable(parsum parsum.cpp)
add_executable(instruction_test instruction_test.cpp ${altivecmm_files})
