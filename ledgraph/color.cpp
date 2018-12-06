#include "color.hpp"

Color Color::scale( double brightness ) const
{
    return {
            (std::uint8_t) ( r() * brightness ),
            (std::uint8_t) ( g() * brightness ),
            (std::uint8_t) ( b() * brightness )
    };
}
