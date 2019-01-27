#include <Wire.h>

#include "WireConfig.hpp"
#include "IoT.hpp"

namespace iot {

    WireConfig::WireConfig( IoT& iot, int sda, int scl ) noexcept
            : iot( iot ),
              sda_( sda ),
              scl_( scl )
    {
        iot.beginEvent += [this] { this->begin(); };
    }

    void WireConfig::begin()
    {
        Wire.begin( sda_, scl_ );
        Wire.setClock( 10000 );
        Wire.setClockStretchLimit( 1500 );
    }

} // namespace iot


