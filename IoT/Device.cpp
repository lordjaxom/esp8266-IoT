#include <utility>

#include "Device.hpp"
#include "IoT.hpp"
#include "Logger.hpp"
#include "String.hpp"

using namespace std;

Device::Device( String name, String state, char const* falseValue, char const* trueValue,
                function< void( bool value ) > action ) noexcept
        : name_( move( name )),
          state_( move( state )),
          falseValue_( falseValue ),
          trueValue_( trueValue ),
          action_( move( action ))
{
    if ( action_ ) {
        IoT.subscribe( str( "cmnd/", name_, "/", state_ ), [this]( String message ) { set( message == trueValue_ ); } );
    }
}

Device::Device( String state, char const* falseValue, char const* trueValue, function< void ( bool value ) > action ) noexcept
        : Device( IoT.topic(), move( state ), falseValue, trueValue, move( action ))
{
}

Device::Device( String name, String state, std::function< void( bool value ) > action ) noexcept
        : Device( move( name ), move( state ), "OFF", "ON", move( action ))
{
}

Device::Device( String state, std::function< void( bool value ) > action ) noexcept
        : Device( IoT.topic(), move( state ), "OFF", "ON", move( action ))
{
}

Device::Device( function< void( bool ) > action ) noexcept
        : Device( "POWER", move( action ))
{
}

void Device::set( bool value )
{
    value_ = value;
    update();
}

void Device::update()
{
    if ( action_ ) {
        log( "setting device ", name_, " to ", value_ ? trueValue_ : falseValue_ );
        action_( value_ );
    }

    IoT.publish( str( "stat/", name_, "/", state_ ), value_ ? trueValue_ : falseValue_ );
}
