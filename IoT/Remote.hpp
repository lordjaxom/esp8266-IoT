#ifndef ESP8266_IOT_REMOTE_HPP
#define ESP8266_IOT_REMOTE_HPP

namespace iot {

    class IoT;

    class Remote
    {
    public:
        Remote( IoT& iot, String name );

        String const& name() const { return name_; }

        bool get() const { return value_; }
        void set( bool value );
        void toggle() { set( !value_ ); }

    private:
        IoT& iot_;
        String name_;
        bool value_ {};
    };

} // namespace iot

#endif // ESP8266_IOT_REMOTE_HPP
