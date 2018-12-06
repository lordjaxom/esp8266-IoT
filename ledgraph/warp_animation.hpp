#ifndef OUTLET_WARP_ANIMATION_HPP
#define OUTLET_WARP_ANIMATION_HPP

#include <cstdint>
#include <memory>

#include <NeoPixelBus.h>

#include "colorwheel.hpp"
#include "ledbuffer.hpp"
#include "sinetable.hpp"
#include "gammatable.hpp"

template<
        unsigned MinBright,
        unsigned MaxBright,
        unsigned ColorRange,
        unsigned ColorSpeed,
        unsigned PulseRange,
        unsigned PulseSpeed,
        typename Buffer
        >
class WarpAnimation
{
    static constexpr double minBright = MinBright / 100.0;
    static constexpr double maxBright = MaxBright / 100.0;
    static constexpr double colorRange = ColorRange / 100.0;
    static constexpr double colorSpeed = ColorSpeed / 400.0;
    static constexpr double pulseRange = PulseRange / 100.0;
    static constexpr double pulseSpeed = PulseSpeed / 400.0;
    static constexpr double brightnessFactor = maxBright - minBright;
    static constexpr double colorSpeedFactor = 1.0 * colorSpeed / ColorWheel256::size();
    static constexpr double pulseSpeedFactor = 1.0 * pulseSpeed / sinetable_size;

    static void cycleUp( double& value, double factor = 1.0 )
    {
        value += 1.0 * factor;
        while ( value >= 1.0 ) {
            value -= 1.0;
        }
    }

    static void cycleDown( double& value, double factor = 1.0 )
    {
        value -= 1.0 * factor;
        while ( value < 0.0 ) {
            value += 1.0;
        }
    }

public:
    WarpAnimation( Buffer& buffer )
            : buffer_( buffer ),
              colorRangeFactor_( 1.0 * colorRange / buffer_.size() ),
              pulseRangeFactor_( 1.0 * pulseRange / buffer_.size() )
    {}

	void animate()
    {
        double brightnessIndex = brightnessOffset_;
        double colorIndex = colorOffset_;
        for ( std::size_t i = 0; i < buffer_.size(); ++i ) {
            double brightness =
                    minBright + ( sinetable_get((std::uint8_t) ( brightnessIndex * 256.0 )) / 255.0 ) * brightnessFactor;
            cycleUp( brightnessIndex, pulseRangeFactor_ );

            buffer_.set( i, GammaTable4::apply( ColorWheel256::get((uint8_t) ( colorIndex * 256.0 )).scale( brightness )));
            cycleUp( colorIndex, colorRangeFactor_ );
        }
        cycleDown( brightnessOffset_, pulseSpeedFactor );
        cycleUp( colorOffset_, colorSpeedFactor );
    }

private:
    Buffer& buffer_;
    double colorRangeFactor_;
    double pulseRangeFactor_;
	double colorOffset_ {};
	double brightnessOffset_ {};
};

template<
        unsigned MinBright = 20,
        unsigned MaxBright = 80,
        unsigned ColorRange = 100,
        unsigned ColorSpeed = 100,
        unsigned PulseRange = 100,
        unsigned PulseSpeed = 100,
        typename Buffer
        >
WarpAnimation< MinBright, MaxBright, ColorRange, ColorSpeed, PulseRange, PulseSpeed, Buffer > warpAnimation( Buffer& buffer )
{
    return WarpAnimation< MinBright, MaxBright, ColorRange, ColorSpeed, PulseRange, PulseSpeed, Buffer >( buffer );
}

#endif // OUTLET_WARP_ANIMATION_HPP