#include <array>
#include <vector>

#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Pcf8574.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/Wire.hpp"
#include "IoT/String.hpp"

using namespace std;

char const* deviceName( uint8_t index )
{
    switch ( index ) {
        case 0: return "Vitrinen";
        case 1: return "Tuer";
        case 2: return "Haengeschraenke";
        case 3: return "Anrichte";
        case 4: return "Deckenlampe";
        case 5: return "Fensterbank";
        case 6: return "Theke";
    }
    return "";
}

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Vorratskeller" );

iot::Wire wire( IoT, 2, 0 );
iot::Pcf8574 pcf8574Output( wire, 56, 0xff );
iot::Pcf8574 pcf8574Input( wire, 57, 0x00 );

array< String, 7 > topicDeviceNames;
vector< iot::PushButton > buttons;
vector< iot::Device> outputs;

iot::SceneManager sceneManager( IoT, "Kueche" );

void setup()
{
    for ( uint8_t i = 0; i < 7; ++i ) {
        topicDeviceNames[i] = iot::str( "Kueche/", deviceName( i ));
        buttons.emplace_back( IoT, iot::debounce( [i] { return !pcf8574Input.read( i ); } ));
        outputs.emplace_back( IoT, topicDeviceNames[i], [i]( bool value ) { pcf8574Output.set( i, value ); } );

        buttons[i].clickedEvent +=
                
                sceneManager.addSceneDevice( outputs[i], {}, { iot::Scene::OFF})
    }

    IoT.begin();
}

void loop()
{
    IoT.loop();
}