#ifndef ESP8266_IOT_IOT_HPP
#define ESP8266_IOT_IOT_HPP

#include <stdint.h>
#include <deque>
#include <vector>
#include <functional>

#include "Event.hpp"

namespace iot {

    class IoT
    {
    public:
        static constexpr uint32_t tick = 10;

        IoT() = default;

        void begin();
        void loop();

        void enqueue( std::function< void () > action );

        Event< void() > beginEvent;
        Event< void() > loopAlwaysEvent;
        Event< void() > loopTickEvent;

    private:
        uint32_t lastLoop_ {};
        std::deque< std::function< void() > > queue_;
    };

} // namespace iot

#endif // ESP8266_IOT_IOT_HPP
