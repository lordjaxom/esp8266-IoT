#include <bitset>

#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/SceneManager.hpp"

#include "ledgraph/fill_animation.hpp"
#include "ledgraph/ledbuffer.hpp"
#include "ledgraph/warp_animation.hpp"

using namespace std;

NeoPixelBus< NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod > neoPixelBus( 37 );
NeoPixelBuffer< NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod > neoPixelBuffer( neoPixelBus );

bitset< 2 > animation;
auto offAnimation = fillAnimation( neoPixelBuffer, 0x000000 );
auto onAnimation = fillAnimation( neoPixelBuffer, 0xffffff );
auto buntAnimation = warpAnimation( neoPixelBuffer );

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

Device ledstreifenWeiss( "Kellerflur/WerkstattStripeWeiss", []( bool value ) { animation.set( 1, value ); } );
Device ledstreifenBunt( "Kellerflur/WerkstattStripeBunt", []( bool value ) { animation.set( 0, value ); } );
SceneManager sceneManager( "Kellerflur" );

Input motionInput( [] { return digitalRead( 0 ) == HIGH; } );
Device motionDevice( "Kellerflur", "MOTION", "NO", "YES" );

void setup()
{
    pinMode( 0, INPUT );

    neoPixelBus.Begin();

    sceneManager.addSceneDevice( ledstreifenWeiss, { Scene::SCENE1, Scene::SCENE2 } );
    sceneManager.addSceneDevice( ledstreifenBunt, { Scene::OFF } );

    motionInput.changeEvent += []( bool value ) { motionDevice.set( value ); };

    IoT.loopTickEvent += [] {
        switch ( animation.to_ulong()) {
            case 0:
                offAnimation.animate();
                break;
            case 1:
                buntAnimation.animate();
                break;
            default:
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
