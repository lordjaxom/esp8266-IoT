#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( [] { return digitalRead( 0 ) == LOW; } ));
Device output( "Kellerflur/Treppe", []( bool value ) { digitalWrite( 2, static_cast< uint8_t >( value ? LOW : HIGH )); } );

SceneManager sceneManager( "Kellerflur" );

void setup()
{
    pinMode( 0, INPUT_PULLUP );
    pinMode( 2, OUTPUT );
    digitalWrite( 2, HIGH );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output, { Scene::SCENE1, Scene::SCENE2 } );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}