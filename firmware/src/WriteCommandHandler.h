#ifndef WRITECOMMANDHANDLER_H_
#define WRITECOMMANDHANDLER_H_

#include "CommandHandler.h"
#include "WriteCommand.h"
#include "Response.h"


class WriteCommandHandler : public CommandHandler
{
public:
    Response* Handle();

private:
    void WriteData(WriteCommand writeCommand);
    void SetData(uint8_t data);
};

#endif