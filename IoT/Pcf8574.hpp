#ifndef ESP8266_IOT_PCF8574_HPP
#define ESP8266_IOT_PCF8574_HPP

#include <stdint.h>

class WireConfig;

class Pcf8574
{
public:
    Pcf8574( WireConfig& wire, uint8_t address, uint8_t outputMask ) noexcept;
    Pcf8574( Pcf8574 const& ) = delete;

    bool read( uint8_t index ) const { return static_cast< bool >( input_ & ( 1 << index )); }

    bool get( uint8_t index ) const { return static_cast< bool >( output_ & ( 1 << index )); }

    void set( uint8_t index, bool value );

private:
    void loop();

    uint8_t address_;
    uint8_t outputMask_;
    uint8_t output_;
    bool dirty_ { true };
    uint8_t input_ {};
};

#endif // ESP8266_IOT_PCF8574_HPP
