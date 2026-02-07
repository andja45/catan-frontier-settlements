
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(catan_controller
        game/GameController.hpp
        game/GameController.cpp
        lobby/LobbyController.hpp
        lobby/LobbyController.cpp
        renderstate/RenderState.h
        renderstate/BoardRenderState.h
        renderstate/BoardRenderState.cpp
        renderstate/ToolbarRenderState.h
        renderstate/ToolbarRenderState.cpp
        renderstate/ChoosePlayerRenderState.cpp
        renderstate/ChoosePlayerRenderState.h
        RoleType.hpp

        game/GameApp.cpp
        game/GameApp.hpp
        lobby/LobbyApp.cpp
        lobby/LobbyApp.hpp
        common/ApplicationRoot.cpp
        common/ApplicationRoot.hpp
        common/AbstractApplet.hpp
        RoleType.hpp
        menu/MenuApp.hpp
        host-join/HostJoinApp.hpp
        host-join/HostJoinController.cpp
        host-join/HostJoinController.hpp
        common/NetworkService.hpp
        host-join/HostJoinApp.cpp
        menu/MenuApp.cpp
        renderstate/PopupRenderState.cpp
        renderstate/PopupRenderState.hpp
        history/HistoryApp.hpp

)
target_link_libraries(catan_controller
    PUBLIC
    catan_game
    catan_gui_qt
    catan_transport
    catan_network_client
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
)
target_include_directories(catan_controller
        PUBLIC
        .
)

