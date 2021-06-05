#include <NeoPixelBus.h>

#include "IoT/Command.hpp"
#include "IoT/Device.hpp"
#include "IoT/Gpio.hpp"
#include "IoT/Input.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Timer.hpp"

using namespace std;

static constexpr uint16_t ledsPerSegment = 3;
static constexpr uint16_t ledSegmentCount = 13;
static constexpr uint16_t totalLedCount = ledsPerSegment * ledSegmentCount;

static constexpr size_t framesPerSecond = 25;
static constexpr size_t timePerFrame = 1000 / framesPerSecond;

NeoPixelBus< NeoGrbFeature, NeoEsp8266AsyncUart1800KbpsMethod > neoPixelBus( totalLedCount );

#include "ledanim/fadeout.hpp"
#include "ledanim/stairway.hpp"

static constexpr size_t STATE_OFF = 0;
static constexpr size_t STATE_UPDOWN = 1;
static constexpr size_t STATE_WAITING = 2;
static constexpr size_t STATE_FADEOUT = 3;

static size_t state;

static void handleDown();
static void handleUp();
static void handleFadeout();
static void handleMotion( bool value );
static void animate();

IoTClass IoT( "Corridor/Light/Downstairs", "VillaKunterbunt2", "sacomoco02047781", "openhab", 1883 );

Input motionInput( gpioInput( 0, false ));
Device motionDevice( "MOTION", "NO", "YES" );

Command downCommand( "DOWN", handleDown );
Command upCommand( "UP", handleUp );

Timer animateTimer( animate );
Timer waitingTimer( handleFadeout );

FadeoutAnimation fadeoutAnimation;
StairwayAnimation stairwayAnimation;

static void handleDown()
{
    stairwayAnimation = StairwayAnimation( StairwayAnimation::FORWARD );
    state = STATE_UPDOWN;
}

static void handleUp()
{
    stairwayAnimation = StairwayAnimation( StairwayAnimation::REVERSE );
    state = STATE_UPDOWN;
}

static void handleFadeout()
{
    fadeoutAnimation = FadeoutAnimation();
    state = STATE_FADEOUT;
}

static void handleMotion( bool value )
{
    motionDevice.set( value );

    switch ( state ) {
        case STATE_OFF:
            if ( value ) {
                handleDown();
            }
            break;

        case STATE_UPDOWN:
            break;

        case STATE_WAITING:
            if ( value ) {
                waitingTimer.stop();
            } else {
                waitingTimer.start( 5000 );
            }
            break;

        case STATE_FADEOUT:
            if ( value ) {
                state = STATE_WAITING;
            }
            break;

        default:
            break;
    }
}

static void animate()
{
    switch ( state ) {
        case STATE_OFF:
            neoPixelBus.ClearTo( RgbColor( 0x00 ));
            break;

        case STATE_UPDOWN:
            if ( stairwayAnimation.animate()) {
                state = STATE_WAITING;
                if ( !motionInput.get()) {
                    waitingTimer.start( 5000 );
                }
            }
            break;

        case STATE_WAITING:
            neoPixelBus.ClearTo( RgbColor( 0xff ));
            break;

        case STATE_FADEOUT:
            if ( fadeoutAnimation.animate()) {
                state = STATE_OFF;
            }
            break;

        default:
            break;
    }
    neoPixelBus.Show();
    animateTimer.start( timePerFrame );
}

void setup()
{
    neoPixelBus.Begin();
    neoPixelBus.ClearTo( RgbColor( 0x00, 0x00, 0x00 ));
    neoPixelBus.Show();

    motionInput.changeEvent += handleMotion;

    IoT.begin();

    animateTimer.start( timePerFrame );
}

void loop()
{
    IoT.loop();
}