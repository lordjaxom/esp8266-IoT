#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Pcf8574.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/Wire.hpp"

#include "ledgraph/fill_animation.hpp"
#include "ledgraph/ledbuffer.hpp"
#include "ledgraph/warp_animation.hpp"

NeoPixelBus< NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod > neoPixelBus( 134 );
NeoPixelBuffer< NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod > neoPixelBuffer( neoPixelBus );
unsigned neoPixelState;
auto onAnimation = fillAnimation( neoPixelBuffer, 0xffffff );
auto buntAnimation = warpAnimation< 10, 40, 20, 25, 100, 25 >( neoPixelBuffer );

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "SchmalerFlur/Garderobe" );

iot::Device ledstreifenWeiss( IoT, "SchmalerFlur/GarderobeWeiss" );
iot::Device ledstreifenBunt( IoT, "SchmalerFlur/GarderobeBunt" );
iot::SceneManager sceneManager( IoT, "SchmalerFlur" );

bool lastMotion;

void setup()
{
    neoPixelBus.Begin();

    sceneManager.addSceneEvent( iot::Scene::SLEEP, [] { neoPixelState = 0; } );
    sceneManager.addSceneEvent( iot::Scene::OFF, [] { neoPixelState = 1; } );
    sceneManager.addSceneEvent( iot::Scene::SCENE1, [] { neoPixelState = 2; } );
    sceneManager.addSceneEvent( iot::Scene::SCENE2, [] { neoPixelState = 2; } );

    sceneManager.addSceneDevice( ledstreifenWeiss, { iot::Scene::SCENE1, iot::Scene::SCENE2 }, { iot::Scene::SLEEP, iot::Scene::OFF } );
    sceneManager.addSceneDevice( ledstreifenBunt, { iot::Scene::OFF }, { iot::Scene::SLEEP, iot::Scene::SCENE1, iot::Scene::SCENE2 } );

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
