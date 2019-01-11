#include <algorithm>
#include <utility>

#include "IoT.hpp"
#include "Logger.hpp"
#include "MQTT.hpp"
#include "WiFi.hpp"

namespace iot {

    MQTT* MQTT::instance;

    void MQTT::publish( String topic, String payload )
    {
        if ( instance == nullptr ) {
            return;
        }

        instance->iot.enqueue( [topic = std::move( topic ), payload = std::move( payload )] {
            log( "publishing ", payload, " to ", topic );
            instance->pubSubClient_.publish( topic.c_str(), payload.c_str());
        } );
    }

    void MQTT::subscribe( String topic, std::function< void( String payload ) > handler )
    {
        if ( instance == nullptr ) {
            return;
        }

        log( "subscribing to ", topic );

        instance->pubSubClient_.subscribe( topic.c_str());
        instance->subscriptions_.emplace( std::move( topic ), std::move( handler ));
    }

    MQTT::MQTT( WiFi& wiFi, char const* ip, uint16_t port, char const* clientId ) noexcept
            : iot( wiFi.iot ),
              ip_( ip ),
              port_( port ),
              clientId_( clientId ),
              pubSubClient_( ip_, port_, wiFiClient_ ),
              reconnectTimer_( iot )
    {
        instance = this;

        iot.loopAlwaysEvent += [this] { this->loop(); };
        reconnectTimer_.expiredEvent += [this] { this->connect(); };

        pubSubClient_.setCallback( [this]( char* topic, uint8_t* payload, unsigned length ) {
            String message;
            message.reserve( length );
            std::for_each( payload, payload + length, [&]( uint8_t ch ) { message += static_cast< char >( ch ); } );

            log( "received ", message, " from ", topic );

            subscriptions_.find( topic )->second( std::move( message ));
        } );
    }

    void MQTT::connect()
    {
        log( "connecting to MQTT broker at ", ip_, " as ", clientId_ );
        if ( pubSubClient_.connect( clientId_ )) {
            log( "connection to MQTT broker established" );
            for ( auto const& subscription : subscriptions_ ) {
                pubSubClient_.subscribe( subscription.first.c_str());
            }
        } else {
            log( "ERROR connecting to MQTT broker: ", pubSubClient_.state());
            reconnectTimer_.start( 1000 );
        }
    }

    void MQTT::reconnect()
    {
        if ( !reconnectTimer_.active()) {
            connect();
        }
    }

    void MQTT::loop()
    {
        if ( !pubSubClient_.connected()) {
            reconnect();
        } else {
            pubSubClient_.loop();
        }
    }

} // namespace iot