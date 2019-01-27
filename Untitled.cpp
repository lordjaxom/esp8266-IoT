#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"

static constexpr uint8_t buttonPin = 0; // 12
static constexpr uint8_t outputPin = 5;

IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Untitled" );

PushButton button( debounce( GpioInput( buttonPin )));
Device output( "Untitled/Output", GpioOutput( outputPin ));

SceneManager sceneManager( "Untitled" );

void setup()
{
    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}