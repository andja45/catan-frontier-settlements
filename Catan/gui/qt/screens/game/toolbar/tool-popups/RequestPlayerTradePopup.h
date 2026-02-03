#ifndef TRADEPOPUP_H
#define TRADEPOPUP_H

#include "../../../../common/cards/QCardRow.h"
#include"../../game/types/ResourceType.h"
#include<QPushButton>
#include "../../../../common/panels/FloatingPanel.h"
#include "../../../../common/theme/GameTheme.h"
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

class RequestPlayerTradePopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit RequestPlayerTradePopup(Player* player,QWidget* parent = nullptr);


signals:
    void tradeSubmitted(const TradeOffer& offer);

private:
    TradeOffer m_offer;
    Player* m_player;
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
    void closePopup();
    void closeEvent(QCloseEvent* e);
};

#endif // TRADEPOPUP_H
