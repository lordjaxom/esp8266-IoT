#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"

IoTClass IoT( "Colin/Light/Living", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button1( debounce( gpioInput( 12 )));
Device output1( "POWER", gpioOutput( 5 ));

PushButton button2( debounce( gpioInput( 14 )));
Remote output2( "Colin/Light/Gaming" );

PushButton button3( debounce( gpioInput( 13 )));
Remote output3( "Apartment/Light/Hallways", "POWER1" );

void setup()
{
    button1.clickedEvent += []( unsigned clicked ) {
        if ( clicked == 1 ) {
            output1.toggle();
        }
    };

    button2.clickedEvent += []( unsigned clicked ) {
        if ( clicked == 1 ) {
            output2.toggle();
        }
    };

    button3.clickedEvent += []( unsigned clicked ) {
        if ( clicked == 1 ) {
            output3.toggle();
        }
        else if ( clicked == 2 ) {
            IoT.publish( "stat/" + IoT.topic() + "/BUTTON3", "2" );
        }
    };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}