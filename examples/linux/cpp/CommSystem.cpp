#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include <canfestival.h>

#include "Sdo.hpp"
#include "od/commsys.h"

using namespace std;

s_BOARD board = {"can0", "250K"};

class Master {
public:
  Master(CO_Data *d) : _od(d), _init(false), _slaveNode(0) {}

  void init() {
    {
      lock_guard<mutex> l(_mutex);
      setState(_od, Operational);
      _init = true;
    }
  }

  bool isInit() {
    {
      lock_guard<mutex> l(_mutex);
      return _init;
    }
  }

  void setSlaveNodeId(uint8_t nodeId) {
    {
      lock_guard<mutex> l(_mutex);
      _slaveNode = nodeId;
    }
  }

  uint8_t getSlaveNodeId() {
    {
      lock_guard<mutex> l(_mutex);
      return _slaveNode;
    }
  }

  CO_Data *getOdPtr() { return _od; }

private:
  CO_Data *_od;
  bool _init;
  uint8_t _slaveNode;
  mutex _mutex;
};

Master m(&commsys_Data);

namespace master {
void init(CO_Data *d) { printf("Communication System Initialization\n"); }

void preOperational(CO_Data *d) { printf("Preoperattional\n"); }

void operational(CO_Data *d) { printf("Operattional\n"); }

void stopped(CO_Data *d) { printf("Stopped\n"); }

void readManufactureNameCallback(CO_Data *d, UNS8 nodeid) {
  char data[100];
  UNS32 abortCode;
  UNS32 size = 100;

  auto ret =
      getReadResultNetworkDict(m.getOdPtr(), nodeid, data, &size, &abortCode);

  while (ret != SDO_FINISHED) {
    switch (ret) {
    case SDO_ABORTED_RCV:
      fprintf(stderr, "Recv is aborted\n");
      sdo::printSdoAbortCode(abortCode);
      return;
    case SDO_ABORTED_INTERNAL:
      fprintf(stderr, "Internal aborted\n");
      sdo::printSdoAbortCode(abortCode);
      return;
    case SDO_PROVIDED_BUFFER_TOO_SMALL:
      fprintf(stderr, "Provieded buffer is too small\n");
      sdo::printSdoAbortCode(abortCode);
      return;
    default:
      ret = getReadResultNetworkDict(m.getOdPtr(), nodeid, data, &size,
                                     &abortCode);
      break;
    }
  }

  printf("Connected Device Name %s\n", data);
  closeSDOtransfer(m.getOdPtr(), nodeid, SDO_SERVER);
}

void slaveBootUp(CO_Data *d, UNS8 nodeid) {
  printf("Boot Up Slave 0x%02x\n", nodeid);

  if (!m.isInit()) {
    m.init();
  }

  masterSendNMTstateChange(d, nodeid, NMT_Start_Node);

  if (readNetworkDictCallback(m.getOdPtr(), nodeid, 0x1008, 0x00,
                              visible_string, readManufactureNameCallback,
                              0) != 0) {
    fprintf(stderr, "Can't read remote device name\n");
  }
  m.setSlaveNodeId(nodeid);
}

void initNodes(CO_Data *dontUse, UNS32 id) {
  setNodeId(m.getOdPtr(), 0x11);
  setState(m.getOdPtr(), Initialisation);
}

void writeRequestCallback(CO_Data *dontUse, UNS8 nodeid) {
  UNS32 abortCode;

  auto ret = getWriteResultNetworkDict(m.getOdPtr(), nodeid, &abortCode);

  while (ret != SDO_FINISHED) {
    switch (ret) {
    case SDO_ABORTED_RCV:
      fprintf(stderr, "Recv is aborted\n");
      sdo::printSdoAbortCode(abortCode);
      return;
    case SDO_ABORTED_INTERNAL:
      fprintf(stderr, "Internal aborted\n");
      sdo::printSdoAbortCode(abortCode);
      return;
    case SDO_PROVIDED_BUFFER_TOO_SMALL:
      fprintf(stderr, "Provieded buffer is too small\n");
      sdo::printSdoAbortCode(abortCode);
      return;
    default:
      ret = getWriteResultNetworkDict(m.getOdPtr(), nodeid, &abortCode);
      break;
    }
  }
  closeSDOtransfer(m.getOdPtr(), nodeid, SDO_SERVER);
}

void sendRequest(UNS8 nodeId) {
  char data[] = "Hello";

  if (writeNetworkDictCallBack(m.getOdPtr(), nodeId, 0x2001, 0x00, sizeof(data),
                               visible_string, data, writeRequestCallback,
                               0) != 0) {
    fprintf(stderr, "Can't write remote device\n");
  }
}

UNS32 receiveCallback(CO_Data *dontUse, const indextable *index, UNS8 subInd) {
  printf("Received 0x2000\n");
  char data[100];
  UNS32 length = 100;
  UNS8 datatype = visible_string;

  auto ret =
      readLocalDict(m.getOdPtr(), 0x2000, 0x00, data, &length, &datatype, 0);
  if (ret != OD_SUCCESSFUL) {
    fprintf(stderr, "Can't read local dict\n");
    return 0;
  }

  printf("0x2000: Read back %s\n", data);

  return 0;
}

} // namespace master

// Main function
int main(int ac, char **av) {
  if (LoadCanDriver("./libcanfestival_can_socket.so") == NULL) {
    fprintf(stderr, "Unable to load library for can socket\n");
    return 1;
  }

  TimerInit();

  m.getOdPtr()->initialisation = master::init;
  m.getOdPtr()->preOperational = master::preOperational;
  m.getOdPtr()->operational = master::operational;
  m.getOdPtr()->stopped = master::stopped;
  m.getOdPtr()->post_SlaveBootup = master::slaveBootUp;

  if (RegisterSetODentryCallBack(m.getOdPtr(), 0x2000, 0x00,
                                 &master::receiveCallback) != 0) {
    fprintf(stderr, "Can't set callback\n");
    return 1;
  }

  if (!canOpen(&board, m.getOdPtr())) {
    fprintf(stderr, "Can't setup masterboard\n");
    return 1;
  }

  StartTimerLoop(&master::initNodes);

  while (m.getSlaveNodeId() == 0) {
    this_thread::sleep_for(chrono::milliseconds(100));
    if (m.getSlaveNodeId() != 0) {
      master::sendRequest(m.getSlaveNodeId());
      m.setSlaveNodeId(0);
    }
  }

  return 0;
}
