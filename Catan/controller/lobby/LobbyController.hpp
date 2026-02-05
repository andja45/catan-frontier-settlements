//
// Created by matija on 1/24/26.
//

#ifndef CATAN_LOBBYCONTROLLER_HPP
#define CATAN_LOBBYCONTROLLER_HPP
#include <LobbyNetworkAdapter.hpp>
#include <QObject>
#include <screens/lobby/LobbyView.h>
#include <screens/messageservice/MessageIntent.hpp>

class LobbyController : public QObject{
    Q_OBJECT
public:
    LobbyController(LobbyView* view, NetworkTransport* transport);

public slots:
    void onGameRequested(const GameConfig &config, const std::string &boardPath = "");
signals:
    void gameStarted(PlayerId myId, int seed,const GameConfig& config, Board* board);
    void lobbyClosed();
private:
    LobbyView* m_view;
    LobbyNetworkAdapter* m_transport;
};

#endif //CATAN_LOBBYCONTROLLER_HPP