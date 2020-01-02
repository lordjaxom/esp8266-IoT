#include "IoT/Debounce.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/String.hpp"

IoTClass IoT( "Sleep/Switch/Bed", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( 0 )));

void setup()
{
    digitalWrite( 2, HIGH );

    button.longClickedEvent += [] { IoT.publish( "cmnd/Sleep/Switch/Bed/POWER", "HOLD" ); };
    button.clickedEvent += []( unsigned clicked ) {
        switch (clicked) {
            case 1:
                IoT.publish( "cmnd/Sleep/Switch/Bed/POWER", "TOGGLE" );
                break;
            case 2:
                IoT.publish( "cmnd/Sleep/Switch/Bed/POWER", "DOUBLE" );
                break;
            default:
                break;
        }
    };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}