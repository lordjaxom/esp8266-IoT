#include "IoT/Debounce.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/String.hpp"

IoTClass IoT( "Apartment/Switch/Doorbell", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( 3 )));

void setup()
{
    digitalWrite( 2, HIGH );

    button.clickedEvent += []( unsigned clicked ) { IoT.publish( "stat/" + IoT.topic() + "/BUTTON", String( clicked )); };
    button.longClickedEvent += [] { IoT.publish( "stat/" + IoT.topic() + "/BUTTON", "HOLD" ); };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}