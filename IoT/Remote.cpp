#include <utility>

#include "IoT.hpp"
#include "Remote.hpp"
#include "String.hpp"

using namespace std;

Remote::Remote( String name ) noexcept
        : name_( move( name ))
{
    IoT.subscribe( str( "stat/", name_, "/POWER" ), [this]( String message ) { value_ = message == "ON"; } );
}

void Remote::set( bool value )
{
    IoT.publish( str( "cmnd/", name_, "/POWER" ), value ? "ON" : "OFF" );
}
