#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/MQTT.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/WiFi.hpp"

iot::IoT IoT;
iot::WiFi wiFi( IoT, "akvsoft", "sacomoco02047781" );
iot::MQTT mqtt( IoT, wiFi, "192.168.178.28", 1883, "Untitled" );

iot::PushButton button( IoT, iot::debounce( [] { return digitalRead( 0 ) == LOW; } ));
iot::Device output( IoT, "Untitled/Output", []( bool value ) { digitalWrite( 2, static_cast< uint8_t >( value ? LOW : HIGH )); } );

iot::SceneManager sceneManager( IoT, "Untitled" );

void setup()
{
    pinMode( 0, INPUT_PULLUP );
    pinMode( 2, OUTPUT );
    digitalWrite( 2, HIGH );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}