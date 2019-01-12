#ifndef ESP8266_IOT_IOT_HPP
#define ESP8266_IOT_IOT_HPP

#include <stdint.h>
#include <functional>
#include <map>

#include <AsyncMqttClient.h>
#include <ESP8266WiFi.h>

#include "Event.hpp"
#include "Timer.hpp"

namespace iot {

    class IoT
    {
    public:
        static constexpr uint32_t tick = 10;

        IoT( char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort, char const* mqttClientId ) noexcept;
        IoT( IoT const& ) = delete;

        void begin();
        void loop();

        void publish( String topic, String payload );
        void subscribe( String topic, std::function< void( String message ) > handler );

        Event< void() > beginEvent;
        Event< void() > loopAlwaysEvent;
        Event< void() > loopTickEvent;

    private:
        void connectToWiFi();
        void connectToMqtt();

        void wiFiConnected();
        void wiFiDisconnected();

        void mqttConnected();
        void mqttDisconnected();
        void mqttMessage( char const* topic, char const* payload, size_t length );

        char const* wiFiSsid_;
        char const* wiFiPassword_;
        WiFiEventHandler wiFiConnectHandler_;
        WiFiEventHandler wiFiDisconnectHandler_;
        Timer wiFiReconnectTimer_;

        char const* mqttIp_;
        uint16_t mqttPort_;
        char const* mqttClientId_;
        Timer mqttReconnectTimer_;
        AsyncMqttClient mqttClient_;
        std::map< String, std::function< void ( String payload ) > > mqttSubscriptions_;

        uint32_t lastLoop_ {};
    };

} // namespace iot

#endif // ESP8266_IOT_IOT_HPP
