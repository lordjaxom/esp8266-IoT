#include <algorithm>

#include <NeoPixelBus.h>

#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/Motion.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/Timer.hpp"

using namespace std;

void update();

uint8_t constexpr levelDelta = 255 * IoTClass::tick / 1000;

uint8_t constexpr offLevel = 0x00;
uint8_t constexpr dimLevel = 0x28;
uint8_t constexpr onLevel = 0xff;

uint16_t constexpr countPixels = 36;

uint8_t currentLevels[countPixels];
uint8_t targetLevels[countPixels];

NeoPixelBus< NeoGrbFeature, NeoEsp8266Uart1800KbpsMethod > neoPixelBus( countPixels );

IoTClass IoT( "Sleep/Light/Floor", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

Input input0( motion( 5000, gpioInput( 12, false )));
Input input1( motion( 5000, gpioInput( 14, false )));
Input input2( motion( 5000, gpioInput( 16, false )));

Device unterBett;

SceneManager sceneManager( "Sleep" );

void update()
{
    if ( unterBett.get()) {
        fill_n( targetLevels, countPixels, onLevel );
    } else {
        fill_n( targetLevels, countPixels, offLevel );
        if ( input0.get()) {
            fill_n( targetLevels + 0, 12, dimLevel );
        }
        if ( input1.get()) {
            fill_n( targetLevels + 11, 14, dimLevel );
        }
        if ( input2.get()) {
            fill_n( targetLevels + 24, 12, dimLevel );
        }
    }

    for ( uint16_t i = 0 ; i < countPixels ; ++i ) {
        int16_t diff = targetLevels[i] - currentLevels[i];
        if ( diff == 0 ) {
            // nothing to do
        } else if ( abs( diff ) < levelDelta ) {
            currentLevels[i] = targetLevels[i];
        } else {
            currentLevels[i] += levelDelta * ( diff > 0 ? 1 : -1 );
        }
        neoPixelBus.SetPixelColor( i, RgbColor( currentLevels[i] ));
    }
    neoPixelBus.Show();
}

void setup()
{
    neoPixelBus.Begin();

    IoT.loopEvent += [] { update(); };

    sceneManager.addSceneDevice( unterBett, { Scene::SCENE2 } );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}