project(canfestival)
cmake_minimum_required(VERSION 3.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/unix)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/timers_unix)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Threads)
find_package(RT)
find_package(DL)

option(CF4L_STATIC_LIB "Build canfestival 4 linux as static lib" OFF)
option(CF4L_BUILD_EXAMPLES "Build the examples" OFF)
option(CF4L_DEBUG_OUTPUT_LIB "Debugging printf outputs from the library" OFF)

set(MAX_CAN_BUS_ID "1" CACHE STRING "Number of can bus to use")
set(SDO_MAX_LENGTH_TRANSFER "32" CACHE STRING "max bytes to transmit by SDO")
set(SDO_BLOCK_SIZE "16" CACHE STRING "max CAN frames transmitted at once for block transfer")
set(SDO_MAX_SIMULTANEOUS_TRANSFERS "4" CACHE STRING "Number of SDO that the node can manage concurrently")
set(NMT_MAX_NODE_ID "128" CACHE STRING "can be reduced to gain memory on small network")
set(SDO_TIMEOUT_MS "3000" CACHE STRING "Timeout in milliseconds for SDO (none to disable the feature")
set(EMCY_MAX_ERRORS "8" CACHE STRING "Max number of active errors managed in error_data structure")
set(DEBUG_WAR_CONSOLE_ON "1" CACHE STRING "enable debug warnings on the ")

foreach(sdoMaxSimTransfer RANGE 1 ${SDO_MAX_SIMULTANEOUS_TRANSFERS})
  set(REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERS_TIMES "repeat ${REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERS_TIMES}")
endforeach(sdoMaxSimTransfer)

foreach(nmtMaxNodeId RANGE 1 ${NMT_MAX_NODE_ID})
  set(REPEAT_NMT_MAX_NODE_ID_TIMES "repeat ${REPEAT_NMT_MAX_NODE_ID_TIMES}")
endforeach(nmtMaxNodeId)

foreach(emcyMaxErrors RANGE 1 ${EMCY_MAX_ERRORS})
  set(REPEAT_EMCY_MAX_ERRORS_TIMES "repeat ${REPEAT_EMCY_MAX_ERRORS_TIMES}")
endforeach(emcyMaxErrors)

configure_file(include/config.h.in config.h @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(cf4ldriver ${CMAKE_CURRENT_SOURCE_DIR}/drivers)
add_library(canfestival_can_socket SHARED ${cf4ldriver}/can_socket.c)

if (CF4L_STATIC_LIB)
    add_library(canfestival_unix STATIC ${cf4ldriver}/unix.c ${cf4ldriver}/timers_unix.c)
else(CF4L_STATIC_LIB)
    add_library(canfestival_unix SHARED ${cf4ldriver}/unix.c ${cf4ldriver}/timers_unix.c)
endif(CF4L_STATIC_LIB)

target_link_libraries(canfestival_unix 
    ${DL_LIBRARIES}
    ${RT_LIBRARIES}
    Threads::Threads
    )

set(cf4lcore ${CMAKE_CURRENT_SOURCE_DIR}/src)
list(APPEND cf4lcore_srcs 
    ${cf4lcore}/objacces.c
    ${cf4lcore}/lifegrd.c
    ${cf4lcore}/sdo.c
    ${cf4lcore}/pdo.c
    ${cf4lcore}/sync.c
    ${cf4lcore}/nmtSlave.c
    ${cf4lcore}/nmtMaster.c
    ${cf4lcore}/states.c
    ${cf4lcore}/timer.c
    ${cf4lcore}/dcf.c
    ${cf4lcore}/emcy.c
    )

if (CF4L_STATIC_LIB)
    add_library(canfestival STATIC ${cf4lcore_srcs})
else(CF4L_STATIC_LIB)
    add_library(canfestival SHARED ${cf4lcore_srcs})
endif(CF4L_STATIC_LIB)

target_link_libraries(canfestival
    ${DL_LIBRARIES}
    )

if (CF4L_DEBUG_OUTPUT_LIB)
    target_compile_definitions(canfestival PRIVATE "DEBUG_WAR_CONSOLE_ON")
endif()

list(APPEND cf4l_INCLUDES
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/unix
    ${CMAKE_CURRENT_SOURCE_DIR}/include/timers_unix
    )
set(cf4l_INCLUDE_DIR ${cf4l_INCLUDES} PARENT_SCOPE)

list(APPEND cf4l_LIBS
    canfestival
    canfestival_unix
    )
set(cf4l_LIBRARIES ${cf4l_LIBS} PARENT_SCOPE)

if (CF4L_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
