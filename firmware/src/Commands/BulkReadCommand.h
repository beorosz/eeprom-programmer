#ifndef BULKREADCOMMAND_H_
#define BULKREADCOMMAND_H_

#include <Arduino.h>

struct BulkReadCommand
{
  uint16_t address;
  byte dataLength;
};

#endif