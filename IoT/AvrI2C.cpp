#include <Wire.h>

#include "AvrI2C.hpp"
#include "IoT.hpp"
#include "WireConfig.hpp"

namespace iot {

    AvrI2C::AvrI2C( WireConfig& wire, uint8_t address )
            : address_( address )
    {
        wire.iot.loopTickEvent += [this] { loop(); };
    }

    void AvrI2C::loop()
    {
        for ( size_t i = 0; i < 2; ++i ) {
            if ( dirty_[i] ) {
                uint8_t buffer[2] { static_cast< uint8_t >( i ), static_cast< uint8_t >( outputs_[i] ? 0xff : 0x00) };
                ::Wire.beginTransmission( address_ );
                ::Wire.write( buffer, 2 );
                if ( ::Wire.endTransmission() == 0 ) {
                    dirty_[i] = false;
                }
            }
        }

        ::Wire.requestFrom( address_, static_cast< uint8_t >( 1 ));
        if ( ::Wire.available()) {
            input_ = static_cast< bool >(::Wire.read());
        }
    }

} // namespace iot
