

add_executable(Catan
        main.cpp
        ../controller/host-join/HostJoinController.cpp
        ../controller/host-join/HostJoinController.hpp
        ../controller/game/GameApp.cpp
        ../controller/game/GameApp.hpp
        ../controller/host-join/HostJoinApp.cpp
        ../controller/host-join/HostJoinApp.hpp
        ../controller/lobby/LobbyApp.cpp
        ../controller/lobby/LobbyApp.hpp
        ../controller/ApplicationRoot.cpp
        ../controller/ApplicationRoot.hpp
        ../controller/AbstractApplet.hpp
        ../controller/RoleEnum.hpp
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
        catan_ascii_view
        catan_network_client
        catan_network_server
        catan_game

)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
