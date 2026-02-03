//
// Created by matija on 1/24/26.
//

#ifndef CATAN_LOBBYCONTROLLER_HPP
#define CATAN_LOBBYCONTROLLER_HPP
#include <LobbyNetworkAdapter.hpp>
#include <QObject>
#include <screens/lobby/LobbyView.h>

class LobbyController : public QObject{
    Q_OBJECT
public:
    LobbyController(LobbyView* view, NetworkTransport* transport);

public slots:
    void onGameRequested(const GameConfig &config, const std::string &boardPath = "");
signals:
    void gameStarted(const GameConfig& config, Board* board, PlayerId myId, int seed);
    void lobbyClosed();
private:
    LobbyView* m_view;
    LobbyNetworkAdapter* m_transport;
    void onGameStarted(PlayerId myId, int32_t gameSeed, const GameConfig& config, Board* board);
};

#endif //CATAN_LOBBYCONTROLLER_HPP