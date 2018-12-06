#ifndef ESP8266_IOT_DEVICE_HPP
#define ESP8266_IOT_DEVICE_HPP

#include <functional>

namespace iot {

    class IoT;

    class Device
    {
    public:
        Device( IoT& iot, char const* name, char const* stateName, char const* falseValue, char const* trueValue,
                std::function< void( bool value ) > action ) noexcept;
        Device( IoT& iot, char const* name, char const* stateName, char const* falseValue = "OFF", char const* trueValue = "ON" ) noexcept;
        Device( IoT& iot, char const* name, std::function< void( bool value ) > action = []( bool ) {} ) noexcept;

        void set( bool value );

    private:
        char const* name_;
        char const* stateName_;
        char const* falseValue_;
        char const* trueValue_;
        std::function< void( bool value ) > action_;
    };

} // namespace iot

#endif // ESP8266_IOT_DEVICE_HPP
