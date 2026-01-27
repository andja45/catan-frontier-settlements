//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CLIENTCONNECTION_HPP
#define CATAN_CLIENTCONNECTION_HPP

#include <NetworkTransport.h>
#include <QObject>
#include <QTcpSocket>
#include "ProtoHeaders.hpp"

class ClientConnection : public QObject {
    Q_OBJECT
public:
    explicit ClientConnection(NetworkTransport* socket, QObject* parent = nullptr);

    void send(const net::Envelope& env);

    bool hasRoom() const { return m_roomId.has_value(); }
    std::string roomId() const { return m_roomId.value(); }
    void setRoomId(std::string name) { m_roomId = name; }
    void setPlayerId(int id) { m_playerId = id; }
    bool isHost() const { return m_isHost; }
    void setHost() { m_isHost = true; }
    std::string name() const { return m_name; }
    void setName(std::string name) { m_name = name; }
    int playerId() const { return m_playerId; }

signals:
    void envelopeReceived(ClientConnection*, net::Envelope&);
    void disconnected(ClientConnection*);
    void errored(ClientConnection*);
    
private slots:
    void onEnvelope(net::Envelope env);
    void onDisconnected();
    void onError();

private:
    NetworkTransport* m_envelopeTransporter;
    std::optional<std::string> m_roomId;
    int m_playerId;
    std::string m_name;
    bool m_isHost;
};

#endif //CATAN_CLIENTCONNECTION_HPP