#ifndef ESP8266_IOT_LOGGER_HPP
#define ESP8266_IOT_LOGGER_HPP

#include <stdint.h>
#include <stdio.h>
#include <type_traits>
#include <utility>

#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>

namespace detail {

//    inline String timestamp()
//    {
//        auto timestamp = millis();
//        auto ms = timestamp % 1000;
//        timestamp /= 1000;
//        auto s = timestamp % 60;
//        timestamp /= 60;
//        auto m = timestamp % 60;
//        timestamp /= 60;
//        auto h = timestamp % 100;
//
//        char buffer[16];
//        sprintf( buffer, "%02lu:%02lu:%02lu.%03lu ", h, m, s, ms );
//        return buffer;
//    }

    class Timestamp
    {
    public:
        Timestamp()
                : timestamp( millis()),
                  ms( timestamp % 1000 ),
                  s( ( timestamp /= 1000, timestamp % 60 )),
                  m( ( timestamp /= 60, timestamp % 60 )),
                  h( ( timestamp /= 60, timestamp % 100 ))
        {
        }

    private:
        unsigned long timestamp;

    public:
        unsigned long ms;
        unsigned long s;
        unsigned long m;
        unsigned long h;
    };

} // namespace detail
/*
class LoggerSocket
{
public:
    void begin();

    bool ready() { return client_.connected(); }

    void log()
    {
        client_.write( "\n" );
    }

    template< typename Arg0, typename ...Args >
    void log( Arg0&& arg0, Args&&... args )
    {
        append( std::forward< Arg0 >( arg0 ));
        log( std::forward< Args >( args )... );
    }

private:
    void connectToLogstash();
    void disconnectFromLogstash();

    template< typename Arg >
    auto append( Arg&& value )
    -> typename std::enable_if< std::is_same< decltype( String( std::forward< Arg >( value ))), String >::value >::type
    {
        String s( std::forward< Arg >( value ));
        client_.write( s.c_str(), s.length());
    }

    template< typename Arg >
    auto append( Arg&& value )
    -> typename std::enable_if< std::is_same< decltype( String( toString( std::forward< Arg >( value )))), String >::value >::type
    {
        String s = toString( std::forward< Arg >( value ));
        client_.write( s.c_str(), s.length());
    }

    AsyncClient client_;
};

class LoggerNoop
{
public:
    void begin() {}
    constexpr bool ready() const { return false; }

    template< typename ...Args >
    void log( Args&&... args ) {}
};
*/

#if defined( LOGGER_SERIAL )
#   include "LoggerSerial.hpp"
using LoggerClass = LoggerSerial;
#else
#   include "LoggerNoop.hpp"
using LoggerClass = LoggerNoop;
#endif

extern LoggerClass Logger;

template< typename ...Args >
void log( Args&&... args )
{
    Logger.log( detail::Timestamp(), std::forward< Args >( args )... );
}

#endif //ESP8266_IOT_LOGGER_HPP
