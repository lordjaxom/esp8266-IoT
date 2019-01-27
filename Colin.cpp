#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "ColinWZ" );

iot::PushButton button( IoT, iot::debounce( iot::GpioInput( IoT, 12 )));
iot::Device output( IoT, "ColinWZ/DeckenLED", iot::GpioOutput( IoT, 5 ));

iot::SceneManager sceneManager( IoT, "ColinWZ" );

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