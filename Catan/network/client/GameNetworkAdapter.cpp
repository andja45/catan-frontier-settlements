//
// Created by jovan on 24/01/2026.
//

#include "GameNetworkAdapter.h"
#include <google/protobuf/util/json_util.h>

#include <move/turn/PlayerLeaveMove.hpp>

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

    *env.mutable_move() = MoveProtoSerializer::toProto(move);

    return env;
}

net::Envelope GameNetworkAdapter::wrapMessage(const std::string& author,const std::string &message) const {
    net::Envelope env;
    env.set_msg_type(net::MSG_CHAT);
    env.mutable_chat()->set_message(message);
    env.mutable_chat()->set_player_name(author);
    return env;
}

void GameNetworkAdapter::handleAck(const net::Envelope &env) {
    emit startGame();
}

void GameNetworkAdapter::sendMove(const Move* move) {
    net::Envelope env = wrapMove(*move);
    m_transport->sendEnvelope(env);
}

void GameNetworkAdapter::setTransport(NetworkTransport* transport) {
    m_transport=transport;
    connect(m_transport, &NetworkTransport::envelopeReceived,
            this, &GameNetworkAdapter::onEnvelope);
    connect(m_transport, &NetworkTransport::protocolError,
                this, &GameNetworkAdapter::onError);
    connect(m_transport, &NetworkTransport::disconnected,
                this, &GameNetworkAdapter::onDisconnected);
}

void GameNetworkAdapter::sendMessage(const std::string& author,const std::string &message) {
    m_transport->sendEnvelope(wrapMessage(author, message));
}

void GameNetworkAdapter::sendReady() {
    m_transport->sendAck();
}

void GameNetworkAdapter::onError(const std::string &error) {
    emit errored(error);
}

void GameNetworkAdapter::onDisconnected() {
    emit disconnected();
}

void GameNetworkAdapter::onEnvelope(const net::Envelope& env) {
    std::string jsonString;
    google::protobuf::util::MessageToJsonString(env, &jsonString);
    qDebug() << "[Game] Received message: \n" << jsonString.c_str();

    if (env.msg_type() == net::MSG_MOVE)
        handleMove(env);
    else if (env.msg_type() == net::MSG_CHAT)
        handleMessage(env);
    else if (env.msg_type() == net::MSG_ACK) {
        handleAck(env);
    }
    else {
        std::string jsonString;
        google::protobuf::util::MessageToJsonString(env, &jsonString);
        qDebug() << "Received unknown message: \n" << jsonString.c_str();
        onError("Received unknown message");
    }

}

void GameNetworkAdapter::handleMessage(const net::Envelope &env) {
    auto msg = std::string(env.chat().message().begin(), env.chat().message().end());
    auto author = std::string(env.chat().player_name().begin(), env.chat().player_name().end());
    emit remoteMessageReceived(author, msg);
}

void GameNetworkAdapter::handleMove(const net::Envelope &env) {
    std::string jsonString;
    google::protobuf::util::MessageToJsonString(env, &jsonString);

    auto move = MoveProtoFactory::fromProto(env.move());
    if (move)
        emit remoteMoveReceived(move.release());
    else {
        std::string jsonString;
        google::protobuf::util::MessageToJsonString(env, &jsonString);
        qDebug() << "Received invalid move: \n" <<  jsonString.c_str();
        onError("Client Received invalid move");
    }
}
