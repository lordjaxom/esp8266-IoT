#include "IoT/AvrI2C.hpp"
#include "IoT/Command.hpp"
#include "IoT/IoT.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/WireConfig.hpp"
#include "IoT/Device.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Debounce.hpp"

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );
WireConfig wireConfig( 2, 0 );
AvrI2C avr2ic( wireConfig, 2 );

PushButton button( debounce( [] { return avr2ic.read(); } ));
//Device schwarzlicht( "Gaestezimmer/Schwarzlicht", []( bool value ) { avr2ic.set( 0, value ); } ); command -> name
//Device deckenlampe( "Gaestezimmer/Deckenlampe", []( bool value ) { avr2ic.set( 1, value ); } );
Command standby( "Gaestezimmer", "STANDBY" );

SceneManager sceneManager( "Gaestezimmer" );

void setup()
{
    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };
    button.longClickedEvent += [] { standby.trigger(); };

    sceneManager.addSceneDevice( schwarzlicht, { Scene::SCENE3 } );
    sceneManager.addSceneDevice( deckenlampe, { Scene::SCENE1 } );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}