#ifndef READCOMMAND_H_
#define READCOMMAND_H_

#include <Arduino.h>

struct ReadCommand
{
  uint8_t address_highbyte;
  uint8_t address_lowbyte;
};

#endif