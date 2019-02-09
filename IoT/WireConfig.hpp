#ifndef ESP8266_IOT_WIRECONFIG_HPP
#define ESP8266_IOT_WIRECONFIG_HPP

class WireConfig
{
public:
    WireConfig( int sda, int scl ) noexcept;
    WireConfig( WireConfig const& ) = delete;

private:
    void begin();

    int sda_;
    int scl_;
};

#endif // ESP8266_IOT_WIRECONFIG_HPP
