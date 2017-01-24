project(stm)

set(CMAKE_C_FLAGS "${STM_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${STM_FLAGS} ${CMAKE_CXX_FLAGS}")

add_executable(htm htm.cpp
	htm4pwr.cpp
	htm4pwr.h
	ThreadPool.cpp
	ThreadPool.h)
