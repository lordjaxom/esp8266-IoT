#ifndef ESP8266_IOT_DEVICE_HPP
#define ESP8266_IOT_DEVICE_HPP

#include <functional>

namespace iot {

    class IoT;

    class Device
    {
    public:
        Device( IoT& iot, String name, char const* stateName, char const* falseValue, char const* trueValue,
                std::function< void( bool value ) > action ) noexcept;
        Device( IoT& iot, String name, char const* stateName, char const* falseValue = "OFF", char const* trueValue = "ON" ) noexcept;
        Device( IoT& iot, String name, std::function< void( bool value ) > action = []( bool ) {} ) noexcept;
        Device( Device const& ) = delete;

        String const& name() const { return name_; }

        bool get() const { return value_; }

        void set( bool value );

        void toggle() { set( !value_ ); }

    private:
        IoT& iot_;
        String name_;
        char const* stateName_;
        char const* falseValue_;
        char const* trueValue_;
        std::function< void( bool value ) > action_;
        bool value_ {};
    };

} // namespace iot

#endif // ESP8266_IOT_DEVICE_HPP
