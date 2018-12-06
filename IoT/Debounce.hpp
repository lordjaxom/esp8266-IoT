#ifndef ESP8266_IOT_DEBOUNCE_HPP
#define ESP8266_IOT_DEBOUNCE_HPP

#include <stdint.h>

namespace iot {

    template< typename T >
    class Debounce
    {
    public:
        explicit Debounce( std::function< T() > input )
                : input_( std::move( input )) {}

        T operator()()
        {
            T temp = value_ ^input_();
            states_[0] = ~( states_[0] & temp );
            states_[1] = states_[0] ^ ( states_[1] & temp );
            temp &= states_[0] & states_[1];
            value_ ^= temp;
            return value_;
        }

    private:
        std::function< T() > input_;
        T value_ {};
        T states_[2] {};
    };

    template< typename Input >
    Debounce< decltype( std::declval< Input >()()) > debounce( Input&& input )
    {
        return Debounce< decltype( std::declval< Input >()()) >( std::forward< Input >( input ));
    }

} // namespace iot

#endif // ESP8266_IOT_DEBOUNCE_HPP
