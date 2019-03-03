#include "IoT.hpp"
#include "Logger.hpp"

LoggerClass Logger;

void LoggerSerial::begin()
{
    Serial.begin( 115200 );
    while ( !Serial ) {}
}

void LoggerSocket::begin()
{
    IoT.wiFiConnectedEvent += [this] { connectToLogstash(); };
    IoT.wiFiDisconnectedEvent += [this] { disconnectFromLogstash(); };
}

void LoggerSocket::connectToLogstash()
{
    client_.connect( "192.168.178.28", 8266 );
}

void LoggerSocket::disconnectFromLogstash()
{
    client_.close( true );
}
