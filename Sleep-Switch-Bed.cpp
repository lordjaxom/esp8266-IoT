#include "IoT/Command.hpp"
#include "IoT/Debounce.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "Sleep/Switch/Bed", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( 3 )));
Command hold( "HOLD" );

SceneManager sceneManager( "Sleep" );

void setup()
{
    digitalWrite( 2, HIGH );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };
    button.longClickedEvent += [] { hold.trigger(); };

//    button.longClickedEvent += [] { IoT.publish( "cmnd/Sleep/Switch/Bed/POWER", "HOLD" ); };
//    button.clickedEvent += []( unsigned clicked ) { IoT.publish( "cmnd/Sleep/Switch/Bed/POWER", String( clicked )); };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}