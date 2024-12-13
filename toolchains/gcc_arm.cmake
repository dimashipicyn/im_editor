
set(CMAKE_C_COMPILER D:/dev/gcc/gcc-arm32/bin/arm-linux-gnueabihf-gcc.exe)
set(CMAKE_CXX_COMPILER D:/dev/gcc/gcc-arm32/bin/arm-linux-gnueabihf-g++.exe)
set(CMAKE_AR D:/dev/gcc/gcc-arm32/bin/arm-linux-gnueabihf-ar.exe)
set(CMAKE_STRIP D:/dev/gcc/gcc-arm32/bin/arm-linux-gnueabihf-strip.exe)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7-a -marm -mfpu=neon -mfloat-abi=hard")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")