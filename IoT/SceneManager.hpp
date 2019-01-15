#ifndef ESP8266_IOT_SCENEMANAGER_HPP
#define ESP8266_IOT_SCENEMANAGER_HPP

#include <algorithm>
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

    static constexpr Scene allScenes[] { Scene::OFF, Scene::SCENE1, Scene::SCENE2, Scene::SLEEP };

    class SceneManager
    {

    public:
        SceneManager( IoT& iot, char const* zone ) noexcept;
        SceneManager( SceneManager const& ) = delete;

        void addSceneEvent( Scene scene, std::function< void() > handler );

        template< typename Device >
        void addSceneDeviceEx( Device& device, std::vector< Scene > const& onScenes, std::vector< Scene > const& ignoredScenes = {} )
        {
            for ( auto scene : onScenes ) {
                sceneEvents_[scene] += [&device] { device.set( true ); };
            }

            std::vector< Scene > offScenes( allScenes, allScenes + sizeof( allScenes ) / sizeof( allScenes[0] ));
            offScenes.erase( std::remove_if( offScenes.begin(), offScenes.end(), [&]( Scene scene ) {
                return std::find( onScenes.begin(), onScenes.end(), scene ) != onScenes.end() ||
                       std::find( ignoredScenes.begin(), ignoredScenes.end(), scene ) != ignoredScenes.end();
            } ));
            for ( auto scene : offScenes ) {
                sceneEvents_[scene] += [&device] { device.set( false ); };
            }
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

        void addRemoteDevice( String name );

        void remoteDeviceButtonClicked( String const& name, unsigned clicked );

    private:
        void changeScene( String const& sceneName );
        void changeScene( Scene scene, bool publish = true );

        IoT& iot_;
        char const* zone_;
        Scene scene_ {};
        std::map< Scene, Event < void() > > sceneEvents_;
        std::map< String, bool > remoteDevices_;
    };

} // namespace iot

#endif // ESP8266_IOT_SCENEMANAGER_HPP
