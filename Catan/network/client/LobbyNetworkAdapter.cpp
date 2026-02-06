//
// Created by matija on 1/24/26.
//

#include "LobbyNetworkAdapter.hpp"

#include <NetworkTransport.h>
#include <board/BoardFactory.hpp>
#include <board/BoardSerializer.hpp>
#include <config/ConfigFactory.hpp>
#include <config/ConfigSerializer.hpp>
#include <google/protobuf/util/json_util.h>

LobbyNetworkAdapter::LobbyNetworkAdapter(QObject *parent) {
    m_transport=nullptr;
}

void LobbyNetworkAdapter::sendLeave() const {
    m_transport->sendEnvelope(wrapLeave());
}

void LobbyNetworkAdapter::sendConfig(const GameConfig &config) const {
    net::Envelope env = wrapConfig(config);
    m_transport->sendEnvelope(env);
}

void LobbyNetworkAdapter::sendStartRequest(const Board &board) const {
    m_transport->sendEnvelope(wrapStartRequest(board));
}

void LobbyNetworkAdapter::sendJoined() {
    m_transport->sendAck();
}

void LobbyNetworkAdapter::onError(const std::string &error) {
    emit errored(error);
}

void LobbyNetworkAdapter::onDisconnected() {
    emit disconnected();
}

void LobbyNetworkAdapter::setTransport(NetworkTransport *transport) {
    m_transport=transport;
    connect(m_transport, &NetworkTransport::envelopeReceived,
                this, &LobbyNetworkAdapter::onEnvelope);
    connect(m_transport, &NetworkTransport::errored,
                this, &LobbyNetworkAdapter::onError);
    connect(m_transport, &NetworkTransport::disconnected,
                this, &LobbyNetworkAdapter::onDisconnected);
}


net::Envelope LobbyNetworkAdapter::wrapConfig(const GameConfig &config) const {
    net::Envelope env=prepareEnvelope();
    env.mutable_setup()->set_type(net::Setup_MessageType_GAME_CONFIG);

    *env.mutable_setup()->mutable_game_config()= ConfigSerializer::toProto(config);

    return env;
}

net::Envelope LobbyNetworkAdapter::wrapStartRequest(const Board &board) const {
    auto env=prepareEnvelope();
    env.mutable_setup()->set_type(net::Setup_MessageType_START_GAME_REQUEST);

    *env.mutable_setup()->mutable_start_game_request ()->mutable_board_info() = BoardSerializer::toProto(board);
    return env;

}

net::Envelope LobbyNetworkAdapter::wrapLeave() const {
    auto env=prepareEnvelope();
    *env.mutable_setup()->mutable_leave();
    return env;
}

net::Envelope LobbyNetworkAdapter::prepareEnvelope() const {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    return env;
}


void LobbyNetworkAdapter::onEnvelope(const net::Envelope &env) {
    std::string jsonString;
    google::protobuf::util::MessageToJsonString(env, &jsonString);
    qDebug() << "[Lobby] Received message: \n" << jsonString.c_str();

    if (env.msg_type() == net::MSG_SETUP) {
        auto protoSetup = env.setup();
        if (protoSetup.type() == net::Setup_MessageType_GAME_CONFIG)
            handleConfig(env);
        if (protoSetup.type()==net::Setup_MessageType_GAME_START)
            handleGameStarted(env);
    }
    else {
        std::string jsonString;
        google::protobuf::util::MessageToJsonString(env, &jsonString);

        qDebug() << "Received unknown message: " << jsonString.c_str();
        onError("Received unknown message");
    }
}

void LobbyNetworkAdapter::handleConfig(const net::Envelope &env) {
    auto protoConfig=env.setup().game_config();
    emit configReceived(ConfigFactory::fromProto(protoConfig));
}

void LobbyNetworkAdapter::handleGameStarted(const net::Envelope &env) {
    auto data=env.setup().game_start();
    auto myId=data.player_id();
    auto seed=data.seed();
    auto protoConfig=data.game_config();
    auto protoBoard=data.board_info();
    emit gameStarted(myId, seed, ConfigFactory::fromProto(protoConfig), BoardFactory::fromProto(protoBoard).release());
}
