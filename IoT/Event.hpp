#ifndef ESP8266_IOT_EVENT_HPP
#define ESP8266_IOT_EVENT_HPP

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <utility>

using Subscription = std::unique_ptr< bool, void (*)( bool* deleted ) >;

template< typename Signature >
class Event;

template< typename ...Args >
class Event< void ( Args... ) > final
{
    using Signature = void ( Args... );
    using Handler = std::function< Signature >;

public:
    Event() noexcept {}

    Event( Event const& ) = delete;

    Event& operator+=( Handler handler )
    {
        handlers_.emplace_back( std::move( handler ), false );
        return *this;
    }

    Subscription subscribe( Handler handler )
    {
        auto it = handlers_.emplace( handlers_.end(), std::move( handler ), false );
        return { &it->second, []( bool* deleted ) { *deleted = true; } };
    }

    template< typename ...T >
    void operator()( T&&... args )
    {
        // elements added inside handlers should not be invoked
        auto it = handlers_.begin();
        auto size = handlers_.size();
        for ( size_t i = 0 ; i < size ; ++i ) {
            // deleted flag might change during invocation
            if ( !it->second ) {
                it->first( std::forward< T >( args )... );
            }
            if ( it->second ) {
                it = handlers_.erase( it );
            } else {
                ++it;
            }
        }
    }

private:
    std::list< std::pair< Handler, bool > > handlers_;
};

#endif // ESP8266_IOT_EVENT_HPP
