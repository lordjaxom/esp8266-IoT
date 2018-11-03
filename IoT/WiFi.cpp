#include "IoT.hpp"
#include "Logger.hpp"
#include "WiFi.hpp"

namespace iot {

    WiFi::WiFi( IoT& iot, char const* ssid, char const* password ) noexcept
            : ssid_( ssid ),
              password_( password )
    {
        iot.beginEvent += [this] { this->begin(); };
    }

    void WiFi::begin()
    {
        log( "connecting to WiFi at ", ssid_ );

        ::WiFi.mode( WIFI_STA );
        ::WiFi.begin( ssid_, password_ );
        while ( ::WiFi.status() != WL_CONNECTED ) {
            log( "waiting for WiFi..." );
            delay( 1000 );
        }
        ::WiFi.setAutoReconnect( true );

        log( "connection to WiFi established as ", ::WiFi.localIP());
    }

} // namespace iot