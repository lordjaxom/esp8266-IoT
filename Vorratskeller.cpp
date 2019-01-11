#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Vorratskeller" );

iot::PushButton button1( IoT, iot::debounce( [] { return digitalRead( 14 ) == LOW; } ));
iot::Device output1( IoT, "Vorratskeller/Deckenlampe",
                     []( bool value ) { digitalWrite( 4, static_cast< uint8_t >( value ? HIGH : LOW )); } );
iot::SceneManager sceneManager1( IoT, "Vorratskeller" );

iot::PushButton button2( IoT, iot::debounce( [] { return digitalRead( 12 ) == LOW; } ));
iot::Device output2( IoT, "Naehstube/Deckenlampe", []( bool value ) { digitalWrite( 5, static_cast< uint8_t >( value ? HIGH : LOW )); } );
iot::SceneManager sceneManager2( IoT, "Naehstube" );

iot::Input motionInput( IoT, [] { return digitalRead( 13 ) == HIGH; } );
iot::Device motionDevice( IoT, "Vorratskeller", "MOTION", "NO", "YES" );

void setup()
{
    pinMode( 14, INPUT_PULLUP );
    pinMode( 12, INPUT_PULLUP );
    pinMode( 4, OUTPUT );
    pinMode( 5, OUTPUT );
    pinMode( 13, INPUT );

    button1.clickedEvent += []( unsigned clicked ) { sceneManager1.sceneButtonClicked( clicked ); };
    sceneManager1.addSceneDevice( output1 );

    button2.clickedEvent += []( unsigned clicked ) { sceneManager2.sceneButtonClicked( clicked ); };
    sceneManager2.addSceneDevice( output2 );

    motionInput.changeEvent += []( bool value ) { motionDevice.set( value ); };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}