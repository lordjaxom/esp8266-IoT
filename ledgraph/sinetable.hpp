#ifndef OUTLET_SINETABLE_HPP
#define OUTLET_SINETABLE_HPP

#include "valtable.hpp"

static uint8_t const sinetable[] = {
	130u,133u,136u,139u,142u,145u,148u,151u,
	154u,157u,160u,163u,166u,169u,172u,175u,
	178u,181u,184u,186u,189u,192u,195u,197u,
	200u,202u,205u,207u,210u,212u,214u,217u,
	219u,221u,223u,225u,227u,229u,231u,233u,
	234u,236u,238u,239u,241u,242u,243u,245u,
	246u,247u,248u,249u,250u,251u,252u,252u,
	253u,253u,254u,254u,255u,255u,255u,255u
};

static constexpr std::size_t sinetable_size = sizeof( sinetable ) / sizeof( sinetable[ 0 ] );

inline uint8_t sinetable_get( uint8_t x )
{
	if ( x < 64 ) {
		return sinetable[ x ];
	} else if ( x < 128 ) {
		return sinetable[ 127 - x ];
	} else if ( x < 192 ) {
		return 255 - sinetable[ x - 128 ];
	} else {
		return 255 - sinetable[ 255 - x ];
	}
}

namespace detail {

	struct SineGenerator
	{
		static constexpr std::uint8_t apply( std::uint8_t input )
		{
			return std::sin( input / 255.0 );
		}
	};
}

struct SineTable
		: public ValueTable< detail::SineGenerator, 0, 63 >
{
};

#endif // OUTLET_SINETABLE_HPP