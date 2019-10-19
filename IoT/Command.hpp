#ifndef ESP8266_IOT_COMMAND_HPP
#define ESP8266_IOT_COMMAND_HPP

#include <functional>

class Command
{
public:
    /**
     * A command publishes a topic when triggered from this Thing.
     * A command triggers an action (if given) when triggered from outside this Thing.
     *
     * Publishes to cmnd/<name>/<commandName>
     * Subscribes to cmnd/<name>/<commandName> if action is given
     *
     * @param name
     * @param commandName
     */
    Command( String name, char const* commandName, std::function< void() > action = nullptr );

    Command( Command const& ) = delete;

    void trigger();

private:
    String topic() const;

    void trigger( String const& message );

    String name_;
    char const* commandName_;
    std::function< void() > action_;
};

#endif // ESP8266_IOT_COMMAND_HPP
