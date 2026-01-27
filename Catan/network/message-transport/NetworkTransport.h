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
    void connectTo(const std::string& addr, uint16_t port);
    void connectTo(const QHostAddress& addr, uint16_t port) const;
    void sendEnvelope(const net::Envelope& env);
    void setSocket(QTcpSocket* socket) { this->m_socket = socket; }

    int32_t getNextSeqToSend() { return m_nextSeqToSend++; }

    signals:
        void envelopeReceived(const net::Envelope& env);
        void disconnected();
        void errored(); // TODO

private:
    int32_t m_nextSeqToSend = 0;
    int32_t m_nextSeqToReceive = 0;

    QTcpSocket* m_socket;
    MessageFramer m_framer;
};



#endif //CATAN_NETWORKTRANSPORT_H
