//
// Created by matija on 1/24/26.
//

#include "ServerController.hpp"

#include <QTimer>
#include <google/protobuf/util/json_util.h>

ServerController::ServerController(QObject* parent)
    : QObject(parent), m_dispatcher(m_roomManager)
{
    m_server=new QTcpServer(this);

    // debug
    connect(m_server, &QTcpServer::newConnection, this, [] {
    qDebug() << "[Server] New incoming connection";
    });

    connect(m_server, &QTcpServer::newConnection,
            this, &ServerController::onNewConnection);


    QTimer *timer = new QTimer(this);
    timer->setInterval(TIME_LIMIT);

    connect(timer, &QTimer::timeout, this, [this] {
        m_roomManager.cleanEmptyRooms();
    });

    timer->start();

}

void ServerController::listen(quint16 port) {
    if (!m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "[Server] Listen failed:" << m_server->errorString();
    }
    qDebug() << "[Server] Listening on" << m_server->serverAddress().toString() << m_server->serverPort();
}

void ServerController::onNewConnection()
{
    auto* socket = m_server->nextPendingConnection();
    // for debug
    connect(socket, &QTcpSocket::connected, this, [socket] {
    qDebug() << "[Socket] Connected to"
             << socket->peerAddress().toString()
             << socket->peerPort();
    });
    connect(socket, &QTcpSocket::disconnected, this, [socket] {
        qDebug() << "[Socket] Disconnected from client";
    });
    connect(socket, &QTcpSocket::errorOccurred,
        this, [socket](QAbstractSocket::SocketError err) {
        qDebug() << "[Socket] Error:"
                 << err
                 << socket->errorString();
    });

    NetworkTransport* socketAdapter = new NetworkTransport(nullptr);
    socketAdapter->setSocket(socket);
    auto* conn = new ClientConnection(socketAdapter, this);

    m_clients.insert(conn);

    connect(conn, &ClientConnection::envelopeReceived,
            this, &ServerController::onEnvelope);
    connect(conn, &ClientConnection::disconnected,
            this, &ServerController::onDisconnected);
    connect(conn, &ClientConnection::errored,
            this, &ServerController::onErrored);
}

void ServerController::onEnvelope(ClientConnection* c, const net::Envelope& env)
{
    std::string jsonString;
    google::protobuf::util::MessageToJsonString(env, &jsonString);
    qDebug() << "Received message: \n" << jsonString.c_str();
    m_dispatcher.dispatch(c, env);
}

void ServerController::onDisconnected(ClientConnection *client) {
    if (client->hasRoom())
        m_roomManager.handleDisconnect(client);

    m_clients.erase(client);
    client->deleteLater();
}

void ServerController::onErrored(ClientConnection *client, const std::string &error) {
    if (client->hasRoom())
        m_roomManager.handleError(client,error);

    m_clients.erase(client);
    client->deleteLater();
}
