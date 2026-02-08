//
// Created by matija on 1/24/26.
//

#include "MessageDispatcher.hpp"

MessageDispatcher::MessageDispatcher(RoomManager &rm) : m_roomManager(rm) {
}

void MessageDispatcher::dispatch(ClientConnection* c,const net::Envelope& env)
{
    switch (env.msg_type()) {

        case net::MsgType::MSG_SETUP:
            dispatchSetup(c, env.setup());
            break;
        case net::MsgType::MSG_MOVE:
            m_roomManager.forwardMove(c, env.move());
            break;
        case net::MsgType::MSG_CHAT:
            m_roomManager.forwardMessage(c, env.chat().message());
            break;
        case net::MsgType::MSG_ACK:
            m_roomManager.ackPlayer(c);
            break;
        default:
            c->softErrored(c,"Received unknown message");
            break;
    }
}

void MessageDispatcher::dispatchSetup(ClientConnection *c,const net::Setup &setup) {
    switch (setup.type()) {
        case net::Setup_MessageType_HOST_REQUEST:
            c->setName(setup.host_request().player_name());
            m_roomManager.createRoom(c,setup.host_request().game_name());
        break;
        case net::Setup_MessageType_JOIN_REQUEST:
            c->setName(setup.host_request().player_name());
            m_roomManager.requestJoinRoom(c, setup.join_request().game_name(), setup.join_request().player_name());
        break;
        case net::Setup_MessageType_LEAVE:
            m_roomManager.removePlayer(c);
        break;
        case net::Setup_MessageType_GAME_CONFIG:
            m_roomManager.forwardConfig(c,setup.game_config());
        break;
        case net::Setup_MessageType_START_GAME_REQUEST:
            m_roomManager.forwardStartGame(c,setup.start_game_request());
        break;
        default:
            c->softErrored(c,"Received unknown message");
        break;
    }
}
