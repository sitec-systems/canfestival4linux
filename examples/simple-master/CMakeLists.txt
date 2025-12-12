add_executable(simple-master
  main.c
  simplemaster.c
  simplemaster.h
)

target_link_libraries(simple-master
  canfestival
  canfestival_unix
  Threads::Threads
  ${RT_LIBRARIES}
  ${DL_LIBRARIES}
)
