list(APPEND CANOpenShell_srcs CANOpenShell.c CANOpenShellMasterOD.c CANOpenShellSlaveOD.c)
add_executable(CANOpenShell ${CANOpenShell_srcs})
target_link_libraries(CANOpenShell canfestival canfestival_unix Threads::Threads ${RT_LIBRARIES} ${DL_LIBRARIES})
