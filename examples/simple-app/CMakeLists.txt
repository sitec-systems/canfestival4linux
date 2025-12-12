add_executable(simple-app
  main.c
  simpleapp.c
  simpleapp.h
)

target_link_libraries(simple-app
  canfestival
  canfestival_unix
  Threads::Threads
  ${RT_LIBRARIES}
  ${DL_LIBRARIES}
)
