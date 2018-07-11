list(APPEND TestMasterMicroMod_srcs TestMaster.c TestMasterMicroMod.c getopt.c)
add_executable(TestMasterMicroMod ${TestMasterMicroMod_srcs})
target_link_libraries(TestMasterMicroMod canfestival canfestival_unix Threads::Threads ${RT_LIBRARIES} ${DL_LIBRARIES})
