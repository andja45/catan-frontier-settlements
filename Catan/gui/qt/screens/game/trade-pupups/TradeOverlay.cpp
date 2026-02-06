//
// Created by matija on 1/30/26.
//

#include "TradeOverlay.hpp"

#include <qevent.h>
#include <screens/game/trade-pupups/popups/AcceptPlayerTradePopup.hpp>

TradeOverlay::TradeOverlay(std::vector<Player *> players, PlayerId currentId, std::unordered_map<TradeId,Trade> *trades, QWidget *parent): QWidget(parent) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    // we listen to resize in order for floating widget to be size of parent
    parent->installEventFilter(this);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_root=new QVBoxLayout(this);
    m_root->setContentsMargins(10,10,10,10);
    m_root->setSpacing(5);


    m_currentId=currentId;
    m_players = players;
    m_trades = trades;
    rebuild();
}

QSize TradeOverlay::sizeHint() const {
    if (layout())
        return layout()->sizeHint();

    return QWidget::sizeHint();
}

bool TradeOverlay::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == parentWidget() && event->type() == QEvent::Resize) {
        updateOverlayGeometry();
    }
    return QWidget::eventFilter(obj, event);
}



void TradeOverlay::refresh() {
    rebuild();
}

void TradeOverlay::onTradeCancelled(TradeId id) {
    emit tradeAccepted(id,-1);
}

void TradeOverlay::onTradeAccepted(TradeId tid, PlayerId pid) {
    emit tradeAccepted(tid,pid);
}

void TradeOverlay::onTradeResponsePositive( PlayerId pid,TradeId tid) {
    emit tradeRespondedPositive(pid,tid);
}

void TradeOverlay::onTradeResponseNegative(PlayerId pid,TradeId tid) {
    emit tradeRespondedNegative(pid,tid);
}

void TradeOverlay::rebuild() {
    if (!m_trades) return;
    restart();
    for (auto [_,t]: *m_trades) {
        addWidgetForTrade(t);
    }
    m_root->addStretch(1);

    layout()->invalidate();
    layout()->activate();

    updateOverlayGeometry();
    update();
}
void TradeOverlay::updateOverlayGeometry()
{
    if (!parentWidget())
        return;

    int w = qBound(150, sizeHint().width(), 500);
    int h = parentWidget()->height();

    setGeometry(0, 0, w, h);
}

void TradeOverlay::restart() {
    while (m_root->count()) {
        auto *item = m_root->takeAt(0);
        delete item->widget();
        delete item;
    }
}

void TradeOverlay::addAcceptWidget(const Trade &trade) {
    auto* player=m_players[m_currentId];
    TradeOffer offer={trade.give(),trade.receive()};
    auto tradeId=trade.id();
    auto* widget=new AcceptPlayerTradePopup(player,offer,tradeId,this);
    auto respondedPlayerIds=trade.respondedPlayers();
    auto respondedPlayers=std::vector<Player*>(respondedPlayerIds.size());
    std::transform(respondedPlayerIds.begin(), respondedPlayerIds.end(),
           respondedPlayers.begin(),[this](PlayerId id) { return m_players[id]; });
    widget->setPlayers(respondedPlayers);
    connect(widget,&AcceptPlayerTradePopup::tradeAccepted,this,&TradeOverlay::onTradeAccepted);
    connect(widget,&AcceptPlayerTradePopup::tradeCancelled,this,&TradeOverlay::onTradeCancelled);
    m_root->addWidget(widget);
}

void TradeOverlay::addRespondWidget(const Trade &trade) {
    auto* player=m_players[m_currentId];
    TradeOffer offer={trade.receive(),trade.give()}; // we exchange give and receive for responding player
    auto tradeId=trade.id();
    auto* widget=new RespondPlayerTradePopup(player,offer,tradeId,this);
    connect(widget,&RespondPlayerTradePopup::tradeAccepted,this,&TradeOverlay::onTradeResponsePositive);
    connect(widget,&RespondPlayerTradePopup::tradeDeclined,this,&TradeOverlay::onTradeResponseNegative);
    m_root->addWidget(widget);
}

void TradeOverlay::addWidgetForTrade(const Trade &trade) {
    if (trade.requester()==m_currentId) { // if we requested we make accept popup
        addAcceptWidget(trade);
    }
    else { // player is responder
        addRespondWidget(trade);
    }
}

void TradeOverlay::showEvent(QShowEvent* e)
{
    QWidget::showEvent(e);
    updateOverlayGeometry();
}