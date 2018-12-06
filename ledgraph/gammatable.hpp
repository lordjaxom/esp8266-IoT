#ifndef OUTLET_GAMMATABLE_HPP
#define OUTLET_GAMMATABLE_HPP

#include <cmath>
#include <cstdint>
#include <limits>
#include <ratio>

#include "color.hpp"
#include "valtable.hpp"

namespace detail {

	template< typename Gamma >
	struct GammaGenerator
	{
		static constexpr std::uint8_t max = std::numeric_limits< uint8_t >::max();

		static constexpr std::uint8_t apply( std::uint8_t input )
		{
			return std::pow( (double) input / max, ( 1.0 * Gamma::den / Gamma::num ) ) * max;
		}
	};

} // namespace detail

template< typename Gamma >
struct BasicGammaTable
        : ValueTable< detail::GammaGenerator< Gamma > >
{
    using Base = ValueTable< detail::GammaGenerator< Gamma > >;

    static Color apply( Color color )
    {
        return { Base::get( color.r() ), Base::get( color.g() ), Base::get( color.b() ) };
    }
};

using GammaTable4 = BasicGammaTable< std::ratio< 4, 10 > >;

#endif // OUTLET_GAMMATABLE_HPP
