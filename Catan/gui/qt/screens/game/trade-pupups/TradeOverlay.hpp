//
// Created by matija on 1/30/26.
//

#ifndef CATAN_TRADEOVERLAY_HPP
#define CATAN_TRADEOVERLAY_HPP
#include <QVBoxLayout>
#include <model/Trade.h>
#include <player/Player.h>
#include <screens/game/toolbar/buttons/DiceWidget.h>
#include <screens/game/trade-pupups/popups/AcceptPlayerTradePopup.hpp>
#include <screens/game/trade-pupups/popups/RespondPlayerTradePopup.h>

class TradeOverlay : public QWidget {
    Q_OBJECT
public:
    TradeOverlay(std::vector<Player *> players, PlayerId currentId, std::unordered_map<TradeId,Trade> *trades, QWidget *parent);
    //void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const;
public slots:
    void refresh();
signals:
    void tradeRespondedPositive(PlayerId, TradeId);
    void tradeRespondedNegative(PlayerId, TradeId);
    void tradeAccepted(TradeId,PlayerId);
private slots:
    void onTradeAccepted(TradeId,PlayerId);
    void onTradeResponsePositive(TradeId,PlayerId);
    void onTradeResponseNegative(TradeId,PlayerId);

private:
    void rebuild();

    void updateGeometry();

    void restart();
    bool eventFilter(QObject *watched, QEvent *event) override;

    void addAcceptWidget(const Trade & trade);

    void addRespondWidget(const Trade & trade);

    void addWidgetForTrade(const Trade& trade);

    std::vector<Player*> m_players;

    std::unordered_map<TradeId,Trade>* m_trades;
    PlayerId m_currentId;

    QVBoxLayout* m_root = nullptr;
};

#endif //CATAN_TRADEOVERLAY_HPP