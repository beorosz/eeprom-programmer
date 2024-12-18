#include "WriteCommandHandler.h"

byte WriteCommandHandler::GetCommandByte()
{
    return 'W';
}

/// @brief Handles write command. The command contains an address high and low byte, and an data byte (3 bytes total).
/// @return S_OK if write was successful, S_FALSE otherwise.
Response* WriteCommandHandler::Handle()
{
    // For a write command expect 3 bytes:
    // - first and secord are the address bytes
    // - third is the data byte
    while(Serial1.available() < sizeof(WriteCommand)) ;

    WriteCommand writeCommand;
    Serial1.readBytes((uint8_t*)&writeCommand, sizeof(WriteCommand));
    WriteData(writeCommand);
    log_d("Byte has been written: %02x%02x %02x\r\n", writeCommand.address_highbyte, writeCommand.address_lowbyte, writeCommand.data);

    Response* pResponse = new Response();
    pResponse->result = Response::WRITE_SUCCEEDED;
    
    return pResponse;
}

void WriteCommandHandler::WriteData(WriteCommand writeCommand)
{
    
    disableWrite();
    disableOutput();
    SetAddress(writeCommand.address_highbyte, writeCommand.address_lowbyte);
    enableWrite();
    SetData(writeCommand.data);
    disableWrite();

    delay(1);
}

void WriteCommandHandler::SetData(uint8_t data)
{   
    SetDataPinsModeTo(OUTPUT);
    for(int i=0;i<8;i++)
    {        
        digitalWrite(dataPins[i], data & 1);
        data = data >> 1;
    }
}
