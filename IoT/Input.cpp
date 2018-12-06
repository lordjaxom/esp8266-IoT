#include <utility>

#include "Input.hpp"
#include "IoT.hpp"

namespace iot {

    Input::Input( IoT& iot, std::function< bool() > input )
            : input_( std::move( input ))
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