#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/SceneManager.hpp"

#include "ledgraph/fill_animation.hpp"
#include "ledgraph/ledbuffer.hpp"
#include "ledgraph/warp_animation.hpp"

NeoPixelBus< NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod > neoPixelBus( 134 );
NeoPixelBuffer< NeoGrbFeature, NeoEsp8266AsyncUart0800KbpsMethod > neoPixelBuffer( neoPixelBus );
unsigned neoPixelState;
auto onAnimation = fillAnimation( neoPixelBuffer, 0xffffff );
auto buntAnimation = warpAnimation< 10, 40, 20, 25, 100, 25 >( neoPixelBuffer );

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

Device ledstreifenWeiss( "SchmalerFlur/GarderobeWeiss" );
Device ledstreifenBunt( "SchmalerFlur/GarderobeBunt" );
SceneManager sceneManager( "SchmalerFlur" );

bool lastMotion;

void setup()
{
    neoPixelBus.Begin();

    sceneManager.addSceneEvent( Scene::SLEEP, [] { neoPixelState = 0; } );
    sceneManager.addSceneEvent( Scene::OFF, [] { neoPixelState = 1; } );
    sceneManager.addSceneEvent( Scene::SCENE1, [] { neoPixelState = 2; } );
    sceneManager.addSceneEvent( Scene::SCENE2, [] { neoPixelState = 2; } );

    sceneManager.addSceneDevice( ledstreifenWeiss, { Scene::SCENE1, Scene::SCENE2 } );
    sceneManager.addSceneDevice( ledstreifenBunt, { Scene::OFF } );

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
