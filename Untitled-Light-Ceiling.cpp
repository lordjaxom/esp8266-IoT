#include "IoT/Command.hpp"
#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "Untitled/Light/Ceiling", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button1( debounce( gpioInput( 12 )));
Device output1( "POWER1", gpioOutput( 5 ));

PushButton button2( debounce( gpioInput( 14 )));
Device output2( "POWER2", gpioOutput( 4 ));

Command standby( "Untitled", "STANDBY", [] { log( "STANDBY was triggered" ); } );

SceneManager sceneManager( "Untitled" );

void setup()
{
//    sceneManager.addSceneDevice( output1, { Scene::SCENE1 } );
//    sceneManager.addSceneDevice( output2, { Scene::SCENE2 } );
//
//    button1.clickedEvent += []( unsigned clicked ) { sceneManager.deviceButtonClicked( output1, clicked ); };
//    button2.clickedEvent += []( unsigned clicked ) { sceneManager.deviceButtonClicked( output2, clicked ); };
//    button1.longClickedEvent += [] { standby.trigger(); };
//    button2.longClickedEvent += [] { standby.trigger(); };

    button1.clickedEvent += []( unsigned clicked ) {
        IoT.publish( str( "tele/", IoT.topic(), "/LOGGING" ), str( "clicked ", String( clicked )));
    };
    button1.longClickedEvent += [] {
        IoT.publish( str( "tele/", IoT.topic(), "/LOGGING" ), "longpressed" );
    };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}