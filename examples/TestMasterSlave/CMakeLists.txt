# Assume that the OD Files are pre generated before the build
# The generated files are TestMaster.c and TestSlave.c

add_executable(TestMasterSlave TestMasterSlave.c Slave.c Master.c TestMaster.c TestSlave.c)
target_link_libraries(TestMasterSlave canfestival canfestival_unix Threads::Threads ${RT_LIBRARIES} ${DL_LIBRARIES})
