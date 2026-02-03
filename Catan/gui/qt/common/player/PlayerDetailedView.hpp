//
// Created by matija on 1/30/26.
//

#ifndef CATAN_PLAYERDETAILEDVIEW_HPP
#define CATAN_PLAYERDETAILEDVIEW_HPP
#include <common/cards/QCardRow.h>
#include <common/panels/FloatingPanel.h>
#include <player/Player.h>

class PlayerDetailedView : public FloatingPanel{
    Q_OBJECT

private:
    ResourceHolder* m_player;
    std::vector<QCard*> m_cards;

public:
    PlayerDetailedView(ResourceHolder *m_player,QWidget *parent);

public slots:
    void refresh();
private:
    void buildUi();

};

#endif //CATAN_PLAYERDETAILEDVIEW_HPP