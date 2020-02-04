#ifndef ESP8266_IOT_REMOTE_HPP
#define ESP8266_IOT_REMOTE_HPP

class Remote
{
public:
    Remote( char const* topic, char const* stateName ) noexcept;
    explicit Remote( char const* topic ) noexcept;

    String const& name() const { return cmndTopic_; } // something arbitrarily unique

    bool get() const { return value_; }

    void set( bool value );

    void toggle() { set( !value_ ); }

private:
    String cmndTopic_;
    bool value_ {};
};

#endif // ESP8266_IOT_REMOTE_HPP
