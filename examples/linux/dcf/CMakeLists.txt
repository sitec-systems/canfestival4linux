list(APPEND dcf_master_srcs gendcf.c master.c masterdic.c)
list(APPEND dcf_slave_srcs slave.c slavedic.c)

add_executable(dcf_master ${dcf_master_srcs})
add_executable(dcf_slave ${dcf_slave_srcs})

target_link_libraries(dcf_master canfestival canfestival_unix Threads::Threads ${RT_LIBRARIES} ${DL_LIBRARIES})
target_link_libraries(dcf_slave canfestival canfestival_unix Threads::Threads ${RT_LIBRARIES} ${DL_LIBRARIES})
