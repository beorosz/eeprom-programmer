#ifndef COMMANDHANDLERFACTORY_H_
#define COMMANDHANDLERFACTORY_H_

#include <Arduino.h>

#include "CommandHandler.h"
#include "ReadCommandHandler.h"
#include "WriteCommandHandler.h"

class CommandHandlerFactory
{
private:
    ReadCommandHandler readCommandHandler;
    WriteCommandHandler writeCommandHandler;
public:    
    /// @brief The command handler
    /// @param commandBytes The command byte ('R' for read, 'W' for write)
    /// @return The command handler handling the next arriving command
    CommandHandler* GetCommandHandler(int commandByte);
};

#endif