//
// Created by matija on 1/24/26.
//

#ifndef CATAN_ROOM_HPP
#define CATAN_ROOM_HPP

#include <vector>
#include <memory>
#include <ServerTypes.hpp>
#include <model/GameConfig.h>

#include "ClientConnection.hpp"
#include "model/GameSession.h"



class Room {
public:
    Room(const std::string &, ClientConnection* host);

    void addPlayer(ClientConnection*, std::string name);
    void removePlayer(ClientConnection*);

    void kickPlayer(ClientConnection * client_connection);

    void processMove(ClientConnection *, const net::Move &move);
    void processMessage(ClientConnection*, const std::string&);

    void processConfig(ClientConnection*,const net::GameConfig&);
    void processStartRequest(ClientConnection*, const net::StartGameRequest&);

    void broadcast(const net::Envelope&);

    bool isEmpty() const;
    bool isFull() const;

    void broadcastConfig();
    void broadcastMove(net::Move);
    void broadcastStartGame();

    void setHost(ClientConnection * host);

    void kickInactivePlayers();

    void kickEveryone();

    void setPlayerReady(ClientConnection * client_connection);

    bool checkStartGame();

    void lobbyToGame();

    RoomState getState() const;

    ~Room();

private:
    std::string m_name;
    RoomState m_state = RoomState::Lobby;
    ClientConnection* m_host=nullptr;
    std::vector<ClientConnection*> m_players;

    std::unique_ptr<GameSession> m_session; // session for in game config for lobby
    std::unique_ptr<GameConfig> m_config;
    int m_seed;
};


#endif //CATAN_ROOM_HPP