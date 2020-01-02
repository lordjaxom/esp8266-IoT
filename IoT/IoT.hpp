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
    static constexpr uint32_t watchdogDelay = 60000;
    static constexpr uint32_t reconnectDelay = 1000;

public:
    static char const *clientId;

    static constexpr uint32_t tick = 10;

    IoTClass( char const* topic, char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort ) noexcept;
    IoTClass( char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort ) noexcept;
    IoTClass( IoTClass const& ) = delete;

    void begin();
    void loop();

    void publish( String const& topic, String const& payload, bool retain = false );
    void subscribe( String topic, std::function< void( String message ) > handler );

    Event< void() > wiFiConnectedEvent;
    Event< void() > wiFiDisconnectedEvent;
    Event< void() > beginEvent;
    Event< void() > loopEvent;

private:
    IoTClass( String clientId, String topic,
              char const* wiFiSsid, char const* wiFiPassword,
              char const* mqttIp, uint16_t mqttPort ) noexcept;

    void connectToWiFi();
    void connectToMqtt();

    void wiFiConnected();
    void wiFiDisconnected();

    void mqttConnected();
    void mqttDisconnected();
    void mqttMessage( char const* topic, char const* payload, size_t length );

    String const clientId_; // must stay constant
    String const topic_; // must stay constant
    String const hostname_; // must stay constant
    Timer watchdogTimer_;

    char const* wiFiSsid_;
    char const* wiFiPassword_;
    WiFiEventHandler wiFiConnectHandler_;
    WiFiEventHandler wiFiDisconnectHandler_;
    Timer wiFiReconnectTimer_;

    char const* mqttIp_;
    uint16_t mqttPort_;
    String const mqttWillTopic_; // must stay constant
    Timer mqttReconnectTimer_;
    AsyncMqttClient mqttClient_;
    std::map< String, std::function< void( String payload ) > > mqttSubscriptions_;

    ESP8266WebServer webServer_;
    ESP8266HTTPUpdateServer updateServer_;

    uint32_t timestamp_ {};
};

#if defined( IOT_TARGET_NAME )
char const* IoTClass::clientId = IOT_TARGET_NAME;
#endif

extern IoTClass IoT;

#endif // ESP8266_IOT_IOT_HPP
