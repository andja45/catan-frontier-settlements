//
// Created by matija on 1/24/26.
//

#ifndef CATAN_ROOMMANAGER_HPP
#define CATAN_ROOMMANAGER_HPP

class RoomManager {
public:
    Room& createRoom(ClientConnection* host);
    Room* findRoom(RoomId);
    void destroyRoom(RoomId);

private:
    std::unordered_map<RoomId, std::unique_ptr<Room>> rooms;
};

#endif //CATAN_ROOMMANAGER_HPP