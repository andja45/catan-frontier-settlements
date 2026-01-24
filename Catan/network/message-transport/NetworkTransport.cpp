//
// Created by jovan on 24/01/2026.
//

#include "../NetworkTransport.h"
#include <QHostAddress>

NetworkTransport::NetworkTransport(QObject* parent) : QObject(parent)
{
    connect(&socket, &QTcpSocket::readyRead, this, [this]() {
      framer.onBytes(socket.readAll());
    });

    framer.setMessageCallback([this](const QByteArray& msg) {
      net::Envelope env;
      if (!env.ParseFromArray(msg.data(), msg.size()))
          return;

        emit envelopeReceived(env);
      });
}

void NetworkTransport::connectTo(const QHostAddress& addr, uint16_t port) {
    socket.connectToHost(addr, port);
}

void NetworkTransport::sendEnvelope(const net::Envelope& env) {
    std::string bytes;
    env.SerializeToString(&bytes);

    QByteArray payload(bytes.data(), bytes.size());
    QByteArray framed = MessageFramer::frameMessage(payload);

    socket.write(framed);
}
