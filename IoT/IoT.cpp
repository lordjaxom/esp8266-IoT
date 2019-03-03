#include <algorithm>

#include "IoT.hpp"
#include "Logger.hpp"
#include "String.hpp"

using namespace std;

IoTClass::IoTClass( char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort ) noexcept
        : watchdogTimer_( [this] { ESP.restart(); } ),
          webServer_( 80 ),
          wiFiSsid_( wiFiSsid ),
          wiFiPassword_( wiFiPassword ),
          wiFiReconnectTimer_( [this] { connectToWiFi(); } ),
          mqttIp_( mqttIp ),
          mqttPort_( mqttPort ),
          mqttReconnectTimer_( [this] { connectToMqtt(); } )
{
}

void IoTClass::begin()
{
    Logger.begin();

    delay( 1000 );

    log( "starting ESP-IoT based application" );

    watchdogTimer_.start( watchdogDelay );

    wiFiConnectHandler_ = WiFi.onStationModeGotIP( [this]( WiFiEventStationModeGotIP const& ) { wiFiConnected(); } );
    wiFiDisconnectHandler_ = WiFi.onStationModeDisconnected(
            [this]( WiFiEventStationModeDisconnected const& ) { wiFiDisconnected(); } );
    connectToWiFi();

    mqttClient_.onConnect( [this]( bool ) { mqttConnected(); } );
    mqttClient_.onDisconnect( [this]( AsyncMqttClientDisconnectReason ) { mqttDisconnected(); } );
    mqttClient_.onMessage( [this]( char const* topic, char const* payload, AsyncMqttClientMessageProperties, size_t length,
                                   size_t, size_t ) { mqttMessage( topic, payload, length ); } );
    mqttClient_.setClientId( clientId );
    mqttClient_.setServer( mqttIp_, mqttPort_ );

    updateServer_.setup( &webServer_, "/update", "admin", "admin" );
    webServer_.begin();

    beginEvent();
}

void IoTClass::loop()
{
    webServer_.handleClient();

    loopAlwaysEvent();

    uint32_t timestamp = millis();
    uint32_t elapsed = timestamp - timestamp_;
    if ( elapsed > tick ) {
        timestamp_ = timestamp;
        loopTickEvent();
    }
}

void IoTClass::publish( String topic, String payload )
{
    log( "publishing ", payload, " to ", topic );

    mqttClient_.publish( topic.c_str(), 1, false, payload.c_str());
}

void IoTClass::subscribe( String topic, std::function< void( String message ) > handler )
{
    mqttSubscriptions_.emplace( move( topic ), move( handler ));
}

void IoTClass::connectToWiFi()
{
    String hostname = str( "IoT-", clientId );

    log( "connecting to WiFi at ", wiFiSsid_, " as ", hostname );

    WiFi.mode( WIFI_STA );
    WiFi.hostname( hostname );
    WiFi.begin( wiFiSsid_, wiFiPassword_ );
}

void IoTClass::connectToMqtt()
{
    log( "connecting to MQTT broker at ", mqttIp_, " as ", clientId );

    mqttClient_.connect();
}

void IoTClass::wiFiConnected()
{
    log( "connection to WiFi established as ", WiFi.localIP());

    wiFiConnectedEvent();
    connectToMqtt();
}

void IoTClass::wiFiDisconnected()
{
    log( "connection to WiFi lost" );

    wiFiDisconnectedEvent();

    mqttReconnectTimer_.stop();
    wiFiReconnectTimer_.start( reconnectDelay );

    if ( !watchdogTimer_.active() ) {
        watchdogTimer_.start( watchdogDelay );
    }
}

void IoTClass::mqttConnected()
{
    log( "connection to MQTT broker established" );

    watchdogTimer_.stop();

    for ( auto const& subscription : mqttSubscriptions_ ) {
        log( "subscribing to ", subscription.first );

        mqttClient_.subscribe( subscription.first.c_str(), 0 );
    }
}

void IoTClass::mqttDisconnected()
{
    log( "connection to MQTT broker lost" );

    if ( WiFi.isConnected()) {
        mqttReconnectTimer_.start( reconnectDelay );
    }

    if ( !watchdogTimer_.active() ) {
        watchdogTimer_.start( watchdogDelay );
    }
}

void IoTClass::mqttMessage( char const* topic, char const* payload, size_t length )
{
    String message;
    message.reserve( length );
    for_each( payload, payload + length, [&]( char ch ) { message += ch; } );

    log( "received ", message, " from ", topic );

    mqttSubscriptions_.find( topic )->second( move( message ));
}
