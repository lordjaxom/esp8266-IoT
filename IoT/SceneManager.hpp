#ifndef ESP8266_IOT_SCENEMANAGER_HPP
#define ESP8266_IOT_SCENEMANAGER_HPP

#include <map>
#include <vector>

#include "Event.hpp"
#include "String.hpp"

enum class Scene : uint8_t
{
    OFF,
    SCENE1,
    SCENE2,
    SLEEP = 64,
    UNKNOWN = 0xff
};

static constexpr Scene allScenes[] { Scene::OFF, Scene::SCENE1, Scene::SCENE2, Scene::SLEEP };

std::vector< Scene > offScenes( std::vector< Scene > const& onScenes, std::vector< Scene > const& ignoredScenes );

class SceneManager
{
public:
    explicit SceneManager( char const* zone ) noexcept;
    SceneManager( SceneManager const& ) = delete;

    void addSceneEvent( Scene scene, std::function< void() > handler );

    template< typename Device >
    void addLocalDevice( Device& device, std::vector< Scene > const& sleepScenes = { Scene::SLEEP } )
    {
        devices_.emplace( device.name(), [&device] { device.toggle(); } );
        for ( auto scene : sleepScenes ) {
            sceneEvents_[scene] += [&device] { device.set( false ); };
        }
    }

    template< typename Device >
    void addSceneDevice( Device& device, std::vector< Scene > const& onScenes = { Scene::SCENE1 },
                         std::vector< Scene > const& ignoredScenes = {} )
    {
        for ( auto scene : onScenes ) {
            sceneEvents_[scene] += [&device] { device.set( true ); };
        }
        for ( auto scene : offScenes( onScenes, ignoredScenes )) {
            sceneEvents_[scene] += [&device] { device.set( false ); };
        }
        addLocalDevice( device );
    }

    void sceneButtonClicked( unsigned clicked );

    template< typename Device >
    void deviceButtonClicked( Device& device, unsigned clicked )
    {
        if ( clicked == 1 ) {
            devices_.find( device.name())->second();
        } else {
            sceneButtonClicked( clicked );
        }
    }

private:
    void loop();

    void changeScene( String const& sceneName );
    void changeScene( Scene scene, bool publish = true );

    char const* zone_;
    Scene scene_ {};
    std::map< Scene, Event < void() > >
    sceneEvents_;
    std::map< StringView, std::function< void() > > devices_;
    std::multimap< Scene, uint32_t > publishedScenes_;
};

#endif // ESP8266_IOT_SCENEMANAGER_HPP
