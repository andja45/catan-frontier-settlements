#ifndef TRADEBANKPOPUP_H
#define TRADEBANKPOPUP_H
#include"../../game/player/Player.h"
#include "TradePopup.h"
class TradeBankPopup: public FloatingPanel
{Q_OBJECT
public:
    explicit TradeBankPopup(Player* player, QWidget* parent);


signals:
    void tradeSubmitted(const TradeOffer& offer);

private:
    std::map<ResourceType,int> m_playerResources ;
    std::map<ResourceType,int> m_tradeRates ;
    Player* m_player;
    TradeOffer m_offer;
    bool m_validGive = false;
    ResourceType m_selectedGive = ResourceType::None;
    ResourceType m_selectedReceive = ResourceType::None;
    std::vector<QCard*> m_giveCards;
    std::vector<QCard*> m_receiveCards;
    QHBoxLayout* m_ratesBox;
    std::map<ResourceType, QLabel*> m_ratesLables;
    QCardRow* m_giveRow;
    QCardRow* m_receiveRow;
    QPushButton* m_sendButton;
    void updateTradeRates();
    std::map<ResourceType, int> selectedResources(std::vector<QCard*> cards) const;
    void paintEvent(QPaintEvent*);
    void resetState();
    void closeEvent(QCloseEvent* e);
};
#endif // TRADEBANKPOPUP_H
