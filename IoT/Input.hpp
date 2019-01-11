#ifndef ESP8266_IOT_INPUT_HPP
#define ESP8266_IOT_INPUT_HPP

#include <functional>

#include "Event.hpp"

namespace iot {

    class IoT;

    class Input
    {
    public:
        Input( IoT& iot, std::function< bool() > input ) noexcept;
        Input( Input const& ) = delete;

        Event< void( bool ) > changeEvent;

    private:
        void loop();

        std::function< bool() > input_;
        bool value_ {};
    };

} // namespace iot

#endif // ESP8266_IOT_INPUT_HPP
