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
iot::MQTT mqtt( IoT, wiFi, "192.168.178.28", 1883, "Vorratskeller" );

iot::PushButton button1( IoT, iot::debounce( [] { return digitalRead( 14 ) == LOW; } ));
iot::PushButton button2( IoT, iot::debounce( [] { return digitalRead( 12 ) == LOW; } ));
iot::Device output1( IoT, "Vorratskeller/Deckenlampe", []( bool value ) {
    digitalWrite( 4, static_cast< uint8_t >( value ? HIGH : LOW ));
} );
iot::Device output2( IoT, "Naehstube/Deckenlampe", []( bool value ) {
    digitalWrite( 5, static_cast< uint8_t >( value ? HIGH : LOW ));
} );

iot::SceneManager sceneManager1( IoT, "Vorratskeller" );
iot::SceneManager sceneManager2( IoT, "Naehstube" );

void setup()
{
    pinMode( 14, INPUT_PULLUP );
    pinMode( 12, INPUT_PULLUP );
    pinMode( 4, OUTPUT );
    pinMode( 5, OUTPUT );
    pinMode( 13, INPUT );

    button1.clickedEvent += []( unsigned clicked ) { sceneManager1.sceneButtonClicked( clicked ); };
    button2.clickedEvent += []( unsigned clicked ) { sceneManager2.sceneButtonClicked( clicked ); };

    sceneManager1.addSceneDevice( output1 );
    sceneManager2.addSceneDevice( output2 );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}