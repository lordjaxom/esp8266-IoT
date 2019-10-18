#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/Motion.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/Timer.hpp"

using namespace std;

void update();

RgbColor const offColor( 0x00 );
RgbColor const dimColor( 0x3F );
RgbColor const onColor( 0xFF );

NeoPixelBus< NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod > neoPixelBus( 36 );

IoTClass IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

Input input0( motion( 5000, [] { return digitalRead( 12 ) == HIGH; } ));
Input input1( motion( 5000, [] { return digitalRead( 14 ) == HIGH; } ));
Input input2( motion( 5000, [] { return digitalRead( 16 ) == HIGH; } ));

Device unterBett( "Schlafzimmer/UnterBett" /*, []( bool ) { update(); }*/ );

SceneManager sceneManager( "Schlafzimmer" );

void update()
{
    if ( unterBett.get()) {
        neoPixelBus.ClearTo( onColor );
    } else {
        neoPixelBus.ClearTo( offColor );
        if ( input0.get()) {
            neoPixelBus.ClearTo( dimColor, 0, 11 );
        }
        if ( input1.get()) {
            neoPixelBus.ClearTo( dimColor, 11, 24 );
        }
        if ( input2.get()) {
            neoPixelBus.ClearTo( dimColor, 24, 35 );
        }
    }
    neoPixelBus.Show();
}

void setup()
{
    pinMode( 12, INPUT );
    pinMode( 14, INPUT );
    pinMode( 16, INPUT );

    neoPixelBus.Begin();

    sceneManager.addSceneDevice( unterBett, { Scene::SCENE2 } );

//    input0.changeEvent += []( bool ) { update(); };
//    input1.changeEvent += []( bool ) { update(); };
//    input2.changeEvent += []( bool ) { update(); };

    IoT.loopEvent += [] { update(); };

    IoT.begin();

    //update();
}

void loop()
{
    IoT.loop();
}