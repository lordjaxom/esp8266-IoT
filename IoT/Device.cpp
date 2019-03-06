#include <utility>

#include <utility>

#include "Device.hpp"
#include "IoT.hpp"
#include "Logger.hpp"
#include "String.hpp"

using namespace std;

Device::Device( String name, char const* stateName, char const* falseValue, char const* trueValue,
                function< void( bool value ) > action ) noexcept
        : name_( move( name )),
          stateName_( stateName ),
          falseValue_( falseValue ),
          trueValue_( trueValue ),
          action_( move( action ))
{
    if ( action_ ) {
        IoT.subscribe( str( "cmnd/", name_, "/", stateName_ ), [this]( String message ) { set( message == trueValue_ ); } );
    }
}

Device::Device( String name, std::function< void( bool value ) > action ) noexcept
        : Device( move( name ), "POWER", "OFF", "ON", move( action ))
{
}

void Device::set( bool value )
{
    if ( action_ ) {
        log( "setting device ", name_, " to ", value ? trueValue_ : falseValue_ );
        action_( value );
    }

    IoT.publish( str( "stat/", name_, "/", stateName_ ), value ? trueValue_ : falseValue_ );

    value_ = value;
}

