#ifndef ESP8266_IOT_EVENT_HPP
#define ESP8266_IOT_EVENT_HPP

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <utility>

using Subscription = std::unique_ptr< void, std::function< void ( void const* ) > >;

template< typename Signature >
class Event;

template< typename ...Args >
class Event< void( Args... ) > final
{
    using Signature = void( Args... );
    using Handler = std::function< Signature >;
    using Container = std::list< std::pair< Handler, bool > >;
    using Iterator = typename Container::iterator;

public:
    Event() noexcept {}

    Event( Event const& ) = delete;

    size_t priorities() const { return priorities_; }

    Event& operator+=( Handler handler )
    {
        handlers_.emplace_back( std::move( handler ), false );
        return *this;
    }

    Subscription subscribe( Handler handler, bool priority = false )
    {
        auto it = handlers_.emplace( handlers_.end(), std::move( handler ), false );
        if ( priority ) {
            ++priorities_;
        }
        return { this, [=]( void const* ) { unsubscribe( it, priority ); } };
    }

    template< typename ...T >
    void operator()( T&& ... args )
    {
        for ( auto it = handlers_.begin() ; it != handlers_.end() ; ) {
            it->first( std::forward< T >( args )... );
            if ( it->second ) {
                it = handlers_.erase( it );
            } else {
                ++it;
            }
        }
    }

private:
    void unsubscribe( Iterator it, bool priority )
    {
        it->second = true;
        if ( priority ) {
            --priorities_;
        }
    }

    Container handlers_;
    size_t priorities_ {};
};

#endif // ESP8266_IOT_EVENT_HPP
