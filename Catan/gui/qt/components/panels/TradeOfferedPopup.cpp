#include "TradeOfferedPopup.h"

TradeOfferedPopup::TradeOfferedPopup(TradeOffer offer, QWidget* parent): FloatingPanel(parent)
{
    m_offer=offer;
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(R"(
        QFrame {
            background: white;
            border-radius: 12px;
            border: 1px solid rgba(0,0,0,40);
        }
    )");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);



    m_giveRow = new QCardRow(this);
    m_receiveRow = new QCardRow(this);
    QCard* helperCard;

    for(auto [resource,num]: m_offer.receive){
        helperCard = m_giveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, num}));

        m_giveCards.push_back(helperCard);
    }

    for(auto [resource,num]: m_offer.give){
        helperCard = m_receiveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, num}));

        m_receiveCards.push_back(helperCard);
    }

    layout->addWidget(new QLabel("You receive"));
    layout->addWidget(m_receiveRow);

    layout->addWidget(new QLabel("You give"));
    layout->addWidget(m_giveRow);

    m_acceptButton = new QPushButton("Accept", this);
    m_declineButton = new QPushButton("Decline", this);

    if(not isOfferAcceptable()){
        m_acceptButton->setEnabled(false);
    }

    auto* hbox = new QHBoxLayout(this);
    hbox->addWidget(m_acceptButton);
    layout->addLayout(hbox);

    connect(m_acceptButton, &QPushButton::clicked, this, [this]() {
        emit tradeAccepted(m_offer);
        close();
    });
    connect(m_declineButton, &QPushButton::clicked, this, [this]() {
        emit tradeDeclined(m_offer);
        close();
    });
}
bool TradeOfferedPopup::isOfferAcceptable() const{
    for(QCard* card : m_giveCards){
        if(card->spec().countBadge > m_playerResources.at(card->spec().resource)){
            return false;
        }
    }
    return true;
}
