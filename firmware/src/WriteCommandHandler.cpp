#include "WriteCommandHandler.h"
#include "Response.h"
#include "WriteCommand.h"
#include "DataLength.h"
#include "HardwareConfiguration.h"

/// @brief Handles write command. The command contains an address high and low byte, and an data byte (3 bytes total).
/// @return S_OK if write was successful, S_FALSE otherwise.
Response* WriteCommandHandler::Handle()
{
    // For a write command expect
    // - 2 bytes, representing data length    
    while(Serial1.available() < 2)
    {
    }    
    DataLength dataLengthBuffer;
    Serial1.readBytes((uint8_t*)&dataLengthBuffer, sizeof(DataLength));
    size_t dataLength = dataLengthBuffer.address_highbyte << 8 + dataLengthBuffer.address_lowbyte;
    // Then the rest of the firmware data
    char* dataBuffer = (char*) malloc(dataLength * sizeof(char));    
    size_t readDataLength = Serial1.readBytes(dataBuffer, dataLength);
    for (size_t i = 0; i < dataLength; i++)
    {
        WriteCommand writeCmd;
        writeCmd.address_highbyte = i >> 8;
        writeCmd.address_lowbyte = i;
        writeCmd.data = dataBuffer[i];
        WriteData(writeCmd);
    }
    
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
