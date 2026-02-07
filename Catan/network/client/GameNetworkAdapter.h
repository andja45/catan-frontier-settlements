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
    void sendMove(const Move* move);
    void setTransport(NetworkTransport *transport);
    void sendMessage(const std::string &author, const std::string &message);

    void sendReady();

    void onError(const std::string& error);
    void onDisconnected();
signals:
    void remoteMoveReceived(Move* move);
    void remoteMessageReceived(const std::string& author, const std::string& message);

    void disconnected();
    void errored(const std::string&);

    void startGame();
private:
    NetworkTransport* m_transport;

    net::Envelope wrapMove(const Move& move) const;
    net::Envelope wrapMessage(const std::string &author, const std::string &message) const;

    void handleAck(const net::Envelope & env);

    void onEnvelope(const net::Envelope& env);
    void handleMessage(const net::Envelope& env); //no?
    void handleMove(const net::Envelope& env);

};


#endif //CATAN_GAMENETWORK_H