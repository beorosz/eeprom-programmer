#ifndef WRITECOMMANDHANDLER_H_
#define WRITECOMMANDHANDLER_H_

#include "../CommandHandler.h"
#include "../../Commands/WriteCommand.h"
#include "../../Responses/Response.h"


class WriteCommandHandler : public CommandHandler
{
public:
    static byte GetCommandByte();
    Response* Handle();    
    
private:
    void WriteData(WriteCommand writeCommand);
    void SetData(uint8_t data);
};

#endif