#ifndef ESP8266_IOT_DEVICE_HPP
#define ESP8266_IOT_DEVICE_HPP

#include <functional>

namespace iot {

    class IoT;

    class Device
    {
    public:
        Device( IoT& iot, char const* name, std::function< void( bool value ) > action ) noexcept;

        void set(bool value);

    private:
        char const* name_;
        std::function< void( bool value ) > action_;
    };

} // namespace iot

#endif // ESP8266_IOT_DEVICE_HPP
