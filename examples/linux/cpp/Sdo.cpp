#include "Sdo.hpp"

#include <canfestival.h>

namespace sdo {

void printSdoAbortCode(UNS32 abortCode) {
  switch (abortCode) {
  case SDOABT_TOGGLE_NOT_ALTERNED:
    fprintf(stderr, "Toogle not alterned\n");
    break;
  case SDOABT_TIMED_OUT:
    fprintf(stderr, "SDO Timed out\n");
    break;
  case SDOABT_CS_NOT_VALID:
    fprintf(stderr, "SDO CS not valid\n");
    break;
  case SDOABT_INVALID_BLOCK_SIZE:
    fprintf(stderr, "SDO invalid block size\n");
    break;
  case SDOABT_OUT_OF_MEMORY:
    fprintf(stderr, "Out of memory\n");
    break;
  case SDOABT_GENERAL_ERROR:
    fprintf(stderr, "General error\n");
    break;
  case SDOABT_LOCAL_CTRL_ERROR:
    fprintf(stderr, "Local ctrl error\n");
    break;
  case OD_SUCCESSFUL:
    fprintf(stderr, "Success\n");
    break;
  case OD_READ_NOT_ALLOWED:
    fprintf(stderr, "Read not allowed\n");
    break;
  case OD_WRITE_NOT_ALLOWED:
    fprintf(stderr, "Write not allowd\n");
    break;
  case OD_NO_SUCH_OBJECT:
    fprintf(stderr, "No such object\n");
    break;
  case OD_NOT_MAPPABLE:
    fprintf(stderr, "Object not mappable\n");
    break;
  case OD_ACCES_FAILED:
    fprintf(stderr, "Access failed\n");
    break;
  case OD_LENGTH_DATA_INVALID:
    fprintf(stderr, "Invalid data length\n");
    break;
  case OD_NO_SUCH_SUBINDEX:
    fprintf(stderr, "No such subindex\nh");
    break;
  case OD_VALUE_RANGE_EXCEEDED:
    fprintf(stderr, "Value range exceeded\n");
    break;
  case OD_VALUE_TOO_LOW:
    fprintf(stderr, "Value too low\n");
    break;
  case OD_VALUE_TOO_HIGH:
    fprintf(stderr, "Value too high\n");
    break;
  default:
    fprintf(stderr, "Unknown error\n");
    break;
  }
}

} // namespace sdo