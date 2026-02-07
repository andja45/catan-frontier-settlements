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
    void sendAccept(ClientConnection * host);

    void sendDecline(ClientConnection * host, const char * str);

    bool createRoom(ClientConnection* host, const std::string& roomName);
    bool requestJoinRoom(ClientConnection* client, const std::string& roomName, const std::string& playerName);

    void handleDisconnect(ClientConnection*);

    void forwardMove(ClientConnection *sender, const net::Move &move);
    void forwardStartGame(ClientConnection* host, const net::StartGameRequest&);
    void forwardMessage(ClientConnection* sender, const std::string&);
    void forwardConfig(ClientConnection* host, const net::GameConfig&);
    void ackPlayer(ClientConnection * client_connection);

    void handleError(ClientConnection * client,const std::string& error);

    void cleanEmptyRooms();
private:
    std::unordered_map<std::string, std::unique_ptr<Room>> m_rooms;
    void removeRoom(const std::string& roomName);
    bool nameTaken(const std::string& roomName);
};


#endif //CATAN_ROOMMANAGER_HPP