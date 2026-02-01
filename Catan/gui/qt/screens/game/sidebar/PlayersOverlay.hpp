//
// Created by matija on 1/30/26.
//

#ifndef CATAN_PLAYERSOVERLAY_HPP
#define CATAN_PLAYERSOVERLAY_HPP
#include <vector>
#include <common/player/PlayerDetailedView.hpp>
#include <common/player/PlayerView.hpp>
#include <player/Bank.h>
#include <player/Player.h>
#include <screens/game/toolbar/buttons/DiceWidget.h>


class PlayersOverlay :public QWidget {
    Q_OBJECT

public:
    PlayersOverlay(std::vector<Player *> players, Bank *bank, PlayerId activePlayerId, QWidget *parent);

    PlayerDetailedView * m_bankView;
    PlayerDetailedView * m_activePlayerView;
    std::vector<PlayerView*> m_playerViews;
};

#endif //CATAN_PLAYERSOVERLAY_HPP