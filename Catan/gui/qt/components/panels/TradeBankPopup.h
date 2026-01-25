#ifndef TRADEBANKPOPUP_H
#define TRADEBANKPOPUP_H
#include"../../game/player/Player.h"
#include "TradePopup.h"
class TradeBankPopup: public FloatingPanel
{Q_OBJECT
public:
    explicit TradeBankPopup(Player* player, QWidget* parent);

    TradeOffer offer() const;

signals:
    void tradeSubmitted(const TradeOffer& offer);

private:
    std::map<ResourceType,int> m_playerResources = {
        {ResourceType::Wood, 8},
        {ResourceType::Brick,2},
        {ResourceType::Wool, 3},
        {ResourceType::Ore,1},
        {ResourceType::Wheat,2}
    };
    std::map<ResourceType,int> m_tradeRates = {
        {ResourceType::Wood, 4},
        {ResourceType::Brick,2},
        {ResourceType::Wool, 2},
        {ResourceType::Ore,4},
        {ResourceType::Wheat,4}
    };
    Player* m_player;
    TradeOffer m_offer;
    bool m_validGive = false;
    std::vector<QCard*> m_giveCards;
    std::vector<QCard*> m_receiveCards;
    QHBoxLayout* m_ratesBox;
    std::map<ResourceType, QLabel*> m_ratesLables;
    QCardRow* m_giveRow;
    QCardRow* m_receiveRow;
    QPushButton* m_sendButton;
    bool isOfferValid() const;
    void updateTradeRates();
    std::map<ResourceType, int> selectedResources(std::vector<QCard*> cards) const;
};
#endif // TRADEBANKPOPUP_H
