#ifndef ESP8266_IOT_LOGGERSERIAL_HPP
#define ESP8266_IOT_LOGGERSERIAL_HPP

#include <HardwareSerial.h>

#include "LoggerPrint.hpp"

class LoggerSerial
        : public LoggerPrint< HardwareSerial, Serial >
{
public:
    void begin()
    {
        Serial.begin( 115200 );
        while ( !Serial ) {}
    }
};

#endif //ESP8266_IOT_LOGGERSERIAL_HPP
