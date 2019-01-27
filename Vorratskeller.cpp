#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Vorratskeller" );

PushButton button1( debounce( [] { return digitalRead( 14 ) == LOW; } ));
Device output1( "Vorratskeller/Deckenlampe",
                     []( bool value ) { digitalWrite( 4, static_cast< uint8_t >( value ? HIGH : LOW )); } );
SceneManager sceneManager1( "Vorratskeller" );

PushButton button2( debounce( [] { return digitalRead( 12 ) == LOW; } ));
Device output2( "Naehstube/Deckenlampe", []( bool value ) { digitalWrite( 5, static_cast< uint8_t >( value ? HIGH : LOW )); } );
SceneManager sceneManager2( "Naehstube" );

Input motionInput( [] { return digitalRead( 13 ) == HIGH; } );
Device motionDevice( "Vorratskeller", "MOTION", "NO", "YES" );

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