//
// Created by matija on 1/24/26.
//

#ifndef CATAN_SESSIONSTATE_HPP
#define CATAN_SESSIONSTATE_HPP
#include <string>

enum class SessionTypes {
    Connected,
    InLobby,
    InGame,
    Disconnected,
};

enum class RoomState {
    InLobby,
    InGame,
};



#endif //CATAN_SESSIONSTATE_HPP