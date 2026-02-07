//
// Created by matija on 1/24/26.
//

#ifndef CATAN_MESSAGEROUTER_HPP
#define CATAN_MESSAGEROUTER_HPP

#include "RoomManager.hpp"
#include "ProtoHeaders.hpp"

class ClientConnection;

class MessageDispatcher {
public:
    explicit MessageDispatcher(RoomManager& rm);

    void dispatch(ClientConnection *c, const net::Envelope &env);

private:
    RoomManager& m_roomManager;

    void dispatchSetup(ClientConnection* sender, const net::Setup& setup);
};

#endif //CATAN_MESSAGEROUTER_HPP