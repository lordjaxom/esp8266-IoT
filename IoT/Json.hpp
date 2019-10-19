#ifndef ESP8266_IOT_JSON_HPP
#define ESP8266_IOT_JSON_HPP

#include <ArduinoJson.h>

namespace detail {

    inline void strAppend( String& result, JsonDocument const& arg )
    {
        serializeJson( arg, result );
    }

} // namespace detail

#endif // ESP8266_IOT_JSON_HPP
