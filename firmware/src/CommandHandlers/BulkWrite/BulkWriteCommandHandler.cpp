#include "BulkWriteCommandHandler.h"

byte BulkWriteCommandHandler::GetCommandByte()
{
    return 'B';
}

Response *BulkWriteCommandHandler::Handle()
{
    // Expect a bulk write command, containing the data length
    while (Serial1.available() < 2) ;

    // Read data, arriving in little endian format
    BulkWriteCommand bulkWriteCommand;
    Serial1.readBytes((uint8_t *)&bulkWriteCommand, 2);
    log_d("Data length: %d\r\n", bulkWriteCommand.length);

    uint8_t *pFirmwareBytes = new uint8_t[bulkWriteCommand.length];
    int currentPosition = 0;

    int availableBytes = Serial1.available();
    while (currentPosition < bulkWriteCommand.length)
    {
        while (Serial1.available() == 0)
            ;
        availableBytes = Serial1.available();
        size_t readBytes = Serial1.readBytes(&pFirmwareBytes[currentPosition], availableBytes);
        currentPosition += readBytes;
    }
    log_d("Firmware bits received, first 4 bytes: %02x %02x %02x %02x, last 4 bytes: %02x %02x %02x %02x\r\n",
          pFirmwareBytes[0], pFirmwareBytes[1], pFirmwareBytes[2], pFirmwareBytes[3],
          pFirmwareBytes[bulkWriteCommand.length - 4], pFirmwareBytes[bulkWriteCommand.length - 3], pFirmwareBytes[bulkWriteCommand.length - 2], pFirmwareBytes[bulkWriteCommand.length - 1]);

    WriteData(pFirmwareBytes, bulkWriteCommand.length);
    delete[] pFirmwareBytes;

    log_d("Firmware data bits have been written.\r\n");

    Response *pResponse = new Response();
    pResponse->result = Response::WRITE_SUCCEEDED;
    pResponse->responseDataSize = sizeof(bulkWriteCommand.length);
    // Number of bytes written
    pResponse->responseData = new byte[pResponse->responseDataSize];
    memcpy(pResponse->responseData, &bulkWriteCommand.length, pResponse->responseDataSize);
    
    return pResponse;
}

void BulkWriteCommandHandler::WriteData(uint8_t *pFirmwareBytes, uint16_t length)
{
    const uint8_t BLOCK_SIZE = 64;

    for (uint16_t address = 0; address < length; address += BLOCK_SIZE)
    {
        uint8_t data;
        uint8_t address_highByte;
        uint8_t address_lowByte;

        do
        {
            disableOutput();
            disableWrite();
            enableChip();
            SetDataPinsModeTo(OUTPUT);

            for (uint8_t blockPointer = 0; blockPointer < BLOCK_SIZE; blockPointer++)
            {
                address_highByte = (address + blockPointer) >> 8;
                address_lowByte = (address + blockPointer) % 256;
                data = pFirmwareBytes[address + blockPointer];
                
                log_d("set - address: %02x%02x, data: %02x\r\n", address_highByte, address_lowByte, data);
                SetAddress(address_highByte, address_lowByte);
                SetData(data);

                delayMicroseconds(1);
                enableWrite();
                delayMicroseconds(1);
                disableWrite();
            }
        } while (WaitForWriteCycleEnd(data) == false);      // try to write the page again if the wait cycle resulted in fail
        
        disableChip();

        // Send the address in little endian format
        Serial1.write(Response::WRITE_INPROGRESS);
        Serial1.write(address_lowByte);
        Serial1.write(address_highByte);
    }
}

void BulkWriteCommandHandler::SetData(uint8_t data)
{   
    SetDataPinsModeTo(OUTPUT);
    for(int i=0;i<8;i++)
    {        
        digitalWrite(dataPins[i], data & 1);
        data = data >> 1;
    }
}

/// @brief Wait for write cycle end with DATA polling: 
/// 1. Monitor I/O7 bit; write in progress if its value complements the written value (e.g. 0 -> 1 or 1 -> 0)
/// 2. Monitor I/O6 bit; it alternates its value in every reading while write is in progress
/// @param lastDataByteInPage Last written byte on the page, for comparison with read I/O7 bit
/// @return true if write cycle finished successfully
bool BulkWriteCommandHandler::WaitForWriteCycleEnd(uint8_t lastDataByteInPage)
{    
    SetDataPinsModeTo(INPUT);
    delayMicroseconds(1);

    uint8_t firstRead;
    uint8_t secondRead;
    
    for (int readCount = m_MaxWriteTimeInMilliSeconds * 1000 / 2; (readCount > 0); readCount--)
    {
        enableOutput();
        delayMicroseconds(1);
        firstRead = ReadDataByte();
        disableOutput();

        enableOutput();
        delayMicroseconds(1);
        secondRead = ReadDataByte();
        disableOutput();

        log_d("WaitForEndOfWriteCycle - last data byte: %02x, firstRead: %02x, secondRead: %02x\r\n", lastDataByteInPage, firstRead, secondRead);
        if ((firstRead == secondRead) && (firstRead == lastDataByteInPage))
        {
            return true;
        }
    }

    return false;
}