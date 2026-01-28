#ifndef TRADEOFFEREDPOPUP_H
#define TRADEOFFEREDPOPUP_H
#include"TradePopup.h"
#include "player/Player.h"

class TradeOfferedPopup : public FloatingPanel
{
    Q_OBJECT
public:
    explicit TradeOfferedPopup(Player *m_player, TradeOffer offer, QWidget *parent);

signals:
    void tradeAccepted(const TradeOffer& offer);
    void tradeDeclined(const TradeOffer& offer);

private:
    std::map<ResourceType,int> m_playerResources ;
    TradeOffer m_offer;
    QCardRow* m_giveRow;

    std::vector<QCard*> m_giveCards;
    std::vector<QCard*> m_receiveCards;
    QCardRow* m_receiveRow;
    QPushButton* m_acceptButton;
    QPushButton* m_declineButton;

    bool isOfferAcceptable() const;
    std::map<ResourceType, int> selectedResources(std::vector<QCard*> cards) const;
};

#endif // TRADEOFFEREDPOPUP_H
