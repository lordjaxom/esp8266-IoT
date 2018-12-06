#include <utility>

#include "Logger.hpp"
#include "Device.hpp"
#include "MQTT.hpp"
#include "String.hpp"

namespace iot {

    Device::Device( IoT& iot, char const* name, char const* stateName, char const* falseValue, char const* trueValue,
                    std::function< void( bool value ) > action ) noexcept
            : name_( name ),
              stateName_( stateName ),
              falseValue_( falseValue ),
              trueValue_( trueValue ),
              action_( std::move( action ))
    {
        MQTT::subscribe( str( "cmnd/", name_, "/", stateName_ ), [this]( String message ) { this->set( message == trueValue_ ); } );
    }

    Device::Device( IoT& iot, char const* name, char const* stateName, char const* falseValue, char const* trueValue ) noexcept
            : Device( iot, name, stateName, falseValue, trueValue, []( bool ) {} ) {}

    Device::Device( IoT& iot, char const* name, std::function< void( bool value ) > action ) noexcept
            : Device( iot, name, "POWER", "OFF", "ON", std::move( action )) {}

    void Device::set( bool value )
    {
        log( "setting device ", name_, " to ", value ? trueValue_ : falseValue_ );

        action_( value );

        MQTT::publish( str( "stat/", name_, "/", stateName_ ), value ? trueValue_ : falseValue_ );
    }

} // namespace iot