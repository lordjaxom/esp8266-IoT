#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

PushButton button( debounce( gpioInput( 0 )));
Device output( "Kellerflur/Treppe", gpioOutput( 2, true ));

SceneManager sceneManager( "Kellerflur" );

void setup()
{
    digitalWrite( 2, HIGH );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.sceneButtonClicked( clicked ); };

    sceneManager.addSceneDevice( output, { Scene::SCENE1, Scene::SCENE2 } );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}