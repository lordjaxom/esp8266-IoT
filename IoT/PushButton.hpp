#ifndef ESP8266_IOT_PUSHBUTTON_HPP
#define ESP8266_IOT_PUSHBUTTON_HPP

#include <functional>

#include "Event.hpp"
#include "Timer.hpp"

class PushButton
{
public:
    explicit PushButton( std::function< bool() > input ) noexcept;
    PushButton( PushButton const& ) = delete;

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

#endif // ESP8266_IOT_PUSHBUTTON_HPP
