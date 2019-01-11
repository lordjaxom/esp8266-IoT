#ifndef ESP8266_IOT_SCENEMANAGER_HPP
#define ESP8266_IOT_SCENEMANAGER_HPP

#include <map>
#include <vector>

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

    template< typename... Scene >
    struct Scenes
    {
    };

    template< typename... Scene >
    Scenes< Scene... > scenes( Scene... );

    class SceneManager
    {
    public:
        SceneManager( IoT& iot, char const* zone ) noexcept;
        SceneManager( SceneManager const& ) = delete;

        void addSceneEvent( Scene scene, std::function< void() > handler );

        template< typename Device, typename OnScenes, typename OffScenes = Scenes<> >
        void addSceneDeviceX( Device& device, OnScenes, OffScenes = OffScenes() )
        {

        }

        template< typename Device >
        void addSceneDevice( Device& device, std::vector< Scene > const& onScenes, std::vector< Scene > const& offScenes )
        {
            for ( auto scene : onScenes ) {
                sceneEvents_[scene] += [&device] { device.set( true ); };
            }
            for ( auto scene : offScenes ) {
                sceneEvents_[scene] += [&device] { device.set( false ); };
            }
        }

        template< typename Device >
        void addSceneDevice( Device& device )
        {
            addSceneDevice( device, { Scene::SCENE1 }, { Scene::SLEEP, Scene::OFF, Scene::SCENE2 } );
        }

        void sceneButtonClicked( unsigned clicked );
        void sceneCommand( String const& sceneName );

        void addRemoteDevice( String name );

        void remoteDeviceButtonClicked( String const& name, unsigned clicked );

    private:
        void changeScene( Scene scene, bool publish = true );

        IoT& iot_;
        char const* zone_;
        Scene scene_ {};
        std::map< Scene, Event < void() > >
        sceneEvents_;
        std::map< String, bool > remoteDevices_;
    };

} // namespace iot

#endif // ESP8266_IOT_SCENEMANAGER_HPP
