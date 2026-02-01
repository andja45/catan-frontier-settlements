//
// Created by matija on 1/31/26.
//

#include "AcceptPlayerTradePopup.hpp"

#include <common/panels/FloatingButton.hpp>
#include <common/player/PlayerDot.h>

AcceptPlayerTradePopup::AcceptPlayerTradePopup(Player *player, TradeOffer offer, TradeId tradeId, QWidget *parent): FloatingPanel(parent) {


    m_playerId=player->getPlayerId();
    m_tradeId=tradeId;
    m_offer=offer;

    setStyleSheet(R"(
    QPushButton {
        padding: 6px 10px;
        border-radius: 8px;
        background: rgba(0,0,0,0);
    }
    QPushButton:hover {
        background: rgba(0,0,0,18);
    }
    QPushButton:pressed {
        background: rgba(0,0,0,32);
    }
    QPushButton:disabled {
        color: rgba(0,0,0,120);
    }
    QPushButton:checked {
        font-weight: 700;
        color: rgb(60,130,255);
    }
    QLabel.section {
        font-weight: 600;
        color: rgba(0,0,0,160);
        padding-left: 4px;
    }
)");

    QCardRow* giveRow;
    QCardRow* receiveRow;

    std::vector<QCard*> acceptorReceives;
    std::vector<QCard*> acceptorGives;

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 12, 8, 12);
    layout->setSpacing(5);

    giveRow = new QCardRow(this);
    receiveRow = new QCardRow(this);


    for(auto [resource,num]: m_offer.give){
        giveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, num}));
    }
    for(auto [resource,num]: m_offer.receive){
         receiveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, num}));
    }

    auto* receiveLabel = new QLabel("You receive", this);
    receiveLabel->setProperty("class", "section");

    auto* giveLabel = new QLabel("You give", this);
    giveLabel->setProperty("class", "section");

    receiveRow->setFixedHeight(50);
    giveRow->setFixedHeight(50);

    receiveRow->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    giveRow->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    layout->addWidget(giveLabel);
    layout->addWidget(giveRow,0,Qt::AlignCenter);

    layout->addWidget(receiveLabel);
    layout->addWidget(receiveRow,0,Qt::AlignCenter);

    m_buttonsBox = new QHBoxLayout();
    m_buttonsBox->addStretch(1);
    layout->addLayout(m_buttonsBox);
}

void AcceptPlayerTradePopup::setPlayers(std::vector<Player *> players) {
    m_players = players;
    rebuild();
}

void AcceptPlayerTradePopup::rebuild() {

    for (auto p:m_players) {
        auto* btn=new FloatingButton(this);
        auto* playerLabel=TextInDot::fromPlayer(p);
        btn->setMargin(0);
        btn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        btn->addWidget(playerLabel);
        btn->setStyleSheet("padding: 0px; border: none;");

        m_buttonsBox->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this, id=p->getPlayerId()]() {
            emit tradeAccepted(m_tradeId,id);
            close();
        });
    }

    layout()->invalidate();
    layout()->activate();
    setMinimumSize(sizeHint());
    adjustSize();
}

void AcceptPlayerTradePopup::paintEvent(QPaintEvent *paint_event){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const qreal radius = 12.0;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 240));
    p.drawRoundedRect(rr, radius, radius);

    p.setPen(QPen(QColor(0,0,0,40), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, radius, radius);
}
