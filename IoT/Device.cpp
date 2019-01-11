#include <utility>

#include "Device.hpp"
#include "IoT.hpp"
#include "Logger.hpp"
#include "String.hpp"

using namespace std;

namespace iot {

    Device::Device( IoT& iot, char const* name, char const* stateName, char const* falseValue, char const* trueValue,
                    function< void( bool value ) > action ) noexcept
            : iot_( iot ),
              name_( name ),
              stateName_( stateName ),
              falseValue_( falseValue ),
              trueValue_( trueValue ),
              action_( move( action ))
    {
        iot_.subscribe( str( "cmnd/", name_, "/", stateName_ ), [this]( String message ) { set( message == trueValue_ ); } );
    }

    Device::Device( IoT& iot, char const* name, char const* stateName, char const* falseValue, char const* trueValue ) noexcept
            : Device( iot, name, stateName, falseValue, trueValue, []( bool ) {} )
    {
    }

    Device::Device( IoT& iot, char const* name, std::function< void( bool value ) > action ) noexcept
            : Device( iot, name, "POWER", "OFF", "ON", std::move( action ))
    {
    }

    void Device::set( bool value )
    {
        log( "setting device ", name_, " to ", value ? trueValue_ : falseValue_ );

        action_( value );

        iot_.publish( str( "stat/", name_, "/", stateName_ ), value ? trueValue_ : falseValue_ );
    }

} // namespace iot