#ifndef ESP8266_IOT_GPIO_HPP
#define ESP8266_IOT_GPIO_HPP

#include <functional>

#include "PushButton.hpp"
#include "Device.hpp"


class GpioInput
{
public:
    explicit GpioInput( uint8_t pin, bool pullUp = true ) noexcept;

    bool operator()() const;

private:
    uint8_t pin_;
    bool pullUp_;
};

class GpioOutput
{
public:
    explicit GpioOutput( uint8_t pin, bool invert = false ) noexcept;

    void operator()( bool value );

private:
    uint8_t pin_;
    bool invert_;
};


#endif // ESP8266_IOT_GPIO_HPP
