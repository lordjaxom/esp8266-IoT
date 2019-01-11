#include <utility>

#include "IoT.hpp"
#include "Timer.hpp"

using namespace std;

namespace iot {

    Timer::Timer( IoT& iot, function< void() > handler ) noexcept
            : handler_( move( handler ))
    {
        iot.loopTickEvent += [this] { loop(); };
    }

    void Timer::start( uint32_t timeout )
    {
        timeout_ = timeout / IoT::tick;
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

} // namespace iot