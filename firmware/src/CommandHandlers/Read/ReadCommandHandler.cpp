#include "ReadCommandHandler.h"

byte ReadCommandHandler::GetCommandByte()
{
    return 'R';
}

Response* ReadCommandHandler::Handle()
{
    // For a read command expect 2 bytes:
    // - first and second are the address bytes
    while(Serial1.available() < 2) ;

    ReadCommand readCommand;
    Serial1.readBytes((uint8_t*)&readCommand, sizeof(ReadCommand));
    uint8_t data = ReadData(readCommand);
    log_d("Byte has been read: %02x%02x %02x\r\n", readCommand.address_highbyte, readCommand.address_lowbyte, data);

    Response* pResponse = new Response();
    pResponse->result = Response::READ_SUCCEDED;
    pResponse->responseDataSize = sizeof(data);
    pResponse->responseData = new byte[pResponse->responseDataSize];
    memcpy(pResponse->responseData, &data, pResponse->responseDataSize);
    
    return pResponse;
}

uint8_t ReadCommandHandler::ReadData(ReadCommand readCommand)
{
    SetDataPinsModeTo(INPUT);

    SetAddress(readCommand.address_highbyte, readCommand.address_lowbyte);
    enableOutput();
    disableWrite();
    delayMicroseconds(1);

    return ReadDataByte();
}

