#include "IoT.hpp"
#include "Logger.hpp"

namespace iot {

    void IoT::begin()
    {
#if defined( LOGGER_SERIAL )
        Serial.begin( 115200 );
        while ( !Serial ) {}
#endif

        log( "starting ESP-IoT based application" );

        beginEvent();
    }

    void IoT::loop()
    {
        loopAlwaysEvent();

        uint32_t timestamp = millis();
        uint32_t elapsed = timestamp - lastLoop_;
        if ( elapsed > tick ) {
            lastLoop_ = timestamp;
            loopTickEvent();

            if ( !queue_.empty()) {
                queue_.front()();
                queue_.pop_front();
            }
        }
    }

    void IoT::enqueue( std::function< void() > action )
    {
        queue_.push_back( std::move( action ));
    }

} // namespace iot