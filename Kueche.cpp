#include "IoT/IoT.hpp"
#include "IoT/MQTT.hpp"
#include "IoT/Pcf8574.hpp"
#include "IoT/WiFi.hpp"
#include "IoT/Wire.hpp"

iot::IoT IoT;
iot::WiFi wiFi( IoT, "akvsoft", "sacomoco02047781" );
iot::MQTT mqtt( wiFi, "192.168.178.28", 1883, "Vorratskeller" );

iot::Wire wire( IoT, 2, 0 );
iot::Pcf8574 pcf8574Output( wire, 56, 0xff );
iot::Pcf8574 pcf8574Input( wire, 57, 0x00 );

void setup()
{
    IoT.begin();
}

void loop()
{
    IoT.loop();
}