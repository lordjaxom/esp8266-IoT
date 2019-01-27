#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Pcf8574.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/WireConfig.hpp"

#include "ledgraph/fill_animation.hpp"
#include "ledgraph/ledbuffer.hpp"
#include "ledgraph/warp_animation.hpp"

NeoPixelBus< NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod > neoPixelBus( 37 );
NeoPixelBuffer< NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod > neoPixelBuffer( neoPixelBus );
unsigned neoPixelState;
auto onAnimation = fillAnimation( neoPixelBuffer, 0xffffff );
auto buntAnimation = warpAnimation( neoPixelBuffer );

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Kellerflur/WerkstattStripe" );

Device ledstreifenWeiss( "Kellerflur/WerkstattStripeWeiss" );
Device ledstreifenBunt( "Kellerflur/WerkstattStripeBunt" );
SceneManager sceneManager( "Kellerflur" );

Input motionInput( [] { return digitalRead( 0 ) == HIGH; } );
Device motionDevice( "Kellerflur", "MOTION", "NO", "YES" );

void setup()
{
    pinMode( 0, INPUT );

    neoPixelBus.Begin();

    sceneManager.addSceneEvent( Scene::SLEEP, [] { neoPixelState = 0; } );
    sceneManager.addSceneEvent( Scene::OFF, [] { neoPixelState = 1; } );
    sceneManager.addSceneEvent( Scene::SCENE1, [] { neoPixelState = 2; } );
    sceneManager.addSceneEvent( Scene::SCENE2, [] { neoPixelState = 2; } );

    sceneManager.addSceneDevice( ledstreifenWeiss, { Scene::SCENE1, Scene::SCENE2 } );
    sceneManager.addSceneDevice( ledstreifenBunt, { Scene::OFF } );

    motionInput.changeEvent += []( bool value ) { motionDevice.set( value ); };

    IoT.loopTickEvent += [] {
        switch ( neoPixelState ) {
            case 0:
                neoPixelBus.ClearTo( 0 );
                break;
            case 1:
                buntAnimation.animate();
                break;
            case 2:
                onAnimation.animate();
                break;
        }
        neoPixelBus.Show();
    };
    IoT.begin();
}

void loop()
{
    IoT.loop();
}
