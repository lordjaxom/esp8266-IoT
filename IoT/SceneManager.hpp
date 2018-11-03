#ifndef ESP8266_IOT_SCENEMANAGER_HPP
#define ESP8266_IOT_SCENEMANAGER_HPP

#include <map>

#include "Event.hpp"

namespace iot {

    class IoT;

    enum class Scene : uint8_t
    {
        OFF,
        SCENE1,
        SCENE2,
        SLEEP = 64,
        UNKNOWN = 0xff
    };

    class SceneManager
    {
    public:
        SceneManager( IoT& iot, char const* zone );

        template< typename Device >
        void addSceneDevice( Device& device )
        {
            sceneEvents_[Scene::SLEEP] += [&device] { device.set( false ); };
            sceneEvents_[Scene::OFF] += [&device] { device.set( false ); };
            sceneEvents_[Scene::SCENE1] += [&device] { device.set( true ); };
            sceneEvents_[Scene::SCENE2] += [&device] { device.set( false ); };
        }

        void sceneButtonClicked( unsigned clicked );
        void sceneCommand( String const& sceneName );

    private:
        void changeScene( Scene scene, bool publish = true );

        IoT& iot_;
        char const* zone_;
        Scene scene_ {};
        std::map< Scene, Event < void() > > sceneEvents_;
    };

} // namespace iot

#endif // ESP8266_IOT_SCENEMANAGER_HPP
