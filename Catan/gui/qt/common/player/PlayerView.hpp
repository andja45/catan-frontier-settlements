//
// Created by matija on 1/30/26.
//

#ifndef CATAN_PLAYERVIEW_HPP
#define CATAN_PLAYERVIEW_HPP
#include <common/cards/QCard.h>
#include <common/panels/FloatingPanel.h>
#include <common/player/PlayerDot.h>
#include <player/Player.h>

class PlayerView: public FloatingPanel {
    Q_OBJECT

public:
    PlayerView(Player* player,QWidget* parent);

private:
    Player* m_player;

    QCard* m_resourceCard;
    QCard* m_devCard;
    QCountBadge* m_vpBadge;
    QCountBadge* m_knBadge;
    QCountBadge* m_roadBadge;

    bool m_bestRoad=false;
    bool m_bestKnights=false;
    bool m_bestPoints=true;
    TextInDot * m_pointsDot;

public slots:
    void setBestRoad(bool bestRoad);
    void setBestKnights(bool bestKnights);
    void setBestPoints(bool bestPoints);
    void refresh();

    void buildUi();

};

#endif //CATAN_PLAYERVIEW_HPP