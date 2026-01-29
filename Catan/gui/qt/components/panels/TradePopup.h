#ifndef TRADEPOPUP_H
#define TRADEPOPUP_H

#include"../cards/QCardRow.h"
#include"../../game/types/ResourceType.h"
#include<QPushButton>
#include"FloatingPanel.h"
#include"../../common/GameTheme.h"
#include<array>

#include "player/Player.h"

struct TradeOffer {
    std::map<ResourceType,int> give;
    std::map<ResourceType,int> receive;
};

enum class TradePanelType {
    MakeTrade,
    OfferedTrade,
    BankTrade
};

class TradePopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit TradePopup(Player*,QWidget* parent = nullptr);

    TradeOffer offer() const;

signals:
    void tradeSubmitted(const TradeOffer& offer);

private:
    std::map<ResourceType,int> m_playerResources ;
    TradeOffer m_offer;

    int m_givesSelected= 0;
    int m_receivesSelected =0;
    std::vector<QCard*> m_giveCards;
    std::vector<QCard*> m_receiveCards;
    QCardRow* m_giveRow;
    QCardRow* m_receiveRow;
    QPushButton* m_sendButton;
    bool isOfferValid() const;
    std::map<ResourceType, int> selectedResources(std::vector<QCard*> cards) const;
    void paintEvent(QPaintEvent*);
    void resetState();
    void closeEvent(QCloseEvent* e);
};

#endif // TRADEPOPUP_H
