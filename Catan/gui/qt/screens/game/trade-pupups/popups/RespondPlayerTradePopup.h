#ifndef TRADEOFFEREDPOPUP_H
#define TRADEOFFEREDPOPUP_H
#include"screens/game/toolbar/tool-popups/RequestPlayerTradePopup.h"
#include "player/Player.h"
#include <QApplication>
#include <QScreen>
#include <algorithm>

class RespondPlayerTradePopup : public FloatingPanel
{
    Q_OBJECT
public:
    RespondPlayerTradePopup(Player *player, TradeOffer offer, TradeId tradeId, bool checked, QWidget *parent);

signals:
    void tradeAccepted(PlayerId, TradeId);
    void tradeDeclined(PlayerId, TradeId);

private:
    std::map<ResourceType,int> m_playerResources ;
    TradeOffer m_offer;
    TradeId m_tradeId;
    PlayerId m_playerId;
    QCardRow* m_giveRow;

    std::vector<QCard*> m_accepterReceives;
    std::vector<QCard*> m_accepterGives;
    QCardRow* m_receiveRow;
    QPushButton* m_acceptButton;
    QPushButton* m_declineButton;


    bool isOfferAcceptable() const;

    void paintEvent(QPaintEvent*);
};

#endif // TRADEOFFEREDPOPUP_H
