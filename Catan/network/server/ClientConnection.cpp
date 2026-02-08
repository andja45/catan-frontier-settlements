//
// Created by matija on 1/24/26.
//

#include "ClientConnection.hpp"

ClientConnection::ClientConnection(NetworkTransport* socket, QObject* parent)
    : QObject(parent), m_envelopeTransporter(socket)
{
    m_wasActive=true;
    m_lastActive= Clock::now();
    m_roomId=std::nullopt;

    socket->setParent(this);
    connect(m_envelopeTransporter, &NetworkTransport::envelopeReceived,
            this, &ClientConnection::onEnvelope);

    connect(m_envelopeTransporter, &NetworkTransport::disconnected,
            this, &ClientConnection::onDisconnected);

    connect(m_envelopeTransporter, &NetworkTransport::protocolError,
            this, &ClientConnection::onSoftError);
}

void ClientConnection::send(const net::Envelope& env)
{
    m_wasActive=true;
    m_envelopeTransporter->sendEnvelope(env);
}

void ClientConnection::ack() {
    m_envelopeTransporter->sendAck();
}

void ClientConnection::sendError(const std::string &error) {
    m_envelopeTransporter->sendError(error);
}

void ClientConnection::onEnvelope(const net::Envelope& env) {
    emit envelopeReceived(this, env);
}

void ClientConnection::onDisconnected() {
    emit disconnected(this);
}

void ClientConnection::onSoftError(const std::string &err) {
    emit softErrored(this,err);
}

void ClientConnection::sendDisconnect() {
    m_envelopeTransporter->disconnectSocket();
}
