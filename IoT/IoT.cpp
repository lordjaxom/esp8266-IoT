#include <algorithm>

#include "IoT.hpp"
#include "Logger.hpp"

using namespace std;


IoT* IoT::instance;

IoT::IoT( char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp, uint16_t mqttPort, char const* mqttClientId ) noexcept
        : wiFiSsid_( wiFiSsid ),
          wiFiPassword_( wiFiPassword ),
          wiFiReconnectTimer_( [this] { connectToWiFi(); } ),
          mqttIp_( mqttIp ),
          mqttPort_( mqttPort ),
          mqttClientId_( mqttClientId ),
          mqttReconnectTimer_( [this] { connectToMqtt(); } )
{
}

void IoT::begin()
{
    instance = this;

#if defined( LOGGER_SERIAL )
    Serial.begin( 115200 );
    while ( !Serial ) {}
#endif

    delay( 1000 );

    log( "starting ESP-IoT based application" );

    wiFiConnectHandler_ = WiFi.onStationModeGotIP( [this]( WiFiEventStationModeGotIP const& ) { wiFiConnected(); } );
    wiFiDisconnectHandler_ = WiFi.onStationModeDisconnected(
            [this]( WiFiEventStationModeDisconnected const& ) { wiFiDisconnected(); } );
    connectToWiFi();

    mqttClient_.onConnect( [this]( bool ) { mqttConnected(); } );
    mqttClient_.onDisconnect( [this]( AsyncMqttClientDisconnectReason ) { mqttDisconnected(); } );
    mqttClient_.onMessage( [this]( char const* topic, char const* payload, AsyncMqttClientMessageProperties, size_t length,
                                   size_t, size_t ) { mqttMessage( topic, payload, length ); } );
    mqttClient_.setClientId( mqttClientId_ );
    mqttClient_.setServer( mqttIp_, mqttPort_ );

    beginEvent();
}

void IoT::loop()
{
    loopAlwaysEvent();

    uint32_t timestamp = millis();
    uint32_t elapsed = timestamp - timestamp_;
    if ( elapsed > tick ) {
        timestamp_ = timestamp;
        loopTickEvent();
    }
}

void IoT::publish( String topic, String payload )
{
    log( "publishing ", payload, " to ", topic );

    mqttClient_.publish( topic.c_str(), 1, false, payload.c_str());
}

void IoT::subscribe( String topic, std::function< void( String message ) > handler )
{
    mqttSubscriptions_.emplace( move( topic ), move( handler ));
}

void IoT::connectToWiFi()
{
    log( "connecting to WiFi at ", wiFiSsid_ );

    WiFi.mode( WIFI_STA );
    WiFi.begin( wiFiSsid_, wiFiPassword_ );
}

void IoT::connectToMqtt()
{
    log( "connecting to MQTT broker at ", mqttIp_, " as ", mqttClientId_ );

    mqttClient_.connect();
}

void IoT::wiFiConnected()
{
    log( "connection to WiFi established as ", WiFi.localIP());

    connectToMqtt();
}

void IoT::wiFiDisconnected()
{
    log( "connection to WiFi lost" );

    mqttReconnectTimer_.stop();
    wiFiReconnectTimer_.start( 1000 );
}

void IoT::mqttConnected()
{
    log( "connection to MQTT broker established" );

    for ( auto const& subscription : mqttSubscriptions_ ) {
        log( "subscribing to ", subscription.first );

        mqttClient_.subscribe( subscription.first.c_str(), 0 );
    }
}

void IoT::mqttDisconnected()
{
    log( "connection to MQTT broker lost" );

    if ( WiFi.isConnected()) {
        mqttReconnectTimer_.start( 1000 );
    }
}

void IoT::mqttMessage( char const* topic, char const* payload, size_t length )
{
    String message;
    message.reserve( length );
    for_each( payload, payload + length, [&]( char ch ) { message += ch; } );

    log( "received ", message, " from ", topic );

    mqttSubscriptions_.find( topic )->second( move( message ));
}

