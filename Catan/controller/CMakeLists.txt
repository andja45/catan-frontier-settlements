add_library(catan_controller
        game/GameController.hpp
        game/GameController.cpp
        host-join/HostJoinController.hpp
        host-join/HostJoinController.cpp
        lobby/LobbyController.hpp
        lobby/LobbyController.cpp
        renderstate/RenderState.h
        renderstate/BoardRenderState.h
        renderstate/BoardRenderState.cpp
        renderstate/ToolbarRenderState.h
        renderstate/ToolbarRenderState.cpp
        renderstate/ChoosePlayerRenderState.cpp
        renderstate/ChoosePlayerRenderState.h
)

target_include_directories(catan_controller
        PUBLIC
        .
)

target_link_libraries(catan_controller
        PUBLIC
        catan_game
        catan_gui_qt
        catan_network_client
)
