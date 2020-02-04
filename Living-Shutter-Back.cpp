#include <list>

#include "IoT/Gpio.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Logger.hpp"
#include "IoT/String.hpp"
#include "IoT/Timer.hpp"

static constexpr uint8_t pinUp = 4;
static constexpr uint8_t pinDown = 5;

static constexpr uint32_t timeUp = 29000;
static constexpr uint32_t timeDown = 26000;

static constexpr uint32_t triggerDelay = 100;

class Command
{
public:
    explicit Command( uint8_t pin )
            : pin_( pin ),
              timer_( [this] { expired(); } ) {}

    bool operator()()
    {
        if ( state_ == 0 ) {
            set( true );
        }
        return finished_;
    }

private:
    void set( bool value )
    {
        log( "setting button at pin ", static_cast< int >( pin_ ), " to ", value ? "HIGH" : "LOW" );
        digitalWrite( pin_, static_cast< uint8_t >( value ? HIGH : LOW ));
        timer_.start( triggerDelay );
        state_++;
    }

    void expired()
    {
        if ( state_ == 1 ) {
            set( false );
        } else {
            finished_ = true;
        }
    }

    uint8_t pin_;
    Timer timer_;
    unsigned state_ {};
    unsigned counter_ {};
    bool finished_ {};
};

IoTClass IoT( "Living/Shutter/Back", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

std::list< Command > commandQueue;

int8_t currentDirection = 0;
double currentPosition = 100;
double targetPosition = currentPosition;
uint32_t movementTimestamp;
uint32_t reportTimestamp;

static void triggerCommand()
{
    log( "triggering command ", currentDirection == 1 ? "DOWN" : "UP" );
    commandQueue.emplace_back( currentDirection == 1 ? pinDown : pinUp );
}

static void publishPosition()
{
    log( "publishing position ", static_cast< int >( currentPosition ));
    IoT.publish( str( "stat/", IoT.topic(), "/SHUTTER1" ), String( static_cast< int >( currentPosition )));
}

static void handleTopic( String const& message )
{
    double desiredPosition;
    if ( message == "UP" ) {
        desiredPosition = 0;
    } else if ( message == "DOWN" ) {
        desiredPosition = 100;
    } else if ( message == "STOP" ) {
        if ( currentDirection != 0 ) {
            log( "received STOP while moving" );
            triggerCommand();
            targetPosition = currentPosition;
            currentDirection = 0;
            publishPosition();
        }
        return;
    } else {
        log( "unknown command ", message );
        return;
    }

    if ( desiredPosition == targetPosition ) {
        log( "already moving to desired position, doing nothing" );
        return;
    }

    auto desiredDirection = static_cast< int8_t >( desiredPosition < currentPosition ? -1 : 1 );
    if ( currentDirection == desiredDirection ) {
        log( "already moving in desired direction, only updating target position" );
        targetPosition = desiredPosition;
        return;
    }

    if ( currentDirection != 0 ) {
        log( "moving in wrong direction, stopping" );
        triggerCommand();
    }

    currentDirection = desiredDirection;
    targetPosition = desiredPosition;
    movementTimestamp = millis();
    reportTimestamp = movementTimestamp - 1000;
    log( "starting movement" );
    triggerCommand();
}

static void handleTick()
{
    if ( !commandQueue.empty() && commandQueue.front()()) {
        commandQueue.erase( commandQueue.begin());
    }

    if ( currentDirection == 0 ) {
        return;
    }

    uint32_t timestamp = millis();
    if ( timestamp - reportTimestamp > 1000 ) {
        log( "direction ", static_cast< int >( currentDirection ), " position ", static_cast< int >( currentPosition ),
                " target ", static_cast< int >( targetPosition ));
        reportTimestamp = timestamp;
    }

    uint32_t elapsed = timestamp - movementTimestamp;
    double moved = elapsed * 100.0 / ( currentDirection == 1 ? timeDown : timeUp ) * currentDirection;
    currentPosition += moved;
    if ( ( currentDirection == 1 && currentPosition >= targetPosition ) ||
            ( currentDirection == -1 && currentPosition <= targetPosition ) ){
        if ( targetPosition == 0 || targetPosition == 100 ) {
            log( "reached final position, doing nothing" );
        } else {
            log( "reached target position, stopping" );
            triggerCommand();
        }
        currentPosition = targetPosition;
        currentDirection = 0;
        publishPosition();
    }
    movementTimestamp = millis();
}

void setup()
{
    pinMode( pinUp, OUTPUT );
    pinMode( pinDown, OUTPUT );

    IoT.loopEvent += &handleTick;

    IoT.subscribe( str( "cmnd/", IoT.topic(), "/SHUTTER1" ), &handleTopic );

    IoT.begin();
}

void loop()
{
    IoT.loop();
}