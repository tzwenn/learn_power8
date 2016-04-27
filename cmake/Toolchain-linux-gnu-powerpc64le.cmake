set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_SYSTEM_PROCESSOR powerpc64le)

set(CMAKE_C_COMPILER /opt/gcc-cross/bin/powerpc64le-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/gcc-cross/bin/powerpc64le-linux-gnu-g++)
set(CMAKE_AR /opt/gcc-cross/bin/powerpc64le-linux-gnu-ar CACHE FILEPATH "Archiver")


set(CMAKE_FIND_ROOT_PATH /opt/gcc-cross/powerpc64le-linux-gnu)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

