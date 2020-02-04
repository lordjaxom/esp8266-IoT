#ifndef ESP8266_IOT_DEBOUNCE_HPP
#define ESP8266_IOT_DEBOUNCE_HPP

#include <stdint.h>
#include <utility>

#include "IoT.hpp"
#include "Logger.hpp"

template< typename Input >
class Debounce
{
    static constexpr unsigned delay = 50 / IoTClass::tick;

public:
    explicit Debounce( Input&& input ) noexcept
            : input_( std::move( input ))
    {
    }

    bool operator()()
    {
        bool state = input_();
        if ( !state ) {
            if ( state_ ) {
                counter_ = delay;
            } else if ( counter_ > 0 && --counter_ == 0 ) {
                value_ = false;
            }
        } else if ( !state_ ) {
            value_ = true;
        }
        state_ = state;
        return value_;
    }

private:
    Input input_;
    bool state_ {};
    bool value_ {};
    unsigned counter_ {};
};

template< typename Input >
Debounce< Input > debounce( Input&& input )
{
    return Debounce< Input >( std::forward< Input >( input ));
}

#endif // ESP8266_IOT_DEBOUNCE_HPP
