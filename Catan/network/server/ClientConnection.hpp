//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CLIENTCONNECTION_HPP
#define CATAN_CLIENTCONNECTION_HPP

#include <NetworkTransport.h>
#include <QObject>
#include <QTcpSocket>
#include <ServerTypes.hpp>

#include "ProtoHeaders.hpp"

class ClientConnection : public QObject {
    Q_OBJECT
public:
    explicit ClientConnection(NetworkTransport* socket, QObject* parent = nullptr);

    void send(const net::Envelope& env);
    void ack();
    void sendError(const std::string& error);

    bool hasRoom() const { return m_roomId.has_value(); }
    std::string roomId() const { return m_roomId.value(); }
    void setRoomId(std::optional<std::string> name) { m_roomId = name; }
    void setPlayerId(int id) { m_playerId = id; }
    bool isHost() const { return m_isHost; }
    void setHost(bool isHost=true) { m_isHost = isHost; }
    std::string name() const { return m_name; }
    void setName(std::string name) { m_name = name; }
    int playerId() const { return m_playerId; }
    void setStatus(ClientStatus status) { m_status = status; }
    ClientStatus status() const { return m_status; }
    bool wasActive() const { return m_wasActive; }
    void resetActive() { m_wasActive=false; }
    bool isConnected() const { return m_envelopeTransporter->isConnected(); }
    void sendDisconnect();

signals:
    void envelopeReceived(ClientConnection*, const net::Envelope&);
    void disconnected(ClientConnection*);
    void softErrored(ClientConnection*, const std::string&);
public slots:
    void onEnvelope(const net::Envelope& env);
    void onDisconnected();
    void onSoftError(const std::string&);


private:
    NetworkTransport* m_envelopeTransporter;
    std::optional<std::string> m_roomId;
    int m_playerId;
    std::string m_name;
    bool m_isHost;
    ClientStatus m_status;
    bool m_wasActive=false;
    TimeStamp m_lastActive;

};

#endif //CATAN_CLIENTCONNECTION_HPP