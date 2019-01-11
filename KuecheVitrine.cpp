#include "IoT/Debounce.hpp"
#include "IoT/IoT.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/String.hpp"

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "KuecheVitrine" );

iot::PushButton button( IoT, iot::debounce( [] { return digitalRead( 0 ) == LOW; } ));

iot::SceneManager sceneManager( IoT, "Kueche" );

void setup()
{
    pinMode( 0, INPUT_PULLUP );

    button.clickedEvent += []( unsigned clicked ) { sceneManager.remoteDeviceButtonClicked( "Vitrinen", clicked ); };

    sceneManager.addRemoteDevice( "Vitrinen" );
    
    IoT.begin();
}

void loop()
{
    IoT.loop();
}