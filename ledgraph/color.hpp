#ifndef OUTLET_COLOR_HPP
#define OUTLET_COLOR_HPP

#include <cstdint>

class Color
{
public:
    constexpr Color()
            : r_ {},
              g_ {},
              b_ {}
    {
    }

    constexpr Color( std::uint8_t r, std::uint8_t g, std::uint8_t b )
            : r_ { r },
              g_ { g },
              b_ { b }
    {
    }

    constexpr Color( std::uint32_t value )
            : Color { (std::uint8_t) ( ( value >> 16 ) & 0xff ),
                      (std::uint8_t) ( ( value >>  8 ) & 0xff ),
                      (std::uint8_t) ( ( value >>  0 ) & 0xff ) }
    {
    }

    std::uint8_t r() const { return r_; }
    std::uint8_t g() const { return g_; }
    std::uint8_t b() const { return b_; }

    void r( std::uint8_t value ) { r_ = value; }
    void g( std::uint8_t value ) { g_ = value; }
    void b( std::uint8_t value ) { b_ = value; }

    Color scale( double brightness ) const;

private:
    std::uint8_t r_, g_, b_;
};

#endif //OUTLET_COLOR_HPP
