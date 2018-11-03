#ifndef ESP8266_IOT_PUSHBUTTON_HPP
#define ESP8266_IOT_PUSHBUTTON_HPP

#include <functional>
#include <utility>

#include "Event.hpp"
#include "Timer.hpp"

namespace iot {

    class IoT;

    class PushButton
    {
    public:
        PushButton( IoT& iot, std::function< bool() > input ) noexcept;

        Event< void( unsigned clicks ) > clickedEvent;

    private:
        void loop();
        void expired();

        std::function< bool() > input_;
        Timer timer_;
        bool value_ {};
        unsigned clicks_ {};
        bool finished_ {};
    };

} // namespace iot

#endif // ESP8266_IOT_PUSHBUTTON_HPP
