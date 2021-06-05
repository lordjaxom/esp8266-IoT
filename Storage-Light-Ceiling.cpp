#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "Storage/Light/Ceiling", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button1( debounce( gpioInput( 14 )));
Device output1( "POWER1", gpioOutput( 4 ));

PushButton button2( debounce( gpioInput( 12 )));
Device output2( "POWER2", gpioOutput( 5 ));

Input motionInput( gpioInput( 13, false ));
Device motionDevice( "MOTION", "NO", "YES" );

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

    motionInput.changeEvent += []( bool value ) {
        motionDevice.set( value );
    };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}