#ifndef ESP8266_IOT_DEVICE_HPP
#define ESP8266_IOT_DEVICE_HPP

#include <functional>

class Device
{
public:
    /**
     * Initializes a switchable device.
     *
     * Publishes to stat/<name>/<stateName>
     * Subscribes to cmnd/<name>/<stateName> if action is specified
     *
     * @param name
     * @param stateName
     * @param falseValue
     * @param trueValue
     * @param action
     */
    Device( String name, String state, char const* falseValue = "OFF", char const* trueValue = "ON",
            std::function< void( bool value ) > action = nullptr ) noexcept;

    /**
     * Initializes a switchable device with falseValue "OFF" and trueValue "ON".
     *
     * @param name
     * @param stateName
     * @param action
     */
    Device( String name, char const* stateName, std::function< void( bool value ) > action = nullptr ) noexcept;

    /**
     * Initializes a switchable device with name from IoT, falseValue "OFF" and trueValue "ON".
     *
     * @param state
     * @param action
     */
    explicit Device( String state, std::function< void( bool value ) > action = nullptr ) noexcept;

    /**
     * Initializes a switchable device with name from IoT, state "POWER", falseValue "OFF" and trueValue "ON".
     *
     * @param action
     */
    explicit Device( std::function< void( bool value ) > action = nullptr ) noexcept;

    Device( Device const& ) = delete;

    String const& name() const { return name_; }

    bool get() const { return value_; }

    void set( bool value );

    void toggle() { set( !value_ ); }

private:
    void update();

    String name_;
    String state_;
    char const* falseValue_;
    char const* trueValue_;
    std::function< void( bool value ) > action_;
    bool value_ {};
};

#endif // ESP8266_IOT_DEVICE_HPP
