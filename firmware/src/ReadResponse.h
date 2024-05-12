#ifndef READRESPONSE_H_
#define READRESPONSE_H_

#include <Arduino.h>
#include "Response.h"

class ReadResponse : public Response
{
public:
    uint8_t data;
};

#endif