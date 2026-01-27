//
// Created by matija on 1/24/26.
//

#include "RoomManager.hpp"

bool RoomManager::createRoom(ClientConnection* host, const std::string& roomName)
{
    if (nameTaken(roomName))
        return false; //!
    auto room = std::make_unique<Room>(roomName, host);
    host->setRoomId(roomName);
    m_rooms[roomName] = std::move(room);
    return true;
}

bool RoomManager::joinRoom(ClientConnection *client, const std::string &roomName, const std::string &playerName) {
    if (nameTaken(roomName))
        return false; //!

    client->setRoomId(roomName);
    if (m_rooms[roomName]->isFull())
        return false;

    m_rooms[roomName]->addPlayer(client,playerName);
    return true;
}

void RoomManager::forwardMove(ClientConnection *sender, net::Move &move) {
    auto roomId = sender->roomId();
    m_rooms.at(roomId)->processMove(sender, move);
    // if game is over?
}

void RoomManager::forwardStartGame(ClientConnection *host, const net::StartGameRequest &req) {
    auto roomId = host->roomId(); // TODO how to propagate errors? you dont, you kick players dont bother sever
    m_rooms.at(roomId)->processStartRequest(host, req);
}

void RoomManager::forwardMessage(ClientConnection *sender, const std::string &msg) {
    auto roomId = sender->roomId();
    m_rooms.at(roomId)->processMessage(sender, msg);
}

void RoomManager::forwardConfig(ClientConnection *host, const net::GameConfig & cfg) {
    auto roomId = host->roomId();
    m_rooms.at(roomId)->processConfig(host, cfg);
}

void RoomManager::handleDisconnect(ClientConnection *client) {
    auto roomId = client->roomId();
    m_rooms.at(roomId)->removePlayer(client);
    if (m_rooms.at(roomId)->isEmpty()) removeRoom(roomId);
}

void RoomManager::removeRoom(const std::string &roomName) {
    m_rooms.erase(roomName);
}

bool RoomManager::nameTaken(const std::string &roomName) {
    for (auto& room : m_rooms) {
        if (room.first == roomName) return true;
    }
    return false;
}
