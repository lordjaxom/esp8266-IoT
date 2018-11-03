#include <utility>

#include "Logger.hpp"
#include "Device.hpp"
#include "MQTT.hpp"
#include "String.hpp"

namespace iot {

    Device::Device( IoT& iot, char const* name, std::function< void( bool value ) > action ) noexcept
            : name_( name ),
              action_( std::move( action ))
    {
        MQTT::subscribe( str( "cmnd/", name_, "/POWER" ), [this]( String message ) { this->set( message == "ON" ); } );
    }

    void Device::set( bool value )
    {
        log( "setting device ", name_, " to ", value ? "ON" : "OFF" );

        action_( value );

        MQTT::publish( str( "stat/", name_, "/POWER" ), value ? "ON" : "OFF" );
    }

} // namespace iot