//
// Created by jovan on 24/01/2026.
//

#include "NetworkTransport.h"
#include <QHostAddress>

NetworkTransport::NetworkTransport(QObject* parent) : QObject(parent)
{
    connect(m_socket, &QTcpSocket::readyRead, this, [this]() {
      m_framer.onBytes(m_socket->readAll());
    });

    m_framer.setMessageCallback([this](const QByteArray& msg) {
        net::Envelope env;
        if (!env.ParseFromArray(msg.data(), msg.size()))
          return;
        if (env.seq()!= m_nextSeqToReceive) {
            emit errored();
        }
        else {
            m_nextSeqToReceive++;
            emit envelopeReceived(env);
        }
    });
}

void NetworkTransport::connectTo(const std::string &addr, uint16_t port) {
    connectTo(QHostAddress(QString::fromStdString(addr)), port);
}

void NetworkTransport::connectTo(const QHostAddress& addr, uint16_t port) const {
    m_socket->connectToHost(addr, port);
}

void NetworkTransport::sendEnvelope(const net::Envelope& env) {
    std::string bytes;
    env.SerializeToString(&bytes);

    QByteArray payload(bytes.data(), bytes.size());
    QByteArray framed = MessageFramer::frameMessage(payload);

    m_socket->write(framed);
}
