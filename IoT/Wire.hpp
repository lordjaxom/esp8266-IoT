#ifndef ESP8266_IOT_WIRE_HPP
#define ESP8266_IOT_WIRE_HPP

namespace iot {

    class IoT;

    class Wire
    {
    public:
        Wire( IoT& iot, int sda, int scl ) noexcept;

        IoT& iot;

    private:
        void begin();

        int sda_;
        int scl_;
    };

} // namespace iot
#endif // ESP8266_IOT_WIRE_HPP
