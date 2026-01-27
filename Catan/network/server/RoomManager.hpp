//
// Created by matija on 1/24/26.
//

#ifndef CATAN_ROOMMANAGER_HPP
#define CATAN_ROOMMANAGER_HPP

#include <ClientConnection.hpp>
#include <unordered_map>
#include <memory>
#include "Room.hpp"

class RoomManager {
public:
    bool createRoom(ClientConnection* host, const std::string& roomName);
    bool joinRoom(ClientConnection* client, const std::string& roomName, const std::string& playerName);

    void handleDisconnect(ClientConnection*);

    void forwardMove(ClientConnection *sender, net::Move &move);
    void forwardStartGame(ClientConnection* host, const net::StartGameRequest&);
    void forwardMessage(ClientConnection* sender, const std::string&);
    void forwardConfig(ClientConnection* host, const net::GameConfig&);
private:
    std::unordered_map<std::string, std::unique_ptr<Room>> m_rooms;
    void removeRoom(const std::string& roomName);
    bool nameTaken(const std::string& roomName);
};


#endif //CATAN_ROOMMANAGER_HPP