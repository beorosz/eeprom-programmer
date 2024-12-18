#include "CommandHandler.h"
#include "HardwareConfiguration.h"

void CommandHandler::SetAddress(uint8_t address_highbyte, uint8_t address_lowbyte)
{    
    shiftOut(SHIFT_SERIAL_DATA_INPUT_PIN, SHIFT_CLOCK_PULSE_PIN, LSBFIRST, address_lowbyte);   
    shiftOut(SHIFT_SERIAL_DATA_INPUT_PIN, SHIFT_CLOCK_PULSE_PIN, LSBFIRST, address_highbyte);

    digitalWrite(SHIFT_STORAGE_CLOCK_PULSE_PIN, LOW);
    digitalWrite(SHIFT_STORAGE_CLOCK_PULSE_PIN, HIGH);
    digitalWrite(SHIFT_STORAGE_CLOCK_PULSE_PIN, LOW);
}

void CommandHandler::SetDataPinsModeTo(uint8_t mode)
{
    for(int i=0;i<8;i++)
    {
        pinMode(dataPins[i], mode);
    }
}

byte CommandHandler::ReadDataByte()
{
    uint8_t data = 0;
    for (int i = 7; i >= 0; i--)
    {
        data = (data << 1) + digitalRead(dataPins[i]);
    }

    return data;
}


void CommandHandler::disableOutput()
{
    digitalWrite(OUTPUT_ENABLE_PIN, HIGH);
}

void CommandHandler::enableOutput()
{
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
}

void CommandHandler::disableWrite()
{
    digitalWrite(WRITE_ENABLE_PIN, HIGH);
}
void CommandHandler::enableWrite()
{
    digitalWrite(WRITE_ENABLE_PIN, LOW);
}

void CommandHandler::disableChip()
{
    digitalWrite(CHIP_ENABLE_PIN, HIGH);
}

void CommandHandler::enableChip()
{
    digitalWrite(CHIP_ENABLE_PIN, LOW);
}

