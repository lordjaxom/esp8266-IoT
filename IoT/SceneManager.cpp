#include "IoT.hpp"
#include "Logger.hpp"
#include "MQTT.hpp"
#include "SceneManager.hpp"
#include "String.hpp"

namespace iot {

    static char const* toString( Scene scene )
    {
        switch ( scene ) {
            case Scene::OFF:
                return "OFF";
            case Scene::SCENE1:
                return "SCENE1";
            case Scene::SCENE2:
                return "SCENE2";
            case Scene::SLEEP:
                return "SLEEP";
            default:
                return "";
        }
    }

    static Scene toScene( String const& scene )
    {
        if ( scene == "OFF" ) return Scene::OFF;
        if ( scene == "SCENE1" ) return Scene::SCENE1;
        if ( scene == "SCENE2" ) return Scene::SCENE2;
        if ( scene == "SLEEP" ) return Scene::SLEEP;
        return Scene::UNKNOWN;
    }

    SceneManager::SceneManager( IoT& iot, char const* zone )
            : iot_( iot ),
              zone_( zone )
    {
        MQTT::subscribe( str( "cmnd/", zone_, "/LIGHTSCENE" ), [this]( String message ) { this->sceneCommand( message ); } );
    }

    void SceneManager::addSceneEvent( Scene scene, std::function< void() > handler )
    {
        sceneEvents_[scene] += move( handler );
    }

    void SceneManager::sceneButtonClicked( unsigned clicked )
    {
        if ( clicked == 1 ) {
            changeScene( scene_ == Scene::OFF || scene_ == Scene::SLEEP ? Scene::SCENE1 : Scene::OFF );
        } else if ( clicked > 1 && clicked <= 2 ) {
            changeScene( static_cast< Scene >( static_cast< uint8_t >( Scene::OFF ) + clicked ));
        } else if ( clicked == 0 ) {
            changeScene( Scene::SLEEP );
        }
    }

    void SceneManager::sceneCommand( String const& sceneName )
    {
        Scene scene = toScene( sceneName );
        if ( scene != Scene::UNKNOWN ) {
            changeScene( scene, true );
        }
    }

    void SceneManager::changeScene( Scene scene, bool publish )
    {
        log( "switching to scene ", scene, " in zone ", zone_ );

        sceneEvents_[scene]();
        scene_ = scene;

        if ( publish ) {
            MQTT::publish( str( "stat/", zone_, "/LIGHTSCENE" ), toString( scene_ ));
        }
    }

} // namespace iot