#include "IoT/Debounce.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/Remote.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/String.hpp"

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( 0 )));
Remote output( "Kueche/Vitrinen" );

SceneManager sceneManager( "Kueche" );

void setup()
{
    button.clickedEvent += []( unsigned clicked ) { sceneManager.deviceButtonClicked( output, clicked ); };

    sceneManager.addLocalDevice( output );
    
    IoT.begin();
}

void loop()
{
    IoT.loop();
}