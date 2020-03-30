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
    bool finished_ {};
};

IoTClass IoT( "Living/Shutter/Back", "akvsoft", "sacomoco02047781", "192.168.178.28", 1883 );

std::list< Command > commandQueue;

int currentDirection = 0;
double currentPosition = 100;
double targetPosition = currentPosition;
uint32_t movementTimestamp;
uint32_t reportTimestamp;

template< typename... Args >
static void log2( Args&&... args )
{
    String message = str( std::forward< Args >( args )... );
    log( message );
    IoT.publish( str( "tele/", IoT.topic(), "/TRACE" ), message );
}

static void triggerCommand()
{
    log2( "triggering command ", currentDirection == 1 ? "DOWN" : "UP" );
    commandQueue.emplace_back( currentDirection == 1 ? pinDown : pinUp );
}

static void publishPosition()
{
    String position( currentPosition, 0 );
    log( "publishing position ", position );
    IoT.publish( str( "stat/", IoT.topic(), "/SHUTTER1" ), position );
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
            log2( "received STOP while moving" );
            triggerCommand();
            targetPosition = currentPosition;
            currentDirection = 0;
            publishPosition();
        }
        return;
    } else {
        char* ptr;
        unsigned desired = strtoul( message.c_str(), &ptr, 10 );
        if ( *ptr == '\0' && desired >= 0 && desired <= 100 ) {
            desiredPosition = desired;
        } else {
            log2( "unknown command ", message );
            return;
        }
    }

    int desiredDirection;
    if ( ( desiredPosition == 0 || desiredPosition == 100 ) && currentDirection == 0 ) {
        log2( "issuing hard move command due to UP or DOWN" );
        desiredDirection = desiredPosition == 0 ? -1 : 1;
    } else {
        if ( desiredPosition == targetPosition ) {
            log2( "already moving to desired position, doing nothing" );
            return;
        }

        desiredDirection = desiredPosition < currentPosition ? -1 : 1;
        if ( currentDirection == desiredDirection ) {
            log2( "already moving in desired direction, only updating target position" );
            targetPosition = desiredPosition;
            return;
        }

        if ( currentDirection != 0 ) {
            log2( "moving in wrong direction, stopping" );
            triggerCommand();
        }
    }

    currentDirection = desiredDirection;
    targetPosition = desiredPosition;
    movementTimestamp = millis();
    reportTimestamp = movementTimestamp - 1000;
    log2( "starting movement" );
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
        log2( "direction ", String( static_cast< int >( currentDirection )), " position ", String( currentPosition, 0 ),
                " target ", String( targetPosition, 0 ));
        reportTimestamp = timestamp;
    }

    uint32_t elapsed = timestamp - movementTimestamp;
    double moved = elapsed * 100.0 / ( currentDirection == 1 ? timeDown : timeUp ) * currentDirection;
    currentPosition += moved;
    if ( ( currentDirection == 1 && currentPosition >= targetPosition ) ||
            ( currentDirection == -1 && currentPosition <= targetPosition ) ){
        if ( targetPosition == 0 || targetPosition == 100 ) {
            log2( "reached final position, doing nothing" );
        } else {
            log2( "reached target position, stopping" );
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