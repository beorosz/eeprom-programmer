#include "ReadCommandHandler.h"
#include "ReadCommand.h"
#include "Response.h"

Response* ReadCommandHandler::Handle()
{
    // For a read command expect 2 bytes:
    // - first and secord are the address bytes    
    while(Serial1.available() < 2)
    {
    }

    ReadCommand readCommand;
    Serial1.readBytes((uint8_t*)&readCommand, sizeof(ReadCommand));
    uint8_t data = ReadData(readCommand);
    log_i("Byte has been read: %02x%02x %02x\r\n", readCommand.address_highbyte, readCommand.address_lowbyte, data);

    ReadResponse* pResponse = new ReadResponse();
    pResponse->result = R_OK;
    pResponse->data = data;
    
    return pResponse;
}

uint8_t ReadCommandHandler::ReadData(ReadCommand readCommand)
{
    for(int i=0;i<8;i++)
    {
        pinMode(dataPins[i], INPUT);
    }
    
    SetAddress(readCommand.address_highbyte, readCommand.address_lowbyte);
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
    digitalWrite(WRITE_ENABLE_PIN, HIGH);
    delayMicroseconds(1);
        
    uint8_t data = 0;
    for(int i=7;i>=0;i--)
    {
        data = (data << 1) + digitalRead(dataPins[i]);
    }

    return data;
}

