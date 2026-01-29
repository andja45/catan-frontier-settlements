#ifndef TRADEOFFEREDPOPUP_H
#define TRADEOFFEREDPOPUP_H
#include"TradePopup.h"
#include "player/Player.h"
#include <QApplication>
#include <QScreen>
#include <algorithm>

class TradeOfferedPopup : public FloatingPanel
{
    Q_OBJECT
public:
    explicit TradeOfferedPopup(Player *m_player, TradeOffer offer, QWidget *parent);
    void stackAndShow();

signals:
    void tradeAccepted(const TradeOffer& offer);
    void tradeDeclined(const TradeOffer& offer);

private:
    std::map<ResourceType,int> m_playerResources ;
    TradeOffer m_offer;
    QCardRow* m_giveRow;

    std::vector<QCard*> m_accepterReceives;
    std::vector<QCard*> m_accepterGives;
    QCardRow* m_receiveRow;
    QPushButton* m_acceptButton;
    QPushButton* m_declineButton;

    bool isOfferAcceptable() const;

    void relayoutAll();

    std::map<ResourceType, int> selectedResources(std::vector<QCard*> cards) const;
    void closeEvent(QCloseEvent* e);

    bool eventFilter(QObject *obj, QEvent *event);

    void paintEvent(QPaintEvent*);
};

#endif // TRADEOFFEREDPOPUP_H
