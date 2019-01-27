#include <Wire.h>

#include "WireConfig.hpp"
#include "IoT.hpp"


WireConfig::WireConfig( int sda, int scl ) noexcept
        : sda_( sda ),
          scl_( scl )
{
    IoT.beginEvent += [this] { this->begin(); };
}

void WireConfig::begin()
{
    Wire.begin( sda_, scl_ );
    Wire.setClock( 10000 );
    Wire.setClockStretchLimit( 1500 );
}




