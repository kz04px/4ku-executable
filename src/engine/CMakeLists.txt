cmake_minimum_required(VERSION 3.10)

# Project
project(4ku VERSION 1.0 LANGUAGES CXX)

include_directories(../)

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Os -s -Wall -Wextra -Wshadow -flto -fno-exceptions -fdata-sections -ffunction-sections -fvisibility=hidden -fvisibility-inlines-hidden -Wl,--gc-sections -Bsymbolic -Wl,--exclude-libs,ALL -fno-inline-small-functions")
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

    # Chess
    ../chess/attack.cpp
    ../chess/makemove.cpp
    ../chess/movegen.cpp
    ../chess/undomove.cpp

    # UCI
    uci/go.cpp
    uci/isready.cpp
    uci/listen.cpp
    uci/moves.cpp

    # Eval
    eval/eval.cpp

    # Search
    search/alphabeta.cpp
)
