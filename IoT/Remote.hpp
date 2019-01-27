#ifndef ESP8266_IOT_REMOTE_HPP
#define ESP8266_IOT_REMOTE_HPP


class Remote
{
public:
    explicit Remote( String name ) noexcept;

    String const& name() const { return name_; }

    bool get() const { return value_; }

    void set( bool value );

    void toggle() { set( !value_ ); }

private:
    String name_;
    bool value_ {};
};


#endif // ESP8266_IOT_REMOTE_HPP
