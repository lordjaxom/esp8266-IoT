#include <NeoPixelBus.h>

#include "IoT/IoT.hpp"

using namespace std;

static constexpr size_t ledCount = 500;

NeoPixelBus< NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod > neoPixelBus( ledCount );

IoTClass IoT( "System/Light/LedTest", "VillaKunterbunt2", "sacomoco02047781", "openhab", 1883 );

static size_t counter;
static uint16_t ledIndex;

static void update()
{
//    if ( counter == 0 ) {
//        neoPixelBus.ClearTo( RgbColor( 0xff ));
//        neoPixelBus.SetPixelColor( ledIndex, RgbColor( 0xff, 0xff, 0xff ));
//        neoPixelBus.Show();
//        ledIndex = (uint16_t) (( ledIndex + 1 ) % ledCount );
//        counter = 250 / IoTClass::tick;
//
//        IoT.publish( "tele/System/Light/LedTest/INDEX", String( ledIndex ));
//    } else {
//        --counter;
//    }
    neoPixelBus.ClearTo( RgbColor( 0xff ));
    neoPixelBus.Show();
}

void setup()
{
    neoPixelBus.Begin();

    IoT.loopEvent += [] { update(); };

    IoT.begin();
}

void loop()
{
    IoT.loop();
}