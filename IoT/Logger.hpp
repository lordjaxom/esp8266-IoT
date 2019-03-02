#ifndef ESP8266_IOT_LOGGER_HPP
#define ESP8266_IOT_LOGGER_HPP

#include <stdint.h>
#include <stdio.h>
#include <type_traits>
#include <utility>

namespace detail {

    inline String timestamp()
    {
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
        return buffer;
    }

} // namespace detail

class LoggerSerial
{
public:
    void begin()
    {
        Serial.begin( 115200 );
        while ( !Serial ) {}
    }

    void log()
    {
        Serial.println();
    }

    template< typename Arg0, typename ...Args >
    void log( Arg0&& arg0, Args&&... args )
    {
        append( std::forward< Arg0 >( arg0 ));
        log( std::forward< Args >( args )... );
    }

private:
    template< typename Arg >
    auto append( Arg&& value )
    -> typename std::enable_if< std::is_same< decltype( Serial.print( std::forward< Arg >( value ))), size_t >::value >::type
    {
        Serial.print( std::forward< Arg >( value ));
    }

    template< typename Arg >
    auto append( Arg&& value )
    -> typename std::enable_if< std::is_same< decltype( Serial.print( toString( std::forward< Arg >( value )))), size_t >::value >::type
    {
        Serial.print( toString( std::forward< Arg >( value )));
    }
};

class LoggerNoop
{
public:
    void begin()
    {
    }

    template< typename ...Args >
    void log( Args&&... args )
    {
    }
};

#if defined( LOGGER_SERIAL )
extern LoggerSerial Logger;
#else
extern LoggerNoop Logger;
#endif

template< typename ...Args >
void log( Args&&... args )
{
    Logger.log( detail::timestamp(), std::forward< Args >( args )... );
}

#endif //ESP8266_IOT_LOGGER_HPP
