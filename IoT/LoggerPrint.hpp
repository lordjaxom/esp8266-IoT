#ifndef ESP8266_IOT_LOGGERPRINT_HPP
#define ESP8266_IOT_LOGGERPRINT_HPP

#include <type_traits>

#include <Print.h>

#include "LoggerBase.hpp"

template< typename Print, Print& print >
class LoggerPrint
        : public LoggerBase< LoggerPrint< Print, print > >
{
public:
    template< typename Arg >
    auto append( Arg&& arg )
    -> typename std::enable_if< std::is_same< decltype( print.print( std::forward< Arg >( arg ))), size_t >::value >::type
    {
        print.print( std::forward< Arg >( arg ));
    }

    template< typename Arg >
    auto append( Arg&& arg )
    -> typename std::enable_if< std::is_same< decltype( this->append( toString( std::forward< Arg >( arg )))), void >::value >::type
    {
        append( toString( std::forward< Arg >( arg )));
    }

    void append( detail::Timestamp const& ts )
    {
        print.printf( "%02lu:%02lu:%02lu.%03lu ", ts.h, ts.m, ts.s, ts.ms );
    }

    void flush()
    {
        print.println();
    }
};

#endif //ESP8266_IOT_LOGGERPRINT_HPP
