#include <Wire.h>

#include "Wire.hpp"
#include "IoT.hpp"

namespace iot {

    Wire::Wire( IoT& iot, int sda, int scl ) noexcept
            : iot( iot ),
              sda_( sda ),
              scl_( scl )
    {
        iot.beginEvent += [this] { this->begin(); };
    }

    void Wire::begin()
    {
        ::Wire.begin( sda_, scl_ );
        ::Wire.setClock( 10000 );
        ::Wire.setClockStretchLimit( 1500 );
    }

} // namespace iot


