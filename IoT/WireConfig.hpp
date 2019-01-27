#ifndef ESP8266_IOT_WIRECONFIG_HPP
#define ESP8266_IOT_WIRECONFIG_HPP

namespace iot {

    class IoT;

    class WireConfig
    {
    public:
        WireConfig( IoT& iot, int sda, int scl ) noexcept;
        WireConfig( WireConfig const& ) = delete;

        IoT& iot;

    private:
        void begin();

        int sda_;
        int scl_;
    };

} // namespace iot
#endif // ESP8266_IOT_WIRECONFIG_HPP
