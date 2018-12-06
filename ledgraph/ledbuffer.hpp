#ifndef OUTLET_LEDBUFFER_HPP
#define OUTLET_LEDBUFFER_HPP

#include <cstddef>
#include <cstdint>

#include <NeoPixelBus.h>

#include "color.hpp"

/**
 * class LedBuffer
 */

template< typename Feature, typename Method >
class NeoPixelBuffer
{
public:
    NeoPixelBuffer( NeoPixelBus< Feature, Method >& bus )
            : bus_( bus )
    {
    }

    NeoPixelBuffer( NeoPixelBuffer const& ) = delete;
    ~NeoPixelBuffer() = default;

    std::size_t size() const { return bus_.PixelCount(); }

    void fill( Color const& color )
    {
        bus_.ClearTo( RgbColor( color.r(), color.g(), color.b()));
    }

    void set( std::size_t index, Color color )
    {
        bus_.SetPixelColor((std::uint16_t) index, RgbColor( color.r(), color.g(), color.b()));
    }

protected:
    Color get( std::size_t index ) const
    {
        auto color = bus_.GetPixelColor((std::uint16_t) index );
        return { color.R, color.G, color.B };
    }

private:
    NeoPixelBus< Feature, Method >& bus_;
};

#endif //OUTLET_LEDBUFFER_HPP
