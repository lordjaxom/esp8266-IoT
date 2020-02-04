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
     * Publishes to cmnd/<name>/<command>
     * Subscribes to cmnd/<name>/<command> if action is given
     *
     * @param name
     * @param command
     */
    Command( String name, String command, std::function< void() > action = nullptr );

    explicit Command( String command, std::function< void () > action = nullptr );

    Command( Command const& ) = delete;

    void trigger();

private:
    String topic() const;

    void trigger( String const& message );

    String name_;
    String command_;
    std::function< void() > action_;
};

#endif // ESP8266_IOT_COMMAND_HPP
