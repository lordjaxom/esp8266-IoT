#ifndef ESP8266_IOT_IOT_HPP
#define ESP8266_IOT_IOT_HPP

#include <stdint.h>
#include <functional>
#include <map>

#include <AsyncMqttClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "Event.hpp"
#include "Timer.hpp"

class IoTClass
{
    static constexpr uint32_t watchdogDelay = 10000;
    static constexpr uint32_t reconnectDelay = 1000;

    static char const *clientId;

public:
    static constexpr uint32_t tick = 10;

    IoTClass( char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort ) noexcept;
    IoTClass( IoTClass const& ) = delete;

    void begin();
    void loop();

    void publish( String topic, String payload );
    void subscribe( String topic, std::function< void( String message ) > handler );

    Event< void() > wiFiConnectedEvent;
    Event< void() > wiFiDisconnectedEvent;
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

    Timer watchdogTimer_;

    ESP8266WebServer webServer_;
    ESP8266HTTPUpdateServer updateServer_;

    char const* wiFiSsid_;
    char const* wiFiPassword_;
    WiFiEventHandler wiFiConnectHandler_;
    WiFiEventHandler wiFiDisconnectHandler_;
    Timer wiFiReconnectTimer_;

    char const* mqttIp_;
    uint16_t mqttPort_;
    Timer mqttReconnectTimer_;
    AsyncMqttClient mqttClient_;
    std::map< String, std::function< void( String payload ) > > mqttSubscriptions_;

    uint32_t timestamp_ {};
};

#if defined( IOT_TARGET_NAME )
char const* IoTClass::clientId = IOT_TARGET_NAME;
#endif

extern IoTClass IoT;

#endif // ESP8266_IOT_IOT_HPP
