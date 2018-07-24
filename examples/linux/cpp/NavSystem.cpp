#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

#include <canfestival.h>

#include "Sdo.hpp"
#include "od/navsystem.h"

using namespace std;

s_BOARD board = {"can1", "250K"};

class Slave {
public:
  Slave(CO_Data *d) : _od(d), _requestRecv(false) {}

  CO_Data *getOdPtr() { return _od; }

  void setRequestRecv(bool val) {
    lock_guard<mutex> l(_mutex);
    _requestRecv = val;
  }

  bool isRequestRecv() {
    lock_guard<mutex> l(_mutex);
    return _requestRecv;
  }

private:
  CO_Data *_od;
  bool _requestRecv;
  mutex _mutex;
};

Slave s(&navsystem_Data);

namespace slave {
void init(CO_Data *d) { printf("Navigation System Initialization\n"); }

void preOperational(CO_Data *d) { printf("Preoperattional\n"); }

void operational(CO_Data *d) { printf("Operattional\n"); }

void stopped(CO_Data *d) { printf("Stopped\n"); }

void initNodes(CO_Data *dontUse, UNS32 id) {
  setNodeId(s.getOdPtr(), 0x10);
  setState(s.getOdPtr(), Initialisation);
}

UNS32 receiveCallback(CO_Data *dontUse, const indextable *index, UNS8 subInd) {
  printf("Received 0x2001\n");
  char data[100];
  UNS32 length = 100;
  UNS8 datatype = visible_string;

  auto ret =
      readLocalDict(s.getOdPtr(), 0x2001, 0x00, data, &length, &datatype, 0);
  if (ret != OD_SUCCESSFUL) {
    fprintf(stderr, "Can't read local dict\n");
    return 0;
  }

  printf("0x2001: Read %s\n", data);

  s.setRequestRecv(true);

  return 0;
}

void writeResponseCallback(CO_Data *dontUse, UNS8 nodeid) {
  UNS32 abortCode;

  auto ret = getWriteResultNetworkDict(s.getOdPtr(), nodeid, &abortCode);

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
      ret = getWriteResultNetworkDict(s.getOdPtr(), nodeid, &abortCode);
      break;
    }
  }
  closeSDOtransfer(s.getOdPtr(), nodeid, SDO_SERVER);
}

void sendResponse(UNS8 nodeId) {
  printf("Sending response\n");
  char data[] = " World!!";

  if (writeNetworkDictCallBack(s.getOdPtr(), 0x11, 0x2000, 0x00, sizeof(data),
                               visible_string, data, writeResponseCallback,
                               0) != 0) {
    fprintf(stderr, "Can't write to commsystem");
  }
}

} // namespace slave

int main(int ac, char **av) {
  if (LoadCanDriver("./libcanfestival_can_socket.so") == NULL) {
    fprintf(stderr, "Unable to load library for can socket\n");
    return 1;
  }

  TimerInit();

  s.getOdPtr()->initialisation = slave::init;
  s.getOdPtr()->preOperational = slave::preOperational;
  s.getOdPtr()->operational = slave::operational;
  s.getOdPtr()->stopped = slave::stopped;

  if (RegisterSetODentryCallBack(s.getOdPtr(), 0x2001, 0x00,
                                 &slave::receiveCallback) != 0) {
    fprintf(stderr, "Can't set callback\n");
    return 1;
  }

  if (!canOpen(&board, s.getOdPtr())) {
    fprintf(stderr, "Can't setup slave board\n");
    return 1;
  }

  StartTimerLoop(&slave::initNodes);

  for (;;) {
    this_thread::sleep_for(chrono::milliseconds(100));
    if (s.isRequestRecv()) {
      slave::sendResponse(0x11);
      s.setRequestRecv(false);
    }
  }

  return 0;
}