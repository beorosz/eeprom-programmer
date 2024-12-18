#ifndef BULKREADCOMMANDHANDLER_H_
#define BULKREADCOMMANDHANDLER_H_

#include "../CommandHandler.h"
#include "../../Commands/BulkReadCommand.h"
#include "../../Responses/Response.h"


class BulkReadCommandHandler : public CommandHandler
{
public:
    static byte GetCommandByte();
    Response* Handle();
    
private:
    /// @brief Reads bytes from EEPROM and stores the data in a buffer.
    /// @param command The command containing the address and the number of bytes to read.
    /// @return The buffer containing the read bytes.
    byte* ReadData(BulkReadCommand command);
};

#endif