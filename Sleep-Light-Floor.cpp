#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/Motion.hpp"
#include "IoT/Timer.hpp"

using namespace std;

void update();

RgbColor const offColor( 0x00 );
RgbColor const dimColor( 0x3F );
RgbColor const onColor( 0xFF );

NeoPixelBus< NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod > neoPixelBus( 36 );

IoTClass IoT( "Sleep/Light/Floor", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

Input input0( motion( 5000, gpioInput( 12, false )));
Input input1( motion( 5000, gpioInput( 14, false )));
Input input2( motion( 5000, gpioInput( 16, false )));

Device unterBett( "Sleep/Light/Floor" /*, []( bool ) { update(); }*/ );

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
    neoPixelBus.Begin();

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