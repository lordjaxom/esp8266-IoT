#include <array>
#include <memory>
#include <type_traits>
#include <vector>

#include "IoT/Debounce.hpp"
#include "IoT/Device.hpp"
#include "IoT/IoT.hpp"
#include "IoT/Pcf8574.hpp"
#include "IoT/PushButton.hpp"
#include "IoT/SceneManager.hpp"
#include "IoT/WireConfig.hpp"
#include "IoT/String.hpp"

using namespace std;

template< typename T, size_t N >
class StaticVector
{
public:
    template< typename ...Args >
    T& emplace( Args&&... args )
    {
        auto& data = data_[size_++];
        new(&data) T( std::forward< Args >( args )... );
    }

private:
    typename std::aligned_storage< sizeof( T ), alignof( T ) >::type data_[N];
    size_t size_ {};
};

static constexpr char const* deviceNames[] = {
        "Vitrinen",
        "Tuer",
        "Haengeschraenke",
        "Anrichte",
        "Deckenlampe",
        "Fensterbank",
        "Theke"
};
static constexpr size_t deviceCount = sizeof( deviceNames ) / sizeof( deviceNames[0] );

iot::IoT IoT( "akvsoft", "sacomoco02047781", "192.168.178.28", 1883, "Vorratskeller" );

iot::WireConfig wireConfig( IoT, 2, 0 );
iot::Pcf8574 pcf8574Output( wireConfig, 56, 0xff );
iot::Pcf8574 pcf8574Input( wireConfig, 57, 0x00 );

StaticVector< iot::PushButton, deviceCount > buttons;
StaticVector< iot::Device, deviceCount > outputs;

iot::SceneManager sceneManager( IoT, "Kueche" );

void setup()
{
    for ( uint8_t i = 0; i < 7; ++i ) {
        auto& button = buttons.emplace( IoT, iot::debounce( [i] { return !pcf8574Input.read( i ); } ));
        auto& output = outputs.emplace( IoT, iot::str( "Kueche/", deviceNames[i] ), [i]( bool value ) { pcf8574Output.set( i, value ); } );

        sceneManager.addLocalDevice( output );

        button.clickedEvent += [&output]( unsigned clicked ) { sceneManager.deviceButtonClicked( output, clicked ); };
    }

    IoT.begin();
}

void loop()
{
    IoT.loop();
}