list(APPEND DS401_Master_srcs getopt.c TestMaster.c TestMasterMicroMod.c)
add_executable(DS401_Master ${DS401_Master_srcs})
target_link_libraries(DS401_Master canfestival canfestival_unix Threads::Threads ${RT_LIBRARIES} ${DL_LIBRARIES})
