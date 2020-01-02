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

Device::Device( String name, char const* stateName, std::function< void( bool value ) > action ) noexcept
        : Device( move( name ), stateName, "OFF", "ON", move( action ))
{
}

Device::Device( String name, std::function< void( bool value ) > action ) noexcept
        : Device( move( name ), "POWER", "OFF", "ON", move( action ))
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

    IoT.publish( str( "stat/", name_, "/", stateName_ ), value_ ? trueValue_ : falseValue_ );
}
