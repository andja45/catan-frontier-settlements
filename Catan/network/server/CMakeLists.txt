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

# Server uses Qt Network/Core. Allow disabling when Qt isn't available.
if(BUILD_NETWORK_QT)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)


    target_link_libraries(catan_network_server
            PUBLIC
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Network
    )

    target_link_libraries(catan_network_server PUBLIC catan_transport)
else()
    # When Qt networking is disabled, still allow building server sources but without Qt link deps.
    message(STATUS "BUILD_NETWORK_QT is OFF: configuring catan_network_server without Qt dependencies")
endif()

target_link_libraries(catan_network_server PUBLIC catan_dto)
target_link_libraries(catan_network_server PUBLIC catan_proto)
target_link_libraries(catan_network_server PUBLIC catan_game)

target_include_directories(catan_network_server
        PUBLIC
        .
)
