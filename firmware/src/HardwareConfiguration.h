#ifndef HARDWARECONFIGURATION_H_
#define HARDWARECONFIGURATION_H_

// ESP32
#ifdef ARDUINO_NodeMCU_32S
#define SHIFT_SERIAL_DATA_INPUT_PIN       33      // GPIO33
#define SHIFT_CLOCK_PULSE_PIN             26      // GPIO26
#define SHIFT_STORAGE_CLOCK_PULSE_PIN     25      // GPIO25
#define TX_PIN                            16      // GPIO16
#define RX_PIN                            17      // GPIO17
#define WRITE_ENABLE_PIN                  9       // GPIO9
#define OUTPUT_ENABLE_PIN                 10      // GPIO10
#define DATA_0_PIN                        5       // GPIO5
#define DATA_1_PIN                        4      // GPIO17
#define DATA_2_PIN                        15      // GPIO16
#define DATA_3_PIN                        23      // GPIO23
#define DATA_4_PIN                        22      // GPIO22
#define DATA_5_PIN                        21      // GPIO21
#define DATA_6_PIN                        19      // GPIO19
#define DATA_7_PIN                        18      // GPIO18
#endif

//Wemos D1 mini
#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
#define SHIFT_SERIAL_DATA_INPUT_PIN       16      // GPIO16
#define SHIFT_CLOCK_PULSE_PIN             12      // GPIO12
#define SHIFT_STORAGE_CLOCK_PULSE_PIN     14      // GPIO14
#define TX_PIN                            15      // GPIO15
#define RX_PIN                            13      // GPIO13
#define WRITE_ENABLE_PIN                  0       // GPIO??
#define OUTPUT_ENABLE_PIN                 0       // GPIO??
#define DATA_0_PIN                        0       // GPIO??
#define DATA_1_PIN                        0       // GPIO??
#define DATA_2_PIN                        0       // GPIO??
#define DATA_3_PIN                        0       // GPIO??
#define DATA_4_PIN                        0       // GPIO??
#define DATA_5_PIN                        0       // GPIO??
#define DATA_6_PIN                        0       // GPIO??
#define DATA_7_PIN                        0       // GPIO??
#endif

#endif