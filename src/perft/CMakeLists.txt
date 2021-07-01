cmake_minimum_required(VERSION 3.10)

# Project
project(perft VERSION 1.0 LANGUAGES CXX)

include_directories(../)

# Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Os -s -Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the test executable
add_executable(
    perft

    main.cpp

    # Chess
    ../chess/attack.cpp
    ../chess/makemove.cpp
    ../chess/movegen.cpp
    ../chess/perft.cpp
    ../chess/set_fen.cpp
    ../chess/undomove.cpp
)
