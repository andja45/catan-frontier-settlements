//
// Created by jovan on 24/01/2026.
//

#ifndef CATAN_GAMENETWORK_H
#define CATAN_GAMENETWORK_H

#pragma once
#include <QObject>
#include <QHostAdress>
#include <memory>
#include "NetworkTransport.h"
#include "move/Move.h"

class GameNetwork : public QObject {
    Q_OBJECT
  public:
    explicit GameNetwork(QObject* parent = nullptr);

    void connectToServer(const QHostAddress&, uint16_t);
    void sendMove(const Move& move);

    signals:
      void remoteMoveReceived(std::unique_ptr<Move> move);

private:
    NetworkTransport transport;
    uint32_t nextSeq = 1;

    net::Envelope wrapMove(const Move& move);
    void onEnvelope(const net::Envelope& env);
};


#endif //CATAN_GAMENETWORK_H