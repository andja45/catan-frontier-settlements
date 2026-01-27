add_library(catan_network_server
        ClientConnection.cpp
        ClientConnection.hpp
        MessageDispatcher.cpp
        MessageDispatcher.hpp
        Room.cpp
        Room.hpp
        RoomManager.cpp
        RoomManager.hpp
        ServerController.cpp
        ServerController.hpp
        SessionTypes.hpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)


target_link_libraries(catan_network_server
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)

target_link_libraries(catan_network_server PUBLIC catan_transport)
target_link_libraries(catan_network_server PUBLIC catan_dto)
target_link_libraries(catan_network_server PUBLIC catan_proto)
target_link_libraries(catan_network_server PUBLIC catan_game)

target_include_directories(catan_network_server
        PUBLIC
        .
)
