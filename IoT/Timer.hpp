#ifndef ESP8266_IOT_TIMER_HPP
#define ESP8266_IOT_TIMER_HPP

#include <functional>
#include <utility>

#include "Event.hpp"

namespace iot {

    class IoT;

    class Timer
    {
    public:
        explicit Timer( IoT& iot ) noexcept;

        bool active() const { return timeout_ > 0; }

        void start( uint32_t timeout );
        void stop();

        Event< void() > expiredEvent;

    private:
        void loop();

        uint32_t timeout_ {};
    };

} // namespace iot

#endif //ESP8266_IOT_TIMER_HPP
