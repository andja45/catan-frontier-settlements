add_library(catan_network_client
        GameNetworkAdapter.h
        GameNetworkAdapter.cpp
        LobbyNetworkAdapter.cpp
        LobbyNetworkAdapter.hpp
        HostJoinNetworkAdapter.cpp
        HostJoinNetworkAdapter.hpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)


target_link_libraries(catan_network_client
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)

target_link_libraries(catan_network_client PUBLIC catan_game)
target_link_libraries(catan_network_client PUBLIC catan_proto)
target_link_libraries(catan_network_client PUBLIC catan_dto)
target_link_libraries(catan_network_client PUBLIC catan_transport)

target_include_directories(catan_network_client
        PUBLIC
        .
)
