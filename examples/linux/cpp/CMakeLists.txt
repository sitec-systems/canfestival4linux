list(APPEND CommSystem_srcs CommSystem.cpp od/commsys.c)
list(APPEND NavSystem_srcs NavSystem.cpp od/navsystem.c)
list(APPEND SdoUtil_srcs Sdo.cpp)

set(CMAKE_CXX_STANDARD 14)

add_library(SdoUtil STATIC ${SdoUtil_srcs})

add_executable(CommSystem ${CommSystem_srcs})
target_link_libraries(CommSystem 
    SdoUtil
    canfestival
    canfestival_unix
    Threads::Threads
    ${RT_LIBRARIES}
    ${DL_LIBRARIES})


add_executable(NavSystem ${NavSystem_srcs})
target_link_libraries(NavSystem 
    SdoUtil
    canfestival
    canfestival_unix
    Threads::Threads
    ${RT_LIBRARIES}
    ${DL_LIBRARIES})
