//
// Created by matija on 1/31/26.
//

#ifndef CATAN_ACCEPTPLAYERTRADEPOPUP_HPP
#define CATAN_ACCEPTPLAYERTRADEPOPUP_HPP
#include <common/panels/FloatingPanel.h>
#include <screens/game/toolbar/tool-popups/RequestPlayerTradePopup.h>

class Player;

class AcceptPlayerTradePopup : public FloatingPanel
{
    Q_OBJECT
public:
    explicit AcceptPlayerTradePopup(Player *m_player, TradeOffer offer,TradeId tradeId, QWidget *parent);
    void setPlayers(std::vector<Player *> players);
signals:
    void tradeAccepted(TradeId,PlayerId);
    void tradeCancelled(TradeId);
private:
    TradeOffer m_offer;
    TradeId m_tradeId;
    PlayerId m_playerId;

    std::vector<Player*> m_players;
    QHBoxLayout* m_buttonsBox;

    void rebuild();

    void paintEvent(QPaintEvent*);
};


#endif //CATAN_ACCEPTPLAYERTRADEPOPUP_HPP