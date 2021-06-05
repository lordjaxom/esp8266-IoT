#include <utility>

#include "Command.hpp"
#include "IoT.hpp"
#include "Json.hpp"
#include "String.hpp"

using namespace std;

Command::Command( String name, String command, function< void() > action )
        : name_( move( name )),
          command_( move( command )),
          action_( move( action ))
{
    if ( action_ ) {
        IoT.subscribe( topic(), [this]( String message ) { trigger( message ); } );
    }
}

Command::Command( String command, function< void() > action )
        : Command( IoT.topic(), move( command ), move( action ))
{
}

void Command::trigger()
{
    StaticJsonDocument< 128 > json;
    json["source"] = IoT.clientId();
    IoT.publish( topic(), str( json ));
}

String Command::topic() const
{
    return str( "cmnd/", name_, "/", command_ );
}

void Command::trigger( String const& message )
{
    StaticJsonDocument< 128 > json;
    if ( !deserializeJson( json, message ) &&
            json["source"] == IoT.clientId()) {
        return;
    }

    action_();
}
