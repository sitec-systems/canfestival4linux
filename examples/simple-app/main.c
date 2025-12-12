#include <stdint.h>
#include <stdio.h>

#include "canfestival.h"
#include "data.h"
#include "simpleapp.h"

void simpleapp_heartbeatError(CO_Data *data, uint8_t heartbeat_id) {
    (void)data;
    printf("heartbeat error for heartbeat id 0x%02x\n", heartbeat_id);
}

static void simpleapp_initialisation(CO_Data *data) {
    (void)data;
    printf("initialisation\n");
}

static void simpleapp_preOperational(CO_Data *data) {
    (void)data;
    printf("preoperation\n");
}

static void simpleapp_operational(CO_Data *data) {
    (void)data;
    printf("operational\n");
}

static void simpleapp_stopped(CO_Data *data) {
    (void)data;
    printf("stopped\n");
}

static uint32_t simpleapp_storeODSubIndex(CO_Data *data, uint16_t index,
                                          uint8_t sub_index) {
    (void)data;
    printf("TestSlave_storeODSubIndex : %4.4x %2.2x\n", index, sub_index);
    return 0;
}

static void simpleapp_init_node(CO_Data *data, uint32_t id) {
    printf("enter init\n");
    setNodeId(&simpleapp_Data, 0x3b);
    setState(&simpleapp_Data, Initialisation);
    printf("initialisation finished\n");
}

int main(int argc, char **argv) {
    printf("hello world\n");

    s_BOARD board = {
        .busname = "can1",
        .baudrate = "250k",
    };

    TimerInit();

    if (LoadCanDriver("/home/root/libcanfestival_can_socket.so") == NULL) {
        fprintf(stderr, "can't load library\n");
        return -1;
    }

    simpleapp_Data.heartbeatError = simpleapp_heartbeatError;
    simpleapp_Data.initialisation = simpleapp_initialisation;
    simpleapp_Data.preOperational = simpleapp_preOperational;
    simpleapp_Data.operational = simpleapp_operational;
    simpleapp_Data.stopped = simpleapp_stopped;
    simpleapp_Data.storeODSubIndex = simpleapp_storeODSubIndex;

    printf("open can\n");
    if (!canOpen(&board, &simpleapp_Data)) {
        fprintf(stderr, "can't open can \n");
        return -1;
    }
    printf("can is open\n");

    // Start timer thread
    StartTimerLoop(&simpleapp_init_node);
    printf("timer is started\n");

    for (;;) {
    }

    return 0;
}
