//
// Created by matija on 1/24/26.
//

#ifndef CATAN_SERVERCONTROLLER_HPP
#define CATAN_SERVERCONTROLLER_HPP

#include <MessageDispatcher.hpp>
#include <QObject>
#include <QTcpServer>
#include <unordered_set>
#include "ClientConnection.hpp"
#include "RoomManager.hpp"

class ServerController : public QObject {
    Q_OBJECT
public:
    explicit ServerController(QObject* parent = nullptr);

    void listen(quint16 port);

private slots:
    void onNewConnection();
    void onEnvelope(ClientConnection *, const net::Envelope &env);
    void onDisconnected(ClientConnection*);
    void onErrored(ClientConnection *, const std::string &error);

private:
    QTcpServer* m_server;

    std::unordered_set<ClientConnection*> m_clients;

    RoomManager m_roomManager;
    MessageDispatcher m_dispatcher;
};

#endif //CATAN_SERVERCONTROLLER_HPP