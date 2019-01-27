#include <utility>

#include "IoT.hpp"
#include "Timer.hpp"

using namespace std;

Timer::Timer( function< void() > handler ) noexcept
        : handler_( move( handler ))
{
    IoT.loopTickEvent += [this] { loop(); };
}

void Timer::start( uint32_t timeout )
{
    timeout_ = timeout / IoTClass::tick;
}

void Timer::stop()
{
    timeout_ = 0;
}

void Timer::loop()
{
    if ( timeout_ > 0 && --timeout_ == 0 ) {
        handler_();
    }
}
