cmake_minimum_required (VERSION 2.8)

project (safeStream)
enable_testing()

find_package( Threads )
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -std=c++11 -pedantic")

include_directories(.)

add_library(safeStream safestream.hpp)
SET_TARGET_PROPERTIES(safeStream PROPERTIES LINKER_LANGUAGE CXX)

add_library(safeLog safelog.hpp)
SET_TARGET_PROPERTIES(safeLog PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (safeStream)

add_library(log log.cpp)
SET_TARGET_PROPERTIES(safeLog PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (safeLog)

# add the executable
add_executable (crazyLogger test.cpp)
target_link_libraries (crazyLogger ${CMAKE_THREAD_LIBS_INIT})

# does the application run
add_test (logTest crazyLogger)
