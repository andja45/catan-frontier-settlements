#include "RespondPlayerTradePopup.h"

#include "../../../../common/audio/AudioManager.h"
#include "player/Player.h"

RespondPlayerTradePopup::RespondPlayerTradePopup(Player *player, TradeOffer offer, TradeId tradeId , QWidget *parent) : FloatingPanel(parent), m_tradeId(tradeId)
{

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_playerResources=player->getResources();
    m_playerId=player->getPlayerId();
    m_offer=offer;
//    setAttribute(Qt::WA_TranslucentBackground, true);
  //  setAttribute(Qt::WA_StyledBackground, false);

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

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 12, 8, 12);
    layout->setSpacing(5);

    m_giveRow = new QCardRow(this);
    m_receiveRow = new QCardRow(this);
    QCard* helperCard;
    for(auto [resource,num]: m_offer.receive){
        if (num<=0) continue;
        helperCard = m_receiveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, num}));
        m_accepterGives.push_back(helperCard);
    }

    for(auto [resource,num]: m_offer.give){ // acc receives what requester gives
        if (num<=0) continue;
        helperCard = m_giveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, num}));
        m_accepterReceives.push_back(helperCard);
    }
    m_receiveRow->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_giveRow->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    auto* receiveLabel = new QLabel("You receive", this);
    receiveLabel->setProperty("class", "section");

    auto* giveLabel = new QLabel("You give", this);
    giveLabel->setProperty("class", "section");

    layout->addWidget(receiveLabel);
    //m_receiveRow->setMaximumHeight(80);
    m_receiveRow->setFixedHeight(50);
    layout->addWidget(m_receiveRow,0,Qt::AlignCenter);
    m_giveRow->setFixedHeight(50);
    layout->addWidget(giveLabel);
    layout->addWidget(m_giveRow,0,Qt::AlignCenter);

    auto* buttons = new QHBoxLayout();
    buttons->addStretch(1);

    m_acceptButton = new QPushButton(QStringLiteral("✔"), this);
    m_declineButton = new QPushButton(QStringLiteral("✖"), this);

    m_acceptButton->setFixedSize(32, 24);
    m_declineButton->setFixedSize(32, 24);

    m_declineButton->setCheckable(true);
    m_declineButton->setAutoExclusive(false);

    m_acceptButton->setCheckable(true);
    m_acceptButton->setEnabled(isOfferAcceptable());

    buttons->addWidget(m_declineButton);
    buttons->addWidget(m_acceptButton);

    layout->addLayout(buttons);

    connect(m_acceptButton, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();

        m_declineButton->setChecked(false);
        m_acceptButton->setChecked(true);
        emit tradeAccepted(m_playerId,m_tradeId);
    });

    connect(m_declineButton, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();

        m_declineButton->setChecked(true);
        m_acceptButton->setChecked(false);

        emit tradeDeclined(m_playerId,m_tradeId);
    });
}

bool RespondPlayerTradePopup::isOfferAcceptable() const{
    for (QCard* card : m_accepterReceives) {
        const auto rt = card->spec().resource;
        const int need = card->spec().countBadge;
        const int have = m_playerResources.count(rt) ? m_playerResources.at(rt) : 0;

        if (need > have)
            return false;
    }
    return true;
}


void RespondPlayerTradePopup::paintEvent(QPaintEvent*) {
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