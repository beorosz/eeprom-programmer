#ifndef COMMANDHANDLERFACTORY_H_
#define COMMANDHANDLERFACTORY_H_

#include <Arduino.h>

#include "CommandHandler.h"
#include "Read/ReadCommandHandler.h"
#include "BulkRead/BulkReadCommandHandler.h"
#include "Write/WriteCommandHandler.h"
#include "BulkWrite/BulkWriteCommandHandler.h"


class CommandHandlerFactory
{
private:
    ReadCommandHandler readCommandHandler;
    BulkReadCommandHandler bulkReadCommandHandler;
    WriteCommandHandler writeCommandHandler;
    BulkWriteCommandHandler bulkWriteCommandHandler;
public:    
    /// @brief The command handler
    /// @param commandBytes The command byte ('R' for read, 'P' for page read, 'W' for write, 'B' for bulk write)
    /// @return The command handler handling the next arriving command
    CommandHandler* GetCommandHandler(byte commandByte);
};

#endif