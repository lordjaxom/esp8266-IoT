#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "Colin/Light/Living", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button1( debounce( gpioInput( 12 )));
Device output1( "POWER", gpioOutput( 5 ));

PushButton button2( debounce( gpioInput( 14 )));
Remote output2( "Colin/Zockbereich" );

PushButton button3( debounce( gpioInput( 13 )));
Remote output3( "FlurLicht", "POWER1" );

SceneManager sceneManager( "Colin" );

void setup()
{
    button1.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };
    button2.clickedEvent += []( unsigned clicked ) { sceneManager.deviceButtonClicked( output2, clicked ); };
    button3.clickedEvent += []( unsigned clicked ) { sceneManager.deviceButtonClicked( output3, clicked ); };

    sceneManager.addSceneDevice( output1 );
    sceneManager.addLocalDevice( output2 );
    sceneManager.addLocalDevice( output3 );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}