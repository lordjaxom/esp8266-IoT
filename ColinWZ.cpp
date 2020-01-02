#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button1( debounce( gpioInput( 12 )));
Device output1( "ColinWZ/DeckenLED", gpioOutput( 5 ));

PushButton button2( debounce( gpioInput( 14 )));
Remote output2( "Colin/Zockbereich" );

SceneManager sceneManager( "ColinWZ" );

void setup()
{
    button1.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };
    button2.clickedEvent += []( unsigned clicked ) { sceneManager.deviceButtonClicked( output2, clicked ); };

    sceneManager.addSceneDevice( output1 );
    sceneManager.addLocalDevice( output2 );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}