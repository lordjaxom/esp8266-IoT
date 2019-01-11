#ifndef ESP8266_IOT_MQTT_HPP
#define ESP8266_IOT_MQTT_HPP

#include <stdint.h>
#include <functional>
#include <map>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "Timer.hpp"

namespace iot {

    class IoT;
    class WiFi;

    class MQTT
    {
    public:
        static void publish( String topic, String payload );
        static void subscribe( String topic, std::function< void( String message ) > handler );

        MQTT( WiFi& wiFi, char const* ip, uint16_t port, char const* clientId ) noexcept;

        IoT& iot;

    private:
        static MQTT* instance;

        void connect();
        void reconnect();
        void loop();

        char const* ip_;
        uint16_t port_;
        char const* clientId_;
        WiFiClient wiFiClient_;
        PubSubClient pubSubClient_;
        Timer reconnectTimer_;
        std::map< String, std::function< void( String message ) > > subscriptions_;
    };

} // namespace iot

#endif // ESP8266_IOT_MQTT_HPP
