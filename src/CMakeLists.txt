list(APPEND canfestival_srcs
    objacces.c
    lifegrd.c
    sdo.c
    pdo.c
    sync.c
    nmtSlave.c
    nmtMaster.c
    states.c
    timer.c
    dcf.c
    emcy.c
    )

add_library(canfestival SHARED ${canfestival_srcs})
