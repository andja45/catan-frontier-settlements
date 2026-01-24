//
// Created by jovan on 24/01/2026.
//

#include "GameNetwork.h"
#include "networkmove/MoveProtoSerializer.h"
#include "networkmove/MoveProtoFactory.h"

GameNetwork::GameNetwork(QObject* parent)
  : QObject(parent)
{
    connect(&transport, &NetworkTransport::envelopeReceived,
            this, &GameNetwork::onEnvelope);
}

void GameNetwork::connectToServer(const QHostAddress& addr, uint16_t port) {
    transport.connectTo(addr, port);
}

net::Envelope GameNetwork::wrapMove(const Move& move) {
    net::Envelope env;
    env.set_msg_type(net::MSG_MOVE);
    env.set_seq(nextSeq++);

    *env.mutable_move() = MoveProtoSerializer::toProto(move);

    return env;
}

void GameNetwork::sendMove(const Move& move) {
    net::Envelope env = wrapMove(move);
    transport.sendEnvelope(env);
}

void GameNetwork::onEnvelope(const net::Envelope& env) {
    if (env.msg_type() != net::MSG_MOVE)
        return;

    auto move = MoveProtoFactory::fromProto(env.move());
    if (move)
        emit remoteMoveReceived(std::move(move));
}
