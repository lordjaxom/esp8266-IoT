#ifndef ESP8266_IOT_DEVICE_HPP
#define ESP8266_IOT_DEVICE_HPP

#include <functional>

class Device
{
public:
    Device( String name, char const* stateName, char const* falseValue = "OFF", char const* trueValue = "ON",
            std::function< void( bool value ) > action = nullptr ) noexcept;
    explicit Device( String name, std::function< void( bool value ) > action = nullptr ) noexcept;
    Device( Device const& ) = delete;

    String const& name() const { return name_; }

    bool get() const { return value_; }

    void set( bool value );

    void toggle() { set( !value_ ); }

private:
    void update();

    String name_;
    char const* stateName_;
    char const* falseValue_;
    char const* trueValue_;
    std::function< void( bool value ) > action_;
    bool value_ {};
};

#endif // ESP8266_IOT_DEVICE_HPP
