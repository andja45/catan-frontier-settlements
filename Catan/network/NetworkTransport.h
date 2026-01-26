//
// Created by jovan on 24/01/2026.
//

#ifndef CATAN_NETWORKTRANSPORT_H
#define CATAN_NETWORKTRANSPORT_H

#pragma once
#include <QObject>
#include <QTcpSocket>
#include "MessageFramer.h"
#include "proto/envelope.pb.h"

class NetworkTransport : public QObject {
    Q_OBJECT
  public:
    explicit NetworkTransport(QObject* parent = nullptr);

    void connectTo(const QHostAddress& addr, uint16_t port);
    void sendEnvelope(const net::Envelope& env);

    signals:
      void envelopeReceived(const net::Envelope& env);

private:
    QTcpSocket socket;
    MessageFramer framer;
};


#endif //CATAN_NETWORKTRANSPORT_H