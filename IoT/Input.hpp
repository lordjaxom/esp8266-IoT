#ifndef ESP8266_IOT_INPUT_HPP
#define ESP8266_IOT_INPUT_HPP

#include <functional>

#include "Event.hpp"

class Input
{
public:
    explicit Input( std::function< bool() > input ) noexcept;
    Input( Input const& ) = delete;

    Event< void( bool ) > changeEvent;

private:
    void loop();

    std::function< bool() > input_;
    bool value_ {};
};

#endif // ESP8266_IOT_INPUT_HPP
