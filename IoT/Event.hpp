#ifndef ESP8266_IOT_EVENT_HPP
#define ESP8266_IOT_EVENT_HPP

#include <functional>
#include <list>
#include <utility>

template< typename Signature >
class Subscription;

template< typename Signature >
class Event;

template< typename ...Args >
class Event< void( Args... ) > final
{
public:
    using Signature = void( Args... );
    using Handler = std::function< Signature >;
    using Subscription = ::Subscription< Signature >;
    using Container = std::list< Handler >;

    Event() noexcept {}

    Event( Event const& ) = delete;

    Event& operator+=( Handler handler )
    {
        handlers_.push_back( std::move( handler ));
        return *this;
    }

    Subscription subscribe( Handler handler )
    {
        auto it = handlers_.insert( handlers_.end(), std::move( handler ));
        return { handlers_, it };
    }

    template< typename ...T >
    void operator()( T&& ... args )
    {
        for ( auto const& handler : handlers_ ) {
            handler( std::forward< T >( args )... );
        }
    }

private:
    Container handlers_;
};

template< typename Signature >
class Subscription
{
public:
    using Event = ::Event< Signature >;
    using Container = typename Event::Container;
    using Iterator = typename Container::iterator;

    Subscription( Container& container, Iterator it )
            : container_( &container ),
              it_( std::move( it )) {}

    Subscription( Subscription const& ) = delete;

    Subscription( Subscription&& other ) noexcept
            : container_( other.container_ ),
              it_( std::move( other.it_ ))
    {
        other.it_ = container_->end();
    }

    ~Subscription()
    {
        if ( it_ != container_->end()) {
            container_->erase( it_ );
        }
    }

private:
    Container* container_;
    Iterator it_;
};


#endif // ESP8266_IOT_EVENT_HPP
