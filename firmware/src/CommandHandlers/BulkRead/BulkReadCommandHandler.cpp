#include "BulkReadCommandHandler.h"

byte BulkReadCommandHandler::GetCommandByte()
{
    return 'P';
}

Response* BulkReadCommandHandler::Handle()
{
    // For a bulk read command expect 3 bytes:
    // - first and second are the address bytes, third is the number of bytes to read and return
    while(Serial1.available() < 3) ;
    
    BulkReadCommand bulkReadCommand;
    Serial1.readBytes((byte*)&bulkReadCommand, 3);

    Response* pResponse = new Response();
    pResponse->responseData = ReadData(bulkReadCommand);
    log_d("address: %04x, length:%02x \r\n", bulkReadCommand.address, bulkReadCommand.dataLength);
        
    pResponse->result = Response::READ_SUCCEDED;
    pResponse->responseDataSize = bulkReadCommand.dataLength;
    
    return pResponse;
}

byte* BulkReadCommandHandler::ReadData(BulkReadCommand command)
{
    byte* responseData = new byte[command.dataLength];

    enableChip();
    SetDataPinsModeTo(INPUT);

    for (size_t pointer = 0; pointer < command.dataLength; pointer++)
    {
        byte address_highByte = (command.address + pointer) >> 8;
        byte address_lowByte = (command.address + pointer) % 256;
        SetAddress(address_highByte, address_lowByte);
        enableOutput();
        disableWrite();
        delayMicroseconds(1);
        responseData[pointer] = ReadDataByte();
    }
    
    disableChip();

    return responseData;
}


