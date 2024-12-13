include($ENV{SOURCE_DIR}/toolchains/toolchain.cmake)

set(CMAKE_SYSROOT ${SYSROOT_ARM32})

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_AR llvm-ar)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target arm-linux-gnueabihf")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")

set(TARGET_ARCHITECTURE arm32)