

add_executable(Catan
        main.cpp
        ../network/server/SessionTypes.hpp
        ../controller/lobby/LobbyController.cpp
        ../controller/lobby/LobbyController.hpp
        ../controller/game/GameController.cpp
        ../controller/game/GameController.hpp
        ../network/client/LobbyNetworkAdapter.cpp
        ../network/client/LobbyNetworkAdapter.hpp
        ../network/client/INetworkAdapter.cpp
        ../network/client/INetworkAdapter.hpp
        ../network/server/Client.cpp
        ../network/server/Client.hpp
        ../network/server/Room.cpp
        ../network/server/Room.hpp
        ../network/server/RoomManager.cpp
        ../network/server/RoomManager.hpp
        ../network/server/ServerController.cpp
        ../network/server/ServerController.hpp
        ../network/server/MessageRouter.cpp
        ../network/server/MessageRouter.hpp
        ../network/dto-translation/board/BoardSerializer.cpp
        ../network/dto-translation/board/BoardSerializer.hpp
        ../network/dto-translation/board/BoardFactory.cpp
        ../network/dto-translation/board/BoardFactory.hpp
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
        catan_ascii_view
)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
