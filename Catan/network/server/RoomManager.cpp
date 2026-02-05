//
// Created by matija on 1/24/26.
//

#include "RoomManager.hpp"

void RoomManager::sendAccept(ClientConnection *host) {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    env.mutable_setup()->set_type(net::Setup_MessageType_ACCEPT);
    env.mutable_setup()->mutable_ack();
    host->send(env);
}

void RoomManager::sendDecline(ClientConnection *host, const char *str) {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    env.mutable_setup()->set_type(net::Setup_MessageType_REJECT);
    env.mutable_setup()->mutable_error()->set_message(str);
    host->send(env);
}

bool RoomManager::createRoom(ClientConnection* host, const std::string& roomName)
{
    if (nameTaken(roomName)) {
        sendDecline(host, "Room with that name already exists");
        return false;
    }

    if (host->hasRoom()) {
        sendDecline(host, "You are already in room!");
        return false;
    }
    // we make room and wait for host to join, if he failes we delete the room
    auto room = std::make_unique<Room>(roomName, host);
    m_rooms[roomName] = std::move(room);

    if (requestJoinRoom(host,roomName,host->name())) {
        m_rooms[roomName]->setHost(host);
        host->setHost();
        return true;
    }
    removeRoom(roomName);
    return false;
}

bool RoomManager::requestJoinRoom(ClientConnection *client, const std::string &roomName, const std::string &playerName) {
    if (!nameTaken(roomName)) {
        sendDecline(client, "Room with that name doesnt exist");
        return false;
    }

    if (m_rooms[roomName]->isFull()) {
        sendDecline(client, "Room is full!");
        return false;
    }

    if (client->hasRoom()) {
        sendDecline(client, "You are already in room!");
        return false;
    }

    client->setRoomId(roomName);
    client->setStatus(ClientStatus::WaitingForRoom);
    client->setName(playerName);
    sendAccept(client);
    return true;
}

void RoomManager::forwardMove(ClientConnection *sender, const net::Move &move) {
    auto roomId = sender->roomId();
    auto room=m_rooms.at(roomId).get();
    room->processMove(sender, move);
    if (room->isEmpty()) {
        removeRoom(roomId);
    }
}

void RoomManager::forwardStartGame(ClientConnection *host, const net::StartGameRequest &req) {
    auto roomId = host->roomId();
    auto room=m_rooms.at(roomId).get();
    room->processStartRequest(host, req);
    if (room->isEmpty()) {
        removeRoom(roomId);
    }
}

void RoomManager::forwardMessage(ClientConnection *sender, const std::string &msg) {
    auto roomId = sender->roomId();
    m_rooms.at(roomId)->processMessage(sender, msg);
}

void RoomManager::forwardConfig(ClientConnection *host, const net::GameConfig & cfg) {
    auto roomId = host->roomId();
    auto room=m_rooms.at(roomId).get();
    room->processConfig(host, cfg);
    if (room->isEmpty()) {
        removeRoom(roomId);
    }
}

void RoomManager::ackPlayer(ClientConnection *c) {
    if (c->hasRoom()&&c->status()==ClientStatus::WaitingForRoom) {
        m_rooms[c->roomId()]->addPlayer(c,c->name());
        c->setStatus(ClientStatus::InLobby);
    }
    else if (c->hasRoom()&&c->status()==ClientStatus::WaitingForGame) {
        m_rooms[c->roomId()]->setPlayerReady(c);
    }
    else {
        c->onError("Client cant be accepted to room");
    }
}

void RoomManager::handleDisconnect(ClientConnection *client) {
    auto roomId = client->roomId();
    m_rooms.at(roomId)->removePlayer(client);

    if (m_rooms.at(roomId)->isEmpty()) removeRoom(roomId);
}

void RoomManager::handleError(ClientConnection *client, const std::string &error) {
    auto roomId = client->roomId();
    m_rooms.at(roomId)->processError(client, error);
}

void RoomManager::cleanEmptyRooms() {
    for (auto it=m_rooms.begin();it!=m_rooms.end();) {
        if (it->second==nullptr || it->second->isEmpty()) {
            removeRoom(it->first);
        } else {
            ++it;
        }
    }
}

void RoomManager::removeRoom(const std::string &roomName) {
    m_rooms.at(roomName)->kickEveryone();
    m_rooms.erase(roomName);
}

bool RoomManager::nameTaken(const std::string &roomName) {
    for (auto& room : m_rooms) {
        if (room.first == roomName) return true;
    }
    return false;
}
