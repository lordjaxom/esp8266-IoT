#ifndef ESP8266_IOT_LEDANIM_FADEOUT_HPP
#define ESP8266_IOT_LEDANIM_FADEOUT_HPP

#include <cstddef>

#include "../ledgraph/gammatable.hpp"

class FadeoutAnimation
{
    static constexpr size_t fadeTime = 2000 / timePerFrame;

public:
    bool animate()
    {
        if ( elapsed_ == fadeTime ) {
            return true;
        }

        size_t level = 255 - ( elapsed_ * 255 / fadeTime );
        neoPixelBus.ClearTo( RgbColor( GammaTable4::apply( (uint8_t) level )));

        return ++elapsed_ == fadeTime;
    }

private:
    size_t elapsed_ = 0;
};

#endif // ESP8266_IOT_LEDANIM_FADEOUT_HPP
