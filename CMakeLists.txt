cmake_minimum_required(VERSION 3.12)
project(esp8266_IoT CXX)

find_package(ArduinoEsp8266 REQUIRED COMPONENTS ESP8266WiFi Wire NeoPixelBus_by_Makuna SPI ESPAsyncTCP AsyncMqttClient)
link_libraries(${ArduinoEsp8266_LIBRARIES})

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DLOGGER_SERIAL)
endif ()

add_subdirectory(IoT)
link_libraries(IoT)

add_subdirectory(ledgraph)

add_esp8266_executable(KellerflurTreppe FLASH 512k KellerflurTreppe.cpp)
add_esp8266_executable(Colin FLASH 4M Colin.cpp)
add_esp8266_executable(Vorratskeller FLASH 4M Vorratskeller.cpp)
add_esp8266_executable(Kueche FLASH 512k Kueche.cpp)
add_esp8266_executable(Kellerflur FLASH 512k Kellerflur.cpp)
target_link_libraries(Kellerflur PUBLIC ledgraph)
add_esp8266_executable(SchmalerFlur FLASH 512k SchmalerFlur.cpp)
target_link_libraries(SchmalerFlur PUBLIC ledgraph)
add_esp8266_executable(KuecheVitrine FLASH 512k KuecheVitrine.cpp)
add_esp8266_executable(Gaestezimmer FLASH 512k Gaestezimmer.cpp)
add_esp8266_executable(Untitled FLASH 4M Untitled.cpp)