#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include "../Responses/Response.h"
#include "HardwareConfiguration.h"


class CommandHandler
{
public:
    static byte GetCommandByte();
    virtual Response* Handle() = 0;

protected:
    /// @brief Sets the address bytes
    /// @param address_highbyte high byte of address
    /// @param address_lowbyte low byte of address
    void SetAddress(uint8_t address_highbyte, uint8_t address_lowbyte);
    void SetDataPinsModeTo(uint8_t mode);
    byte ReadDataByte();

    void disableOutput();
    void enableOutput();
    void disableWrite();
    void enableWrite();
    void disableChip();
    void enableChip();

protected:
    byte dataPins[8] = { DATA_0_PIN, DATA_1_PIN, DATA_2_PIN, DATA_3_PIN, DATA_4_PIN, DATA_5_PIN, DATA_6_PIN, DATA_7_PIN };
};

#endif