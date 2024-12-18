#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <Arduino.h>

class Response
{
public:
    /// @brief Response result byte
    uint8_t result;
    /// @brief Additional response data size
    uint8_t responseDataSize;
    /// @brief Response data, stored in byte array
    byte * responseData;

    virtual ~Response()
    {
        if(responseData != NULL)
        {
            delete[] responseData;    
            responseData = NULL;
        }
    }

public:    
    static const uint8_t WRITE_INPROGRESS = 0x1;
    static const uint8_t WRITE_SUCCEEDED = 0x2;
    static const uint8_t WRITE_ERROR = 0x3;
    
    static const uint8_t READ_SUCCEDED = 0x10;
    static const uint8_t READ_ERROR = 0x11;
};

#endif