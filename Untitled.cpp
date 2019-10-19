#include "IoT/Command.hpp"
#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"

static constexpr uint8_t buttonPin = 0; // 12
static constexpr uint8_t outputPin = 5;

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( buttonPin )));
Device output( "Untitled/Output", gpioOutput( outputPin ));
Command standby( "Untitled", "STANDBY", [] { log( "STANDBY was triggered" ); } );

SceneManager sceneManager( "Untitled" );

void setup()
{
    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };
    button.longClickedEvent += [] { standby.trigger(); };

    sceneManager.addSceneDevice( output );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}