//
// Created by jovan on 24/01/2026.
//

#include "GameNetworkAdapter.h"
#include "move/MoveProtoSerializer.h"
#include "move/MoveProtoFactory.h"

GameNetworkAdapter::GameNetworkAdapter(QObject* parent)
  : QObject(parent)
  , m_transport(nullptr)
{
}

net::Envelope GameNetworkAdapter::wrapMove(const Move& move) const {
    net::Envelope env;
    env.set_msg_type(net::MSG_MOVE);
    env.set_seq(m_transport->getNextSeqToSend());

    *env.mutable_move() = MoveProtoSerializer::toProto(move);

    return env;
}

net::Envelope GameNetworkAdapter::wrapMessage(const std::string &message) const {
    net::Envelope env;
    env.set_msg_type(net::MSG_CHAT);
    env.set_seq(m_transport->getNextSeqToSend());
    env.mutable_chat()->set_message(message);
    return env;
}

void GameNetworkAdapter::sendMove(const Move* move) {
    net::Envelope env = wrapMove(*move);
    m_transport->sendEnvelope(env);
}

void GameNetworkAdapter::setTransport(NetworkTransport* transport) {
    m_transport=transport;
    connect(m_transport, &NetworkTransport::envelopeReceived,
            this, &GameNetworkAdapter::onEnvelope);
}

void GameNetworkAdapter::sendMessage(const std::string& author,const std::string &message) {
    m_transport->sendEnvelope(wrapMessage(message));
}

void GameNetworkAdapter::onEnvelope(const net::Envelope& env) {
    if (env.msg_type() == net::MSG_MOVE)
        handleMove(env);
    else if (env.msg_type() == net::MSG_CHAT)
        handleMessage(env);

}

void GameNetworkAdapter::handleMessage(const net::Envelope &env) {
    auto msg = std::string(env.chat().message().begin(), env.chat().message().end());
    // Author isn't currently encoded in the protobuf envelope, so emit empty author for now.
    emit remoteMessageReceived("", msg);
}

void GameNetworkAdapter::handleMove(const net::Envelope &env) {
    auto move = MoveProtoFactory::fromProto(env.move());
    if (move)
        emit remoteMoveReceived(move.release());
}
