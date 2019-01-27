#include "IoT/AvrI2C.hpp"
#include "IoT/IoT.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/WireConfig.hpp"
#include "IoT/Device.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Debounce.hpp"

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Gaestezimmer" );
iot::WireConfig wireConfig( IoT, 2, 0 );
iot::AvrI2C avr2ic( wireConfig, 2 );

iot::PushButton button( IoT, iot::debounce( [] { return avr2ic.read(); } ));
iot::Device schwarzlicht( IoT, "Gaestezimmer/Schwarzlicht", []( bool value ) { avr2ic.set( 0, value ); } );
iot::Device deckenlampe( IoT, "Gaestezimmer/Deckenlampe", []( bool value ) { avr2ic.set( 1, value ); } );

iot::SceneManager sceneManager( IoT, "Gaestezimmer" );

void setup()
{
    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( schwarzlicht, { iot::Scene::SCENE2 } );
    sceneManager.addSceneDevice( deckenlampe, { iot::Scene::SCENE1 } );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}