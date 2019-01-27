#ifndef ESP8266_IOT_EVENT_HPP
#define ESP8266_IOT_EVENT_HPP

#include <functional>
#include <vector>


template< typename Signature >
class Event;

template< typename ...Args >
class Event< void( Args... ) > final
{
public:
    using Signature = void( Args... );
    using Handler = std::function< Signature >;

    Event() noexcept {}

    Event( Event const& ) = delete;

    Event& operator+=( Handler handler )
    {
        handlers_.push_back( std::move( handler ));
        return *this;
    }

    template< typename ...T >
    void operator()( T&& ... args )
    {
        for ( auto const& handler : handlers_ ) {
            handler( std::forward< T >( args )... );
        }
    }

private:
    std::vector< Handler > handlers_;
};


#endif // ESP8266_IOT_EVENT_HPP
