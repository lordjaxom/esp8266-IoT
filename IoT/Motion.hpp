#ifndef ESP8266_IOT_MOTION_HPP
#define ESP8266_IOT_MOTION_HPP

#include <stdint.h>
#include <utility>

#include "IoT.hpp"

template< typename Input >
class Motion
{
    using Type = decltype( std::declval< Input >()());

public:
    Motion( uint32_t timeout, Input&& input ) noexcept
            : timeout_( timeout / IoTClass::tick ),
              input_( std::move( input ))
    {
    }

    Type operator()()
    {
        if ( input_()) {
            value_ = true;
            remaining_ = 0;
        } else if ( value_ ) {
            if ( remaining_ == 0 ) {
                remaining_ = timeout_;
            } else if ( --remaining_ == 0 ) {
                value_ = false;
            }
        }
        return value_;
    }

private:
    uint32_t timeout_;
    Input input_;
    Type value_ {};
    uint32_t remaining_ {};
};

template< typename Input >
Motion< Input > motion( uint32_t timeout, Input&& input )
{
    return Motion< Input >( timeout, std::forward< Input >( input ));
}

#endif // ESP8266_IOT_MOTION_HPP
