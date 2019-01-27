#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "ColinWZ" );

PushButton button( debounce( gpioInput( 12 )));
Device output( "ColinWZ/DeckenLED", gpioOutput( 5 ));

SceneManager sceneManager( "ColinWZ" );

void setup()
{
    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}