#include <utility>

#include "IoT.hpp"
#include "Remote.hpp"
#include "String.hpp"

using namespace std;

namespace iot {

    Remote::Remote( IoT& iot, String name )
            : iot_( iot ),
              name_( move( name ))
    {
        iot_.subscribe( str( "stat/", name_, "/POWER" ), [this]( String message ) { value_ = message == "ON"; } );
    }

    void Remote::set( bool value )
    {
        iot_.publish( str( "cmnd/", name_, "/POWER" ), value ? "ON" : "OFF" );
    }

} // namespace iot
