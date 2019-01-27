#include <Wire.h>

#include "IoT.hpp"
#include "Logger.hpp"
#include "Pcf8574.hpp"
#include "WireConfig.hpp"


Pcf8574::Pcf8574( WireConfig& wire, uint8_t address, uint8_t outputMask ) noexcept
        : address_( address ),
          outputMask_( outputMask ),
          output_( ~outputMask )
{
    IoT::get().loopTickEvent += [this] { this->loop(); };
}

void Pcf8574::loop()
{
    if ( dirty_ ) {
        log( "updating channel values for PCF8574 at address ", static_cast< int >( address_ ), " to ", static_cast< int >( output_ ));

        Wire.beginTransmission( address_ );
        Wire.write( output_ );
        if ( Wire.endTransmission() == 0 ) {
            dirty_ = false;
        }
    }

    if ( Wire.requestFrom( address_, static_cast< uint8_t >( 1 ))) {
        input_ = static_cast< uint8_t >( ~Wire.read());
    }
}

void Pcf8574::set( uint8_t index, bool value )
{
    output_ = static_cast< uint8_t >( value ? output_ | ( 1 << index ) : output_ & ~( 1 << index ));
    dirty_ = true;
}


