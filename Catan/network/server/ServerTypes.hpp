//
// Created by matija on 1/24/26.
//

#ifndef CATAN_SESSIONSTATE_HPP
#define CATAN_SESSIONSTATE_HPP
#include <chrono>
#include <string>

namespace std::chrono::_V2 {
    struct steady_clock;
}

enum class ClientStatus {
    Connected,
    WaitingForRoom,
    InLobby,
    WaitingForGame,
    InGame,
    Disconnected,
};

enum class RoomState {
    Lobby,
    InGame
};

using TimeStamp=std::chrono::time_point<std::chrono::_V2::steady_clock>;
using Clock = std::chrono::steady_clock;

// time after which player is considered inactive (one day in seconds)
#define TIME_LIMIT 24*60*60

#endif //CATAN_SESSIONSTATE_HPP