
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

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
        ServerTypes.hpp
)

target_link_libraries(catan_network_server
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)

target_link_libraries(catan_network_server PUBLIC
        catan_transport
        catan_dto
        catan_proto
        catan_game
        Qt6::Core
        Qt6::Network
)

target_include_directories(catan_network_server
        PUBLIC
        .
)

set_target_properties(catan_network_server PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)
