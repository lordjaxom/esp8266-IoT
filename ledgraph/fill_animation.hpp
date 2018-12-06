#ifndef OUTLET_FILL_ANIMATION_HPP
#define OUTLET_FILL_ANIMATION_HPP

#include <memory>

#include "color.hpp"
#include "ledbuffer.hpp"

template< typename Buffer >
class FillAnimation
{
public:
    FillAnimation( Buffer& buffer, Color color )
            : buffer_( buffer ),
              color_( color )
    {}

    void animate()
    {
        buffer_.fill( color_ );
    }

private:
    Buffer& buffer_;
    Color color_;
};

template< typename Buffer >
FillAnimation< Buffer > fillAnimation( Buffer& buffer, Color color )
{
    return FillAnimation< Buffer >( buffer, color );
}

#endif //OUTLET_FILL_ANIMATION_HPP
