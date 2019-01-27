#ifndef ESP8266_IOT_AVRI2C_HPP
#define ESP8266_IOT_AVRI2C_HPP

#include <stdint.h>

namespace iot {

    class WireConfig;

    class AvrI2C
    {
    public:
        explicit AvrI2C( WireConfig& wire, uint8_t address );

        AvrI2C( AvrI2C const& ) = delete;

        void set( size_t index, bool value )
        {
            outputs_[index] = value;
            dirty_[index] = true;
        }

        bool read() const { return input_; }

    private:
        void loop();

        uint8_t address_;
        bool outputs_[2] {};
        bool dirty_[2] { true, true };
        bool input_ {};
    };

} // namespace iot

#endif // ESP8266_IOT_AVRI2C_HPP
