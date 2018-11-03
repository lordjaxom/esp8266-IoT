#include "IoT.hpp"
#include "Logger.hpp"
#include "PushButton.hpp"

namespace iot {

    PushButton::PushButton( IoT& iot, std::function< bool() > input ) noexcept
            : input_( std::move( input )),
              timer_( iot )
    {
        iot.loopTickEvent += [this] { this->loop(); };
        timer_.expiredEvent += [this] { this->expired(); };
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