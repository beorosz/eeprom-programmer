#ifndef READCOMMANDHANDLER_H_
#define READCOMMANDHANDLER_H_

#include "../CommandHandler.h"
#include "../../Commands/ReadCommand.h"
#include "../../Responses/Response.h"


class ReadCommandHandler : public CommandHandler
{
public:
    static byte GetCommandByte();
    Response* Handle();
    
private:
    uint8_t ReadData(ReadCommand readCommand);
};

#endif