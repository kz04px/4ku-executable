cmake_minimum_required(VERSION 3.10)

# Project
project(4ku VERSION 1.0 LANGUAGES CXX)

include_directories(../)

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Os -s -Wall -Wextra -Wshadow -flto -fuse-linker-plugin -fno-plt -fno-stack-protector -fno-stack-check -fno-unwind-tables -fno-asynchronous-unwind-tables -fomit-frame-pointer -fno-exceptions -fno-rtti -fno-threadsafe-statics -fdata-sections -ffunction-sections -fvisibility=hidden -fvisibility-inlines-hidden -fno-inline-small-functions -Wl,--gc-sections    -Wl,--build-id=none -z norelro -Wl,--no-eh-frame-hdr -no-pie -Wl,--no-ld-generated-unwind-info -Wl,--hash-style=sysv -Wl,-z,nodynamic-undefined-weak -Wl,--gc-sections -Bsymbolic -Wl,--exclude-libs,ALL")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add engine executable
add_executable(
    4ku

    main.cpp
    search.cpp
    time.cpp

    # Chess
    ../chess/attack.cpp
    ../chess/makemove.cpp
    ../chess/movegen.cpp

    # UCI
    uci/go.cpp
    uci/isready.cpp
    uci/listen.cpp
)
