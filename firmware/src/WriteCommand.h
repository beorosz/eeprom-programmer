#ifndef WRITECOMMAND_H_
#define WRITECOMMAND_H_

#include <Arduino.h>

struct WriteCommand
{
  uint8_t address_highbyte;
  uint8_t address_lowbyte;
  uint8_t data;
};

#endif