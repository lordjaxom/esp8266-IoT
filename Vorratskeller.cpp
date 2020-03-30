#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "Vorratskeller", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button1( debounce( gpioInput( 14 )));
Device output1( "Vorratskeller/Deckenlampe", "POWER", gpioOutput( 4 )); // command -> name
SceneManager sceneManager1( "Vorratskeller" );

PushButton button2( debounce( gpioInput( 12 )));
Device output2( "Naehstube/Deckenlampe", "POWER", gpioOutput( 5 ));
SceneManager sceneManager2( "Naehstube" );

Input motionInput( gpioInput( 13, false ));
Device motionDevice( "Vorratskeller", "MOTION", "NO", "YES" );

void setup()
{
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