#add_subdirectory(unix)
#add_subdirectory(can_socket)

add_library(canfestival_can_socket SHARED can_socket.c)
add_library(canfestival_unix SHARED unix.c timers_unix.c)
