//
// Created by matija on 1/25/26.
//

#include "HostJoinNetworkAdapter.hpp"

net::Envelope HostJoinNetworkAdapter::wrapHost(std::string gameName, std::string hostName) {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    env.set_seq(m_transport->getNextSeqToSend());
    env.mutable_setup()->mutable_host_request()->set_game_name(gameName);
    env.mutable_setup()->mutable_host_request()->set_player_name(hostName);
    return env;
}

net::Envelope HostJoinNetworkAdapter::wrapJoin(std::string gameName, std::string playerName) {
    net::Envelope env;
    env.set_msg_type(net::MSG_SETUP);
    env.set_seq(m_transport->getNextSeqToSend());    *env.mutable_setup()->mutable_join_request();
    env.mutable_setup()->mutable_join_request()->set_game_name(gameName);
    env.mutable_setup()->mutable_join_request()->set_player_name(playerName);
    return env;
}

void HostJoinNetworkAdapter::connectTo(const std::string &addr, std::uint16_t port) {
    m_transport->connectTo(addr, port);
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
    if (env.msg_type() == net::MSG_SETUP) {
        auto protoSetup = env.setup();
        if (protoSetup.type() == net::Setup_MessageType_ACK)
            handleAcceptResponse(env);
        if (protoSetup.type() == net::Setup_MessageType_ERROR)
            handleRejectResponse(env);
    }
}

void HostJoinNetworkAdapter::handleAcceptResponse(const net::Envelope &env) {
    emit acceptReceived();
}

void HostJoinNetworkAdapter::handleRejectResponse(const net::Envelope &env) {
    emit rejectReceived(env.setup().error().message());
}
