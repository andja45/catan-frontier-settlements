//
// Created by matija on 1/24/26.
//

#include "ClientConnection.hpp"

ClientConnection::ClientConnection(NetworkTransport* socket, QObject* parent)
    : QObject(parent), m_envelopeTransporter(socket)
{
    connect(m_envelopeTransporter, &NetworkTransport::envelopeReceived,
            this, &ClientConnection::onEnvelope);

    connect(m_envelopeTransporter, &NetworkTransport::disconnected,
            this, &ClientConnection::onDisconnected);

    connect(m_envelopeTransporter, &NetworkTransport::errored,
            this, &ClientConnection::onError);
}

void ClientConnection::send(const net::Envelope& env)
{
    m_envelopeTransporter->sendEnvelope(env);}

void ClientConnection::onEnvelope(net::Envelope env) {
    emit envelopeReceived(this, env);
}

void ClientConnection::onDisconnected() {
    emit disconnected(this);
}

void ClientConnection::onError() {
    emit errored(this);
}

