project(canfestival)
cmake_minimum_required(VERSION 3.3)

include_directories(include)
include_directories(include/unix)
include_directories(include/timers_unix)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Threads)
find_package(RT)
find_package(DL)

set(MAX_CAN_BUS_ID "1" CACHE STRING "Number of can bus to use")
set(SDO_MAX_LENGTH_TRANSFER "32" CACHE STRING "max bytes to transmit by SDO")
set(SDO_BLOCK_SIZE "16" CACHE STRING "max CAN frames transmitted at once for block transfer")
set(SDO_MAX_SIMULTANEOUS_TRANSFERS "4" CACHE STRING "Number of SDO that the node can manage concurrently")
set(NMT_MAX_NODE_ID "128" CACHE STRING "can be reduced to gain memory on small network")
set(SDO_TIMEOUT_MS "3000" CACHE STRING "Timeout in milliseconds for SDO (none to disable the feature")
set(EMCY_MAX_ERRORS "8" CACHE STRING "Max number of active errors managed in error_data structure")

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

add_subdirectory(drivers)
add_subdirectory(src)
add_subdirectory(examples)
