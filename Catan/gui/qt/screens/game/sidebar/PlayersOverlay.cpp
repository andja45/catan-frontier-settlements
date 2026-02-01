//
// Created by matija on 1/30/26.
//

#include "PlayersOverlay.hpp"

#include <QVBoxLayout>
#include <common/player/PlayerDetailedView.hpp>
#include <common/player/PlayerView.hpp>
#include <player/Bank.h>
#include <player/Player.h>

PlayersOverlay::PlayersOverlay(std::vector<Player*>players,Bank* bank,PlayerId activePlayerId,QWidget *parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    auto *stackBox = new QVBoxLayout(this);
    stackBox->setContentsMargins(0, 0, 0, 0);
    stackBox->setSpacing(10);

    m_bankView= new PlayerDetailedView(bank,this);
    stackBox->addWidget(m_bankView);

    m_playerViews = std::vector<PlayerView*>();
    for (auto*player : players) {
        auto* pv = new PlayerView(player,this);
        m_playerViews.push_back(pv);
        stackBox->addWidget(pv);
    }

    m_activePlayerView = new PlayerDetailedView(players[activePlayerId],this);
    stackBox->addWidget(m_activePlayerView);
    stackBox->addStretch(1);

    // auto* t = new QTimer(this);
    // connect(t, &QTimer::timeout, this, &PlayersOverlay::refreshAll);
    // t->start(100);
}
