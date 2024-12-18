#include <Arduino.h>

#include "CommandHandlerFactory.h"
#include "CommandHandler.h"

CommandHandler* CommandHandlerFactory::GetCommandHandler(byte commandByte)
{
    if(commandByte == ReadCommandHandler::GetCommandByte())
        return &readCommandHandler;

    if(commandByte == WriteCommandHandler::GetCommandByte())
        return &writeCommandHandler;

    if(commandByte == BulkWriteCommandHandler::GetCommandByte())
        return &bulkWriteCommandHandler;
    
    if(commandByte == BulkReadCommandHandler::GetCommandByte())
        return &bulkReadCommandHandler;
}