//
// Created by jovan on 24/01/2026.
//

#ifndef CATAN_NETWORKTRANSPORT_H
#define CATAN_NETWORKTRANSPORT_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork/QHostAddress>

#include "MessageFramer.h"
#include "ProtoHeaders.hpp"


class NetworkTransport : public QObject {
    Q_OBJECT
  public:
    explicit NetworkTransport(QObject* parent = nullptr);
    void sendEnvelope(const net::Envelope& env);
    void setSocket(QTcpSocket* socket);
    QTcpSocket* socket() const { return m_socket; }
    void sendError(const std::string& error);
    void sendAck();
    QTcpSocket::SocketState state() const;
    bool isConnected() const;
    void disconnectSocket();
    signals:
        void envelopeReceived(const net::Envelope& env);

        void disconnected(); // just propagate
        void protocolError(const std::string&);


private:
    int32_t m_nextSeqToSend = 0;
    int32_t m_nextSeqToReceive = 0;

    QTcpSocket* m_socket;
    MessageFramer m_framer;
};



#endif //CATAN_NETWORKTRANSPORT_H
