cmake_minimum_required(VERSION 3.12)
project(esp8266_IoT CXX)

find_package(ArduinoEsp8266 REQUIRED COMPONENTS
        ArduinoJson
        AsyncMqttClient
        ESP8266HTTPUpdateServer
        ESP8266WebServer
        ESP8266WiFi
        ESPAsyncTCP
        NeoPixelBus_by_Makuna
        SPI
        Wire)
link_libraries(${ArduinoEsp8266_LIBRARIES})

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DLOGGER_SERIAL)
elseif(${CMAKE_BUILD_TYPE} STREQUAL Testing)
    add_definitions(-DLOGGER_SOCKET)
endif()

add_subdirectory(IoT)
link_libraries(IoT)

add_subdirectory(ledgraph)

function(add_iot_executable name)
    add_esp8266_executable("${name}" ${ARGN})

    target_compile_definitions("${name}" PUBLIC IOT_TARGET_NAME="${name}")

    add_custom_target("${name}_ota"
            COMMAND curl -v -F "file=@$<TARGET_FILE:${name}>.bin" -F submit=update -u admin:admin "http://IoT-${name}/update")
endfunction()

add_iot_executable(Vorratskeller FLASH 4M Vorratskeller.cpp)
add_iot_executable(Kueche FLASH 512k Kueche.cpp)
add_iot_executable(KellerflurWerkstatt FLASH 512k KellerflurWerstatt.cpp)
target_link_libraries(KellerflurWerkstatt PUBLIC ledgraph)
add_iot_executable(SchmalerFlurGarderobe FLASH 512k SchmalerFlurGarderobe.cpp)
target_link_libraries(SchmalerFlurGarderobe PUBLIC ledgraph)
add_iot_executable(KuecheVitrine FLASH 512k KuecheVitrine.cpp)
add_iot_executable(Gaestezimmer FLASH 512k Gaestezimmer.cpp)
add_iot_executable(Untitled FLASH 4M Untitled.cpp)
add_iot_executable(Sleep-Light-Floor FLASH 4M Sleep-Light-Floor.cpp)
add_iot_executable(Sleep-Light-Ceiling FLASH 4M Sleep-Light-Ceiling.cpp)
add_iot_executable(Sleep-Switch-Bed FLASH 1M Sleep-Switch-Bed.cpp)
add_iot_executable(Colin-Light-Living FLASH 4M Colin-Light-Living.cpp)
add_iot_executable(Living-Shutter-Back FLASH 1M Living-Shutter-Back.cpp)
