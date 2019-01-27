#include <utility>

#include "Input.hpp"
#include "IoT.hpp"

using namespace std;

Input::Input( function< bool() > input ) noexcept
        : input_( move( input ))
{
    IoT.loopTickEvent += [this] { this->loop(); };
}

void Input::loop()
{
    bool value = input_();
    if ( value_ != value ) {
        value_ = value;
        changeEvent( value_ );
    }
}


