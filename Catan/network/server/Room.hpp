//
// Created by matija on 1/24/26.
//

#ifndef CATAN_ROOM_HPP
#define CATAN_ROOM_HPP

#include <vector>
#include <memory>
#include <model/GameConfig.h>

#include "ClientConnection.hpp"
#include "model/GameSession.h"

enum class RoomState {
    Lobby,
    InGame
};

class Room {
public:
    Room(const std::string &, ClientConnection* host);

    void addPlayer(ClientConnection*, std::string name);
    void removePlayer(ClientConnection*);

    void kickPlayer(ClientConnection * client_connection);

    void processMove(ClientConnection *, net::Move &move);
    void processMessage(ClientConnection*, const std::string&);

    void processConfig(ClientConnection*,const net::GameConfig&);
    void processStartRequest(ClientConnection*, const net::StartGameRequest&);

    void broadcast(const net::Envelope&);

    void processDice(net::Move &move);
    void processTradeRequest(net::Move &move);

    bool isEmpty() const;
    bool isFull() const;

    void broadcastConfig();
    void broadcastMove(net::Move);
    void broadcastStartGame();



private:
    std::string m_name;
    RoomState m_state = RoomState::Lobby;
    ClientConnection* m_host=nullptr;
    std::vector<ClientConnection*> m_players;

    std::unique_ptr<GameSession> m_session;
    std::unique_ptr<GameConfig> m_config;

    int m_currTradeId=0;
    int m_seed;

    std::mt19937 m_rng;
    std::uniform_int_distribution<int> m_d6{1, 6};
};


#endif //CATAN_ROOM_HPP