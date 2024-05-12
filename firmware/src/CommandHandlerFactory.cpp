#include <Arduino.h>

#include "CommandHandlerFactory.h"
#include "CommandHandler.h"

CommandHandler* CommandHandlerFactory::GetCommandHandler(int commandByte)
{
    if(commandByte == 'R')
        return &readCommandHandler;

    return &writeCommandHandler;
}