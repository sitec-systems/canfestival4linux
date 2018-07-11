project(canfestival)
cmake_minimum_required(VERSION 3.3)

include_directories(include)
include_directories(include/unix)
include_directories(include/timers_unix)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Threads)
find_package(RT)
find_package(DL)

add_subdirectory(drivers)
add_subdirectory(src)
add_subdirectory(examples)
