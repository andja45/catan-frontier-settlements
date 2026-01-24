//
// Created by matija on 1/24/26.
//

#ifndef CATAN_ROOM_HPP
#define CATAN_ROOM_HPP



class Room {
public:
    Room(RoomId id, ClientConnection* host);

    void addPlayer(ClientConnection*);
    void removePlayer(ClientConnection*);

    void startGame();

    void broadcast(const Envelope&);
    bool isHost(ClientConnection*) const;

private:
    RoomState state;
    std::vector<ClientConnection*> players;
    GameConfig config;

    std::unique_ptr<GameSession> session; // tek kad krene igra
};

#endif //CATAN_ROOM_HPP