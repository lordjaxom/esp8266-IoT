#include <utility>

#include "Command.hpp"
#include "IoT.hpp"
#include "Logger.hpp"
#include "Json.hpp"
#include "String.hpp"

using namespace std;

Command::Command( String name, char const* commandName, function< void() > action )
        : name_( move( name )),
          commandName_( commandName ),
          action_( move( action ))
{
    if ( action_ ) {
        IoT.subscribe( topic(), [this]( String message ) { trigger( message ); } );
    }
}

void Command::trigger()
{
    StaticJsonDocument< 128 > json;
    json["source"] = IoTClass::clientId;
    IoT.publish( str( "cmnd/", name_, "/", commandName_ ), str( json ));
}

String Command::topic() const
{
    return str( "cmnd/", name_, "/", commandName_ );
}

void Command::trigger( String const& message )
{
    StaticJsonDocument< 128 > json;
    if ( deserializeJson( json, message )) {
        return;
    }
    if ( json["source"] == IoTClass::clientId ) {
        return;
    }

    action_();
}


