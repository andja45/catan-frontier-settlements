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
    QString m_text;
    QColor m_color;
    bool m_border;

    void buildUi();

public:
    PlayerDetailedView(ResourceHolder *player, QString text, QWidget *parent, bool border=false,QColor col=Qt::black);

    void paintEvent(QPaintEvent *event) override;
public slots:


    void refresh();

};

#endif //CATAN_PLAYERDETAILEDVIEW_HPP