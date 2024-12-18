#ifndef BULKWRITECOMMANDHANDLER_H_
#define BULKWRITECOMMANDHANDLER_H_

#include "../CommandHandler.h"
#include "../../Commands/BulkWriteCommand.h"
#include "../../Responses/Response.h"


class BulkWriteCommandHandler : public CommandHandler
{
public:
    static byte GetCommandByte();
    Response* Handle();    
        
private:
    void WriteData(uint8_t *pFirmwareBytes, uint16_t length);
    void SetData(uint8_t data);
    bool WaitForWriteCycleEnd(uint8_t lastDataByteInPage);

private:
    uint16_t m_MaxWriteTimeInMilliSeconds = 10;
};

#endif