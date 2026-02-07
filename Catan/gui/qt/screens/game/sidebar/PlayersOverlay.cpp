//
// Created by matija on 1/30/26.
//

#include "PlayersOverlay.hpp"

#include <QVBoxLayout>
#include <common/player/PlayerDetailedView.hpp>
#include <common/player/PlayerView.hpp>
#include <common/theme/GameTheme.h>
#include <player/Bank.h>
#include <player/Player.h>

PlayersOverlay::PlayersOverlay(std::vector<Player*>players,Bank* bank,PlayerId activePlayerId,QWidget *parent):m_players(players) {
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    auto *stackBox = new QVBoxLayout(this);
    stackBox->setContentsMargins(0, 0, 0, 0);
    stackBox->setSpacing(10);

    m_bankView= new PlayerDetailedView(bank,QString::fromStdString("Bank"),this);
    stackBox->addWidget(m_bankView);

    m_playerPanels =  std::vector<PlayerView*>();
    for (auto*player : players) {
        auto* pv = new PlayerView(player,this);
        m_playerPanels.push_back(pv);
        stackBox->addWidget(pv);
    }

    m_localPlayerView = new PlayerDetailedView(players[activePlayerId],"You",this,true,GameTheme::getPlayerColor(activePlayerId));

    stackBox->addWidget(m_localPlayerView);
    stackBox->addStretch(1);

    // auto* t = new QTimer(this);
    // connect(t, &QTimer::timeout, this, &PlayersOverlay::refreshAll);
    // t->start(100);
    m_activePlayerId=0;
    refreshTurnGlow();
    adjustSize();
    setMinimumSize(sizeHint());
}

void PlayersOverlay::refreshTurnGlow()
{
    for (auto p:m_playerPanels) {
        p->setActiveHighlight(false);
    }

    if (m_activePlayerId != -1)
        m_playerPanels[m_activePlayerId]->setActiveHighlight(true);
}

void PlayersOverlay::update(PlayerId pid) {
    m_activePlayerId=pid;
    refreshTurnGlow();
    for (auto p:m_playerPanels) {
        p->refresh();
    }
    m_localPlayerView->refresh();
    m_bankView->refresh();
    QWidget::update();
}
