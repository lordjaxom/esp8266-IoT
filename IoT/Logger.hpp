#ifndef ESP8266_IOT_LOGGER_HPP
#define ESP8266_IOT_LOGGER_HPP

#include <stdint.h>
#include <stdio.h>
#include <type_traits>
#include <utility>

#include <Arduino.h>

namespace detail {

    inline void logTimestamp()
    {
#if defined( LOGGER_SERIAL )
        auto timestamp = millis();
        auto ms = timestamp % 1000;
        timestamp /= 1000;
        auto s = timestamp % 60;
        timestamp /= 60;
        auto m = timestamp % 60;
        timestamp /= 60;
        auto h = timestamp % 100;

        char buffer[16];
        sprintf( buffer, "%02lu:%02lu:%02lu.%03lu ", h, m, s, ms );
        Serial.print( buffer );
#endif
    }

    template< typename T >
    auto logAppend( T&& value )
    -> typename std::enable_if< std::is_same< decltype( Serial.print( std::forward< T >( value ))), size_t >::value >::type
    {
#if defined( LOGGER_SERIAL )
        Serial.print( std::forward< T >( value ));
#endif
    }

    template< typename T >
    auto logAppend( T&& value )
    -> typename std::enable_if< std::is_same< decltype( Serial.print( toString( std::forward< T >( value )))), size_t >::value >::type
    {
#if defined( LOGGER_SERIAL )
        Serial.print( toString( std::forward< T >( value )));
#endif
    }

    inline void log()
    {
#if defined( LOGGER_SERIAL )
        Serial.println();
#endif
    }

    template< typename Arg0, typename ...Args >
    void log( Arg0&& arg0, Args&& ... args )
    {
        logAppend( std::forward< Arg0 >( arg0 ));
        log( std::forward< Args >( args )... );
    }

} // namespace detail

template< typename ...Args >
void log( Args&& ... args )
{
    detail::logTimestamp();
    detail::log( std::forward< Args >( args )... );
}

#endif //ESP8266_IOT_LOGGER_HPP
