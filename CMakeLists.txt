cmake_minimum_required (VERSION 2.8)

project (safeStream)
enable_testing()

find_package( Threads )
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -std=c++11 -pedantic")

include_directories(include)

add_library(safeStream src/safestream.cpp)

add_library(safeLog include/safelog.hpp)
#SET_TARGET_PROPERTIES(safeLog PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (safeLog safeStream)

add_library(log src/log.cpp)
target_link_libraries (log safeLog)

# Add the executable
add_executable (crazyLogger test/test.cpp)
target_link_libraries (crazyLogger log ${CMAKE_THREAD_LIBS_INIT})

# Does the application run
add_test (logTest crazyLogger)
