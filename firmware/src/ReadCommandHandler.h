#ifndef READCOMMANDHANDLER_H_
#define READCOMMANDHANDLER_H_

#include "CommandHandler.h"
#include "ReadCommand.h"
#include "ReadResponse.h"


class ReadCommandHandler : public CommandHandler
{
public:
    Response* Handle();
private:
    uint8_t ReadData(ReadCommand readCommand);
};

#endif