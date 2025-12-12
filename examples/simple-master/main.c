#include <stdint.h>
#include <stdio.h>

#include "canfestival.h"
#include "data.h"
#include "def.h"
#include "nmtMaster.h"
#include "sdo.h"
#include "simplemaster.h"
#include "states.h"

void simplemaster_heartbeatError(CO_Data *data, uint8_t heartbeat_id) {
    (void)data;
    printf("heartbeat error for heartbeat id 0x%02x\n", heartbeat_id);
}

static void simplemaster_initialisation(CO_Data *data) {
    (void)data;
    printf("initialisation\n");
}

static void simplemaster_preOperational(CO_Data *data) {
    (void)data;
    printf("preoperation\n");
}

static void simplemaster_operational(CO_Data *data) {
    (void)data;
    printf("operational\n");
}

static void simplemaster_stopped(CO_Data *data) {
    (void)data;
    printf("stopped\n");
}

static uint32_t simplemaster_storeODSubIndex(CO_Data *data, uint16_t index,
                                             uint8_t sub_index) {
    (void)data;
    printf("TestSlave_storeODSubIndex : %4.4x %2.2x\n", index, sub_index);
    return 0;
}

static void simplemaster_init_node(CO_Data *data, uint32_t id) {
    printf("enter init\n");
    setNodeId(&simplemaster_Data, 0x3a);
    setState(&simplemaster_Data, Initialisation);
    printf("initialisation finished\n");
    setState(&simplemaster_Data, Pre_operational);
    printf("pre operational finished\n");
    setState(&simplemaster_Data, Operational);
    printf("operational finished\n");
}

static void simplemaster_sdo_write_callback(CO_Data *data, uint8_t node_id) {
    uint32_t abort_code = {0};
    uint8_t result = 0;

    result = getWriteResultNetworkDict(data, node_id, &abort_code);
    if (result == SDO_FINISHED) {
        printf("transfer was finished with abort code 0x%04x\n", abort_code);
        return;
    }

    if (result == SDO_ABORTED_RCV || result == SDO_ABORTED_INTERNAL) {
        printf("transfer was aborted with 0x%04x\n", abort_code);
        return;
    }

    printf("transfer is still in progress\n");
}

static void simplemaster_post_slave_bootup(CO_Data *data, uint8_t node_id) {
    printf("bootup of slave 0x%02x\n", node_id);

    masterSendNMTstateChange(data, node_id, NMT_Start_Node);

    char dns_address[] = "1.1.1.1";

    uint8_t result = writeNetworkDictCallBack(
        data, node_id, 0x500b, 0x00, 20, domain, dns_address,
        simplemaster_sdo_write_callback, 0);
    if (result != 0) {
        printf("error writing network dict of 0x%02x: 0x%02x\n", node_id,
               result);
        return;
    }

    printf("writing network dict is finished\n");
}

int main(int argc, char **argv) {
    printf("hello world\n");

    s_BOARD board = {
        .busname = "can0",
        .baudrate = "250k",
    };

    TimerInit();

    if (LoadCanDriver("/home/root/libcanfestival_can_socket.so") == NULL) {
        fprintf(stderr, "can't load library\n");
        return -1;
    }

    simplemaster_Data.heartbeatError = simplemaster_heartbeatError;
    simplemaster_Data.initialisation = simplemaster_initialisation;
    simplemaster_Data.preOperational = simplemaster_preOperational;
    simplemaster_Data.operational = simplemaster_operational;
    simplemaster_Data.stopped = simplemaster_stopped;
    simplemaster_Data.storeODSubIndex = simplemaster_storeODSubIndex;
    simplemaster_Data.post_SlaveBootup = simplemaster_post_slave_bootup;

    printf("open can\n");
    if (!canOpen(&board, &simplemaster_Data)) {
        fprintf(stderr, "can't open can \n");
        return -1;
    }
    printf("can is open\n");

    // Start timer thread
    StartTimerLoop(&simplemaster_init_node);
    printf("timer is started\n");

    for (;;) {
    }

    return 0;
}
