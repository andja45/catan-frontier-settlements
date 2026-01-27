//
// Created by matija on 1/24/26.
//

#include "ServerController.hpp"

ServerController::ServerController(QObject* parent)
    : QObject(parent),
      m_dispatcher(m_roomManager)
{
    connect(&m_server, &QTcpServer::newConnection,
            this, &ServerController::onNewConnection);
}

void ServerController::listen(quint16 port) {
    connect(&m_server, &QTcpServer::newConnection,
            this, &ServerController::onNewConnection);
    m_server.listen(QHostAddress::Any, port);
}

void ServerController::onNewConnection()
{
    auto* socket = m_server.nextPendingConnection(); //!
    NetworkTransport* socketAdapter = new NetworkTransport(this);
    socketAdapter->setSocket(socket);
    auto* conn = new ClientConnection(socketAdapter, this);

    m_clients.insert(conn);

    connect(conn, &ClientConnection::envelopeReceived,
            this, &ServerController::onEnvelope);

    connect(conn, &ClientConnection::disconnected,
            this, &ServerController::onDisconnected);
    connect(conn, &ClientConnection::errored,
            this, &ServerController::onDisconnected); //!

}

void ServerController::onEnvelope(ClientConnection* c, net::Envelope& env)
{
    m_dispatcher.dispatch(c, env);
}

void ServerController::onDisconnected(ClientConnection *client) {
    if (client->hasRoom())
        m_roomManager.handleDisconnect(client);
}
