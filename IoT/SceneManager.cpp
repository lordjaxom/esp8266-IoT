#include <algorithm>
#include <utility>

#include "IoT.hpp"
#include "Logger.hpp"
#include "SceneManager.hpp"
#include "String.hpp"

using namespace std;

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

vector< Scene > offScenes( vector< Scene > const& onScenes, vector< Scene > const& ignoredScenes )
{
    vector< Scene > offScenes( allScenes, allScenes + sizeof( allScenes ) / sizeof( allScenes[0] ));
    offScenes.erase( remove_if( offScenes.begin(), offScenes.end(), [&]( Scene scene ) {
        return find( onScenes.begin(), onScenes.end(), scene ) != onScenes.end() ||
               find( ignoredScenes.begin(), ignoredScenes.end(), scene ) != ignoredScenes.end();
    } ), offScenes.end());
    return offScenes;
}

SceneManager::SceneManager( char const* zone )
        : zone_( zone )
{
    IoT.loopEvent += [this] { loop(); };
    IoT.subscribe( str( "cmnd/", zone_, "/LIGHTSCENE" ), [this]( String message ) { changeScene( message ); } );
}

void SceneManager::loop()
{
    for ( auto it = publishedScenes_.begin(); it != publishedScenes_.end(); ) {
        if ( --it->second == 0 ) {
            it = publishedScenes_.erase( it );
        } else {
            ++it;
        }
    }
}

void SceneManager::addSceneEvent( Scene scene, function< void() > handler )
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

void SceneManager::changeScene( String const& sceneName )
{
    Scene scene = toScene( sceneName );
    if ( scene != Scene::UNKNOWN ) {
        auto it = publishedScenes_.find( scene );
        if ( it == publishedScenes_.end()) {
            changeScene( scene, false );
        } else {
            publishedScenes_.erase( it );
        }
    }
}

void SceneManager::changeScene( Scene scene, bool publish )
{
    log( "switching to scene ", scene, " in zone ", zone_ );

    sceneEvents_[scene]();
    if ( publish ) {
        IoT.publish( str( "cmnd/", zone_, "/LIGHTSCENE" ), toString( scene ));
        publishedScenes_.emplace( scene, 1000 / IoTClass::tick );
    }

    scene_ = scene;
}

