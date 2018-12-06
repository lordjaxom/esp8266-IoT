#ifndef ESP8266_IOT_WIFI_HPP
#define ESP8266_IOT_WIFI_HPP

#include <ESP8266WiFi.h>

namespace iot {

    class IoT;

    class WiFi
    {
    public:
        WiFi( IoT& iot, char const* ssid, char const* password ) noexcept;

        IoT& iot;

    private:
        void begin();

        char const* ssid_;
        char const* password_;
    };

} // namespace iot

#endif // ESP8266_IOT_WIFI_HPP
