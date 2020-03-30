#include <utility>

#include "IoT.hpp"
#include "Logger.hpp"
#include "PushButton.hpp"

using namespace std;

PushButton::PushButton( function< bool() > input ) noexcept
        : input_( move( input )),
          timer_( [this] { expired(); } )
{
    IoT.loopEvent += [this] { loop(); };
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
            timer_.start( 200 );
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
            longClickedEvent();
        }
    } else {
        clickedEvent( clicks_ );
    }
    clicks_ = 0;
}
