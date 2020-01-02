#include <algorithm>
#include <utility>

#include "IoT.hpp"
#include "Logger.hpp"
#include "String.hpp"

using namespace std;

static String toClientId( char const* topic )
{
    String clientId( topic );
    clientId.replace( '/', '-' );
    return clientId;
}

IoTClass::IoTClass( char const* topic, char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort ) noexcept
        : IoTClass( toClientId( topic ), topic, wiFiSsid, wiFiPassword, mqttIp, mqttPort )
{
}

IoTClass::IoTClass( char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort ) noexcept
        : IoTClass( clientId, clientId, wiFiSsid, wiFiPassword, mqttIp, mqttPort )
{
}

IoTClass::IoTClass( String clientId, String topic,
                    char const* wiFiSsid, char const* wiFiPassword,
                    char const* mqttIp, uint16_t mqttPort ) noexcept
        : clientId_( move( clientId )),
          topic_( move( topic )),
          hostname_( str( "IoT-", clientId_ )),
          watchdogTimer_( [] { ESP.restart(); } ),
          wiFiSsid_( wiFiSsid ),
          wiFiPassword_( wiFiPassword ),
          wiFiReconnectTimer_( [this] { connectToWiFi(); } ),
          mqttIp_( mqttIp ),
          mqttPort_( mqttPort ),
          mqttWillTopic_( str( "tele/", topic_, "/LWT" )),
          mqttReconnectTimer_( [this] { connectToMqtt(); } ),
          webServer_( 80 )
{
}

void IoTClass::begin()
{
    Logger.begin();

    delay( 1000 );

    log( "starting ESP-IoT based application" );

    watchdogTimer_.start( watchdogDelay );

    wiFiConnectHandler_ = WiFi.onStationModeGotIP( [this]( WiFiEventStationModeGotIP const& ) { wiFiConnected(); } );
    wiFiDisconnectHandler_ = WiFi.onStationModeDisconnected( [this]( WiFiEventStationModeDisconnected const& ) { wiFiDisconnected(); } );
    connectToWiFi();

    mqttClient_.onConnect( [this]( bool ) { mqttConnected(); } );
    mqttClient_.onDisconnect( [this]( AsyncMqttClientDisconnectReason ) { mqttDisconnected(); } );
    mqttClient_.onMessage( [this]( char const* topic, char const* payload, AsyncMqttClientMessageProperties, size_t length,
                                   size_t, size_t ) { mqttMessage( topic, payload, length ); } );
    mqttClient_.setClientId( clientId_.c_str() );
    mqttClient_.setServer( mqttIp_, mqttPort_ );
    mqttClient_.setWill( mqttWillTopic_.c_str(), 1, true, "Offline" );

    updateServer_.setup( &webServer_, "/update", "admin", "admin" );

    beginEvent();

    timestamp_ = millis();
}

void IoTClass::loop()
{
    webServer_.handleClient();

    loopEvent();

    uint32_t elapsed = millis() - timestamp_;
    if ( elapsed < tick ) {
        delay( tick - elapsed );
    }
    timestamp_ = millis();
}

void IoTClass::publish( String const& topic, String const& payload, bool retain )
{
    if ( !mqttClient_.connected()) {
        return;
    }

    log( "publishing ", payload, " to ", topic );

    mqttClient_.publish( topic.c_str(), 1, retain, payload.c_str());
}

void IoTClass::subscribe( String topic, std::function< void( String message ) > handler )
{
    mqttSubscriptions_.emplace( move( topic ), move( handler ));
}

void IoTClass::connectToWiFi()
{
    log( "connecting to WiFi at ", wiFiSsid_, " as ", hostname_ );

    WiFi.persistent( false );
    WiFi.mode( WIFI_STA );
    WiFi.hostname( hostname_ );
    WiFi.begin( wiFiSsid_, wiFiPassword_ );
}

void IoTClass::connectToMqtt()
{
    log( "connecting to MQTT broker at ", mqttIp_, " as ", clientId_ );

    mqttClient_.connect();
}

void IoTClass::wiFiConnected()
{
    log( "connection to WiFi established as ", WiFi.localIP());

    connectToMqtt();
    webServer_.begin();

    wiFiConnectedEvent();
}

void IoTClass::wiFiDisconnected()
{
    log( "connection to WiFi lost" );

    mqttReconnectTimer_.stop();
    wiFiReconnectTimer_.start( reconnectDelay );

    if ( !watchdogTimer_.active() ) {
        watchdogTimer_.start( watchdogDelay );
    }

    wiFiDisconnectedEvent();
}

void IoTClass::mqttConnected()
{
    log( "connection to MQTT broker established" );

    watchdogTimer_.stop();

    publish( mqttWillTopic_, "Online", true );

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
