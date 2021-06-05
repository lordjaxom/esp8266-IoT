#ifndef ESP8266_IOT_LEDANIM_STAIRWAY_HPP
#define ESP8266_IOT_LEDANIM_STAIRWAY_HPP

#include <cstddef>

#include "../ledgraph/gammatable.hpp"

class StairwayAnimation
{
    static constexpr size_t totalFadeTime = 5360 / timePerFrame;
    static constexpr size_t segmentFadeTime = 1000 / timePerFrame;
    static constexpr size_t offsetPerSegment = ( totalFadeTime - segmentFadeTime ) / ( ledSegmentCount - 1 );

public:
    static constexpr bool FORWARD = true;
    static constexpr bool REVERSE = false;

    explicit StairwayAnimation( bool direction = FORWARD ) :
            direction_( direction ) {}

    bool animate()
    {
        if ( elapsed_ == totalFadeTime ) {
            return true;
        }

        size_t offset = 0;
        auto i = (uint16_t) ( direction_ ? 0 : totalLedCount - 1 );
        for ( uint16_t seg = 0; seg < ledSegmentCount; ++seg, offset += offsetPerSegment ) {
            size_t level = elapsed_ > offset
                           ? min(( elapsed_ - offset ) * 255 / segmentFadeTime, 255u )
                           : 0;

            for ( uint16_t led = 0; led < ledsPerSegment; ++led, i += direction_ ? 1 : -1 ) {
                neoPixelBus.SetPixelColor( i, RgbColor( GammaTable4::apply((uint8_t) level )));
            }
        }
        return ++elapsed_ == totalFadeTime;
    }

private:
    bool direction_;
    size_t elapsed_ = 0;
};

#endif // ESP8266_IOT_LEDANIM_STAIRWAY_HPP
