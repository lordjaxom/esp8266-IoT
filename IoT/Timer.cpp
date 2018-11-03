#include "IoT.hpp"
#include "Timer.hpp"

namespace iot {

    Timer::Timer( IoT& iot ) noexcept
    {
        iot.loopTickEvent += [this] { this->loop(); };
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
            expiredEvent();
        }
    }

} // namespace iot