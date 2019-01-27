#include "Debounce.hpp"
#include "Gpio.hpp"
#include "IoT.hpp"

using namespace std;

namespace iot {

    GpioInput::GpioInput( IoT& iot, uint8_t pin, bool pullUp ) noexcept
            : pin_( pin ),
              pullUp_( pullUp )
    {
        iot.beginEvent += [this] { pinMode( pin_, static_cast< uint8_t >( pullUp_ ? INPUT_PULLUP : INPUT )); };
    }

    bool GpioInput::operator()() const
    {
        return digitalRead( pin_ ) == ( pullUp_ ? LOW : HIGH );
    }

    GpioOutput::GpioOutput( IoT& iot, uint8_t pin, bool invert ) noexcept
            : pin_( pin ),
              invert_( invert )
    {
        iot.beginEvent += [this] { pinMode( pin_, OUTPUT ); };
    }

    void GpioOutput::operator()( bool value )
    {
        digitalWrite( pin_, static_cast< uint8_t >( value == invert_ ? LOW : HIGH ));
    }

} // namespace iot
