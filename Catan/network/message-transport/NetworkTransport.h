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
    void sendError(const std::string& error);
    void sendAck();
    QTcpSocket::SocketState state() const;
    signals:
        void envelopeReceived(const net::Envelope& env);

        void disconnected(); // just propagate
        void errored(const std::string&);


private:
    int32_t m_nextSeqToSend = 0;
    int32_t m_nextSeqToReceive = 0;

    QTcpSocket* m_socket;
    MessageFramer m_framer;
};



#endif //CATAN_NETWORKTRANSPORT_H
