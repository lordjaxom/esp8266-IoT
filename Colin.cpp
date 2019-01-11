#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "ColinWZ" );

iot::PushButton button( IoT, iot::debounce( [] { return digitalRead( 12 ) == LOW; } ));
iot::Device output( IoT, "ColinWZ/DeckenLED", []( bool value ) { digitalWrite( 5, static_cast< uint8_t >( value ? HIGH : LOW )); } );

iot::SceneManager sceneManager( IoT, "ColinWZ" );

void setup()
{
    pinMode( 12, INPUT_PULLUP );
    pinMode( 5, OUTPUT );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}