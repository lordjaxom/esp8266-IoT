#ifndef ESP8266_IOT_STRING_HPP
#define ESP8266_IOT_STRING_HPP


namespace detail {

    inline void strAppend( String& result, char const* arg )
    {
        result += arg;
    }

    inline void strAppend( String& result, __FlashStringHelper const* arg )
    {
        result += arg;
    }

    inline void strAppend( String& result, String const& arg )
    {
        result += arg;
    }

    inline void str( String& result ) {}

    template< typename Arg0, typename ...Args >
    void str( String& result, Arg0&& arg0, Args&& ...args )
    {
        strAppend( result, std::forward< Arg0 >( arg0 ));
        str( result, std::forward< Args >( args )... );
    }

} // namespace detail

template< typename ...Args >
String str( Args&& ... args )
{
    String result;
    detail::str( result, std::forward< Args >( args )... );
    return result;
}

class StringView
{
public:
    StringView( char const* data, size_t length ) : data_( data ), length_( length ) {}

    StringView( char const* data ) : StringView( data, strlen( data )) {} // NOLINT(google-explicit-constructor)
    StringView( String const& string ) : StringView( string.c_str(), string.length()) {} // NOLINT(google-explicit-constructor)

    bool operator<( StringView const& other ) const
    {
        return std::lexicographical_compare( data_, data_ + length_, other.data_, other.data_ + other.length_ );
    }

private:
    char const* data_;
    size_t length_;
};


#endif //ESP8266_IOT_STRING_HPP
