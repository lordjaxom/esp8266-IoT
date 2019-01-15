#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Kellerflur/Treppe" );

iot::PushButton button( IoT, iot::debounce( [] { return digitalRead( 0 ) == LOW; } ));
iot::Device output( IoT, "Kellerflur/Treppe", []( bool value ) { digitalWrite( 2, static_cast< uint8_t >( value ? LOW : HIGH )); } );

iot::SceneManager sceneManager( IoT, "Kellerflur" );

void setup()
{
    pinMode( 0, INPUT_PULLUP );
    pinMode( 2, OUTPUT );
    digitalWrite( 2, HIGH );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output, { iot::Scene::SCENE1, iot::Scene::SCENE2 } );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}