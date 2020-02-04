#include <algorithm>
#include <utility>

#include "IoT.hpp"
#include "Json.hpp"
#include "Logger.hpp"
#include "SceneManager.hpp"
#include "String.hpp"

using namespace std;

static constexpr size_t jsonMessageSize = 256;

static char const* toString( Scene scene )
{
    switch ( scene ) {
        case Scene::OFF:
            return "OFF";
        case Scene::SCENE1:
            return "SCENE1";
        case Scene::SCENE2:
            return "SCENE2";
        case Scene::SCENE3:
            return "SCENE3";
        case Scene::SCENE4:
            return "SCENE4";
        default:
            return "";
    }
}

static Scene toScene( String const& scene )
{
    if ( scene == "OFF" ) return Scene::OFF;
    if ( scene == "SCENE1" ) return Scene::SCENE1;
    if ( scene == "SCENE2" ) return Scene::SCENE2;
    if ( scene == "SCENE3" ) return Scene::SCENE3;
    if ( scene == "SCENE4" ) return Scene::SCENE4;
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
    IoT.subscribe( topic( "SCENE" ), [=]( String message ) { sceneReceived( message ); } );
}

String SceneManager::topic( char const* command ) const
{
    return str( "cmnd/", zone_, "/", command );
}

void SceneManager::addSceneEvent( Scene scene, function< void() > handler )
{
    sceneEvents_[scene] += move( handler );
}

void SceneManager::sceneButtonClicked( unsigned clicked )
{
    if ( clicked == 1 ) {
        changeScene( scene_ == Scene::OFF ? Scene::SCENE1 : Scene::OFF );
    } else if ( clicked > 1 && clicked <= 4 ) {
        changeScene( static_cast< Scene >( static_cast< uint8_t >( Scene::OFF ) + clicked ));
    }
}

void SceneManager::sceneReceived( String const& message )
{
    StaticJsonDocument< jsonMessageSize > json;
    if ( deserializeJson( json, message )) {
        return;
    }
    if ( json["source"] == IoT.clientId() ) {
        return;
    }

    Scene scene = toScene( json["scene"] );
    if ( scene == Scene::UNKNOWN ) {
        return;
    }
    this->changeScene( scene, false );
}

void SceneManager::changeScene( Scene scene, bool publish )
{
    log( "switching to scene ", scene, " in zone ", zone_ );

    sceneEvents_[scene]();

    if ( publish ) {
        StaticJsonDocument< jsonMessageSize > json;
        json["source"] = IoT.clientId();
        json["scene"] = toString( scene );
        IoT.publish( topic( "SCENE" ), str( json ));
    }

    scene_ = scene;
}
