project(stm)

set(STM_FLAGS "-fgnu-tm")

set(CMAKE_C_FLAGS "${STM_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${STM_FLAGS} ${CMAKE_CXX_FLAGS}")

add_executable(stm stm.cpp
	stm4pwr.cpp
	stm4pwr.h
	ThreadPool.cpp
	ThreadPool.h)
