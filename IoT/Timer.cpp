#include <utility>

#include "IoT.hpp"
#include "Timer.hpp"

using namespace std;

Timer::Timer( function< void() > handler ) noexcept
        : subscription_( IoT.loopEvent.subscribe( [this] { loop(); } )),
          handler_( move( handler ))
{
}

void Timer::start( uint32_t timeout )
{
    timeout_ = timeout;
}

void Timer::stop()
{
    timeout_ = 0;
}

void Timer::loop()
{
    if ( timeout_ > IoTClass::tick ) {
        timeout_ -= IoTClass::tick;
    } else if ( timeout_ > 0 ) {
        timeout_ = 0;
        handler_();
    }
}
