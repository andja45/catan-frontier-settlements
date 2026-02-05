//
// Created by matija on 1/25/26.
//

#include "HostJoinNetworkAdapter.hpp"

#include <google/protobuf/util/json_util.h>

net::Envelope HostJoinNetworkAdapter::wrapHost(std::string gameName, std::string hostName) {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    env.mutable_setup()->set_type(net::Setup_MessageType_HOST_REQUEST);
    env.mutable_setup()->mutable_host_request()->set_game_name(gameName);
    env.mutable_setup()->mutable_host_request()->set_player_name(hostName);
    return env;
}

net::Envelope HostJoinNetworkAdapter::wrapJoin(std::string gameName, std::string playerName) {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    env.mutable_setup()->set_type(net::Setup_MessageType_JOIN_REQUEST);

    *env.mutable_setup()->mutable_join_request();
    env.mutable_setup()->mutable_join_request()->set_game_name(gameName);
    env.mutable_setup()->mutable_join_request()->set_player_name(playerName);
    return env;
}

void HostJoinNetworkAdapter::setTransport(NetworkTransport *t) {
    m_transport=t;
    connect(m_transport, &NetworkTransport::envelopeReceived,
            this, &HostJoinNetworkAdapter::onEnvelope);
}


void HostJoinNetworkAdapter::sendHost(std::string gameName, std::string hostName) {
    auto env=wrapHost(gameName, hostName);
    m_transport->sendEnvelope(env);
}

void HostJoinNetworkAdapter::sendJoin(std::string gameName, std::string playerName) {
    auto env=wrapJoin(gameName, playerName);
    m_transport->sendEnvelope(env);

}

void HostJoinNetworkAdapter::onEnvelope(const net::Envelope &env) {
    std::string jsonString;
    google::protobuf::util::MessageToJsonString(env, &jsonString);
    qDebug() << "[Host-join] Received message: \n" << jsonString.c_str();

    if (env.msg_type() == net::MSG_SETUP) {
        auto protoSetup = env.setup();
        if (protoSetup.type() == net::Setup_MessageType_ACCEPT) {
            handleAcceptResponse(env);
            return;
        }
        if (protoSetup.type() == net::Setup_MessageType_REJECT) {
            handleRejectResponse(env);
            return;
        }
    }
    google::protobuf::util::MessageToJsonString(env, &jsonString);
    qDebug() << "Received unknown message: \n" << jsonString.c_str();
}

void HostJoinNetworkAdapter::handleAcceptResponse(const net::Envelope &env) {
    emit acceptReceived();
}

void HostJoinNetworkAdapter::handleRejectResponse(const net::Envelope &env) {
    emit rejectReceived(env.setup().error().message());
}
