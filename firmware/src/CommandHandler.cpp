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

