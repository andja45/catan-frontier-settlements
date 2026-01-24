//
// Created by matija on 1/24/26.
//

#ifndef CATAN_SESSIONSTATE_HPP
#define CATAN_SESSIONSTATE_HPP

enum class SessionState {
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