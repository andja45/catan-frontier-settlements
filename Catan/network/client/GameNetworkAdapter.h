//
// Created by jovan on 24/01/2026.
//

#ifndef CATAN_GAMENETWORK_H
#define CATAN_GAMENETWORK_H

#include <QObject>
#include <memory>
#include <NetworkTransport.h>
#include <QtNetwork/QHostAddress>
#include "move/Move.h"
#include "ProtoHeaders.hpp"

class GameNetworkAdapter : public QObject {
    Q_OBJECT
public:
    explicit GameNetworkAdapter(QObject* parent = nullptr);
    void sendMove(const Move& move);
    void setTransport(std::unique_ptr<NetworkTransport> transport);
    void sendMessage(const std::string& message);

signals:
    void remoteMoveReceived(std::unique_ptr<Move> move);
    void remoteMessageReceived(const std::string& message);

private:
    std::unique_ptr<NetworkTransport> m_transport;

    net::Envelope wrapMove(const Move& move) const;
    net::Envelope wrapMessage(const std::string& message) const;

    void onEnvelope(const net::Envelope& env);
    void handleMessage(const net::Envelope& env); //no?
    void handleMove(const net::Envelope& env);

};


#endif //CATAN_GAMENETWORK_H