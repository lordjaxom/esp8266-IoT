#include "IoT/Command.hpp"
#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "Sleep/Light/Ceiling", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( 12 )));
Device output1( "POWER1", gpioOutput( 5 ));
Device output2( "POWER2", gpioOutput( 4 ));
Command hold( "HOLD" );

SceneManager sceneManager( "Sleep" );

void setup()
{
    sceneManager.addSceneDevice( output1, { Scene::SCENE1 } );
    sceneManager.addSceneDevice( output2, { Scene::SCENE3 } );
    
    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };
    button.longClickedEvent += [] { hold.trigger(); };
    
//    button.longClickedEvent += [] { IoT.publish( "cmnd/Sleep/Light/Ceiling/POWER", "HOLD" ); };
//    button.clickedEvent += []( unsigned clicked ) {
//        switch (clicked) {
//            case 1:
//                output1.toggle();
//                break;
//            case 2:
//                IoT.publish( "cmnd/Sleep/Light/Ceiling/POWER", "DOUBLE" );
//                break;
//            default:
//                break;
//        }
//    };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}