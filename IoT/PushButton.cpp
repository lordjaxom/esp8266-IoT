#include <utility>

#include "IoT.hpp"
#include "Logger.hpp"
#include "PushButton.hpp"

using namespace std;

namespace iot {

    PushButton::PushButton( IoT& iot, function< bool() > input ) noexcept
            : input_( move( input )),
              timer_( iot, [this] { expired(); } )
    {
        iot.loopTickEvent += [this] { this->loop(); };
    }

    void PushButton::loop()
    {
        bool value = input_();
        if ( value_ != value ) {
            value_ = value;

            if ( value_ ) {
                timer_.start( 1000 );
            } else if ( !finished_ ) {
                ++clicks_;
                timer_.start( 500 );
            } else {
                finished_ = false;
            }
        }
    }

    void PushButton::expired()
    {
        if ( value_ ) {
            finished_ = true;
            if ( clicks_ == 0 ) {
                clickedEvent( 0 );
            }
        } else {
            clickedEvent( clicks_ );
        }
        clicks_ = 0;
    }

} // namespace iot