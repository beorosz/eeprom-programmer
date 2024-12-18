#include <Arduino.h>

#include "HardwareConfiguration.h"
#include "CommandHandlers/CommandHandlerFactory.h"
#include "CommandHandlers/CommandHandler.h"
#include "Responses/Response.h"

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
  log_i("Starting up...\r\n");

  pinMode(SHIFT_SERIAL_DATA_INPUT_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PULSE_PIN, OUTPUT);
  pinMode(SHIFT_STORAGE_CLOCK_PULSE_PIN, OUTPUT);
  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  pinMode(WRITE_ENABLE_PIN, OUTPUT);
  pinMode(CHIP_ENABLE_PIN, OUTPUT);

#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
  Serial1.begin(115200, SERIAL_8N1, SERIAL_FULL, TX_PIN);
#endif
#ifdef ARDUINO_NodeMCU_32S
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN, false);
#endif
}

void loop()
{
  // Wait for a command byte, telling the type of operation requested
  int numberOfBytesAvailable = Serial1.available();
  if (numberOfBytesAvailable > 0) // command byte is available
  {
    int commandByte = Serial1.read();
    log_d("Command received: %c\r\n", commandByte);

    CommandHandler *pHandler = commandHandlerFactory.GetCommandHandler(commandByte);
    Response *pResponse = pHandler->Handle();

    Serial1.write(pResponse->result);
    Serial1.write(pResponse->responseData, pResponse->responseDataSize);
    Serial1.flush();

    log_d("Response result sent: %02x\r\n", pResponse->result);

    delete pResponse;
  }
}
