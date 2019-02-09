#ifndef ESP8266_IOT_TIMER_HPP
#define ESP8266_IOT_TIMER_HPP

#include <functional>

class Timer
{
public:
    explicit Timer( std::function< void() > handler ) noexcept;
    Timer( Timer const& ) = delete;

    bool active() const { return timeout_ > 0; }

    void start( uint32_t timeout );
    void stop();

private:
    void loop();

    std::function< void() > handler_;
    uint32_t timeout_ {};
};

#endif //ESP8266_IOT_TIMER_HPP
