#include <utility>

#include "IoT.hpp"
#include "Remote.hpp"
#include "String.hpp"

using namespace std;

Remote::Remote( char const* topic, char const* stateName ) noexcept
        : cmndTopic_( str( "cmnd/", topic, "/", stateName ))
{
    IoT.subscribe( str( "stat/", topic, "/", stateName ), [this]( String message ) { value_ = message == "ON"; } );
}

Remote::Remote( char const* topic ) noexcept
        : Remote( topic, "POWER" )
{
}

void Remote::set( bool value )
{
    IoT.publish( cmndTopic_, value ? "ON" : "OFF" );
}
