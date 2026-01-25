#ifndef TRADEOFFEREDPOPUP_H
#define TRADEOFFEREDPOPUP_H
#include"TradePopup.h"

class TradeOfferedPopup : public FloatingPanel
{
    Q_OBJECT
public:
    explicit TradeOfferedPopup(TradeOffer offer, QWidget* parent = nullptr);

signals:
    void tradeAccepted(const TradeOffer& offer);
    void tradeDeclined(const TradeOffer& offer);

private:
    std::map<ResourceType,int> m_playerResources = {
        {ResourceType::Wood, 4},
        {ResourceType::Brick,2},
        {ResourceType::Wool, 3},
        {ResourceType::Ore,1},
        {ResourceType::Wheat,2}
    };
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
