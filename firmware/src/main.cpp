#include <Arduino.h>

#include "HardwareConfiguration.h"
#include "CommandHandlerFactory.h"
#include "CommandHandler.h"
#include "Command.h"
#include "ReadResponse.h"

/**************************************/
/*         Global variables           */
/**************************************/
CommandHandlerFactory commandHandlerFactory;


/************************************/
/*             Setup                */
/************************************/
void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up...\r\n");

  pinMode(SHIFT_SERIAL_DATA_INPUT_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PULSE_PIN, OUTPUT);
  pinMode(SHIFT_STORAGE_CLOCK_PULSE_PIN, OUTPUT);
  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  pinMode(WRITE_ENABLE_PIN, OUTPUT);

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
  Serial1.begin(57600, SERIAL_8N1, SERIAL_FULL, TX_PIN);
#endif
#ifdef ARDUINO_NodeMCU_32S
  Serial1.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN, false);
#endif  
}

void loop() {    
  // Wait for a command byte, telling if write or read operation is requested  
  int numberOfBytesAvailable = Serial1.available();
  if (numberOfBytesAvailable > 0)    // command byte is available
  {    
    int commandByte = Serial1.read();
    CommandHandler* pHandler = commandHandlerFactory.GetCommandHandler(commandByte);
    Response* pResponse = pHandler->Handle();
        
    Serial1.write(pResponse->result);
    if(commandByte == 'R')
    {      
      ReadResponse* pReadResponse = static_cast<ReadResponse*>(pResponse);
      Serial1.write(pReadResponse->data);
    }
    delete pResponse;
  }
}

