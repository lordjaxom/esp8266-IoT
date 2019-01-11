#include <utility>

#include "Input.hpp"
#include "IoT.hpp"

using namespace std;

namespace iot {

    Input::Input( IoT& iot, function< bool() > input ) noexcept
            : input_( move( input ))
    {
        iot.loopTickEvent += [this] { this->loop(); };
    }

    void Input::loop()
    {
        bool value = input_();
        if ( value_ != value ) {
            value_ = value;
            changeEvent( value_ );
        }
    }


} // namespace iot