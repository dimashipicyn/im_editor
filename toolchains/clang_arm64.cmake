include($ENV{SOURCE_DIR}/toolchains/toolchain.cmake)

set(CMAKE_SYSROOT ${SYSROOT_ARM64})

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_AR llvm-ar)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target aarch64-linux-gnu -march=armv8-a")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")

include_directories(
    "${CMAKE_SYSROOT}/usr/include/aarch64-linux-gnu/c++/10"
    "${CMAKE_SYSROOT}/usr/include/c++/10"
    "${CMAKE_SYSROOT}/usr/include"
)

set(TARGET_ARCHITECTURE arm64)
