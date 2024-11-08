#include "WriteCommandHandler.h"
#include "Response.h"
#include "WriteCommand.h"
#include "HardwareConfiguration.h"

/// @brief Handles write command. The command contains an address high and low byte, and an data byte (3 bytes total).
/// @return S_OK if write was successful, S_FALSE otherwise.
Response* WriteCommandHandler::Handle()
{
    // For a write command expect 3 bytes:
    // - first and secord are the address bytes
    // - third is the data byte
    while(Serial1.available() < 3)
    {
    }

    WriteCommand writeCommand;
    Serial1.readBytes((uint8_t*)&writeCommand, sizeof(WriteCommand));    
    WriteData(writeCommand);
    log_d("Byte has been written: %02x%02x %02x\r\n", writeCommand.address_highbyte, writeCommand.address_lowbyte, writeCommand.data);

    Response* pResponse = new Response();
    pResponse->result = W_OK;
    
    return pResponse;
}

void WriteCommandHandler::WriteData(WriteCommand writeCommand)
{
    digitalWrite(WRITE_ENABLE_PIN, HIGH);
    digitalWrite(OUTPUT_ENABLE_PIN, HIGH);
    SetAddress(writeCommand.address_highbyte, writeCommand.address_lowbyte);
    digitalWrite(WRITE_ENABLE_PIN, LOW);
    SetData(writeCommand.data);
    digitalWrite(WRITE_ENABLE_PIN, HIGH);
    delay(6);
}

void WriteCommandHandler::SetData(uint8_t data)
{        
    for(int i=0;i<8;i++)
    {
        pinMode(dataPins[i], OUTPUT);
    }
    for(int i=0;i<8;i++)
    {        
        digitalWrite(dataPins[i], data & 1);
        data = data >> 1;
    }
}
