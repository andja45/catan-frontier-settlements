#include "TradePopup.h"

TradePopup::TradePopup(Player* p,QWidget* parent)
    : FloatingPanel(Qt::Popup, parent)
{
    m_playerResources= p->getResources();
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, false);

    setStyleSheet(R"(
        QPushButton {
            padding: 6px 10px;
            border-radius: 8px;
            background: rgba(0,0,0,0);
        }
        QPushButton:hover { background: rgba(0,0,0,18); }
        QPushButton:pressed { background: rgba(0,0,0,32); }
        QPushButton:disabled { color: rgba(0,0,0,120); }

        QLabel.section {
            font-weight: 600;
            color: rgba(0,0,0,160);
            padding-left: 4px;
        }
    )");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    m_giveCards.clear();
    m_receiveCards.clear();
    m_giveRow = new QCardRow(this);
    m_receiveRow = new QCardRow(this);
    QCard* helperCard;

    m_sendButton = new QPushButton("Send Trade", this);
    m_sendButton->setEnabled(false);
    layout->addWidget(m_sendButton);

    for(ResourceType resource : ResourceCardTypes){
        m_offer.give[resource] = 0;
        helperCard = m_giveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, 0}));

        connect(helperCard, &QCard::leftClicked, this, [this, helperCard, resource]() {
            if(m_playerResources[resource]>helperCard->spec().countBadge){
                helperCard->incrementCount();
                m_offer.give[resource]++;
                m_givesSelected++;
                m_sendButton->setEnabled(m_givesSelected>0 and m_receivesSelected>0);
            }
            else{
                helperCard->flicker(GameTheme::getFlickerColorByResource(resource));
            }
        });
        connect(helperCard, &QCard::rightClicked, this, [this, helperCard, resource]() {
            if(m_offer.give[resource]>0){
                helperCard->decrementCount();
                m_offer.give[resource]--;
                m_givesSelected--;
                m_sendButton->setEnabled(m_givesSelected>0 and m_receivesSelected>0);
            }

            else{
                helperCard->flicker(GameTheme::getFlickerColorByResource(resource));
            }
        });
        m_giveCards.push_back(helperCard);

        helperCard=m_receiveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, 0}));
        connect(helperCard, &QCard::leftClicked, this, [this, helperCard,resource]() {
            if(helperCard->spec().countBadge<19){
                helperCard->incrementCount();
                m_offer.receive[resource]++;
                m_receivesSelected++;
                m_sendButton->setEnabled(m_givesSelected>0 and m_receivesSelected>0);
            }

            else{
                helperCard->flicker(GameTheme::getFlickerColorByResource(resource));
            }
        });
        connect(helperCard, &QCard::rightClicked, this, [this, helperCard,resource]() {
            if(m_offer.receive[resource]>0){
                helperCard->decrementCount();
                m_offer.receive[resource]--;
                m_receivesSelected--;
                m_sendButton->setEnabled(m_givesSelected>0 and m_receivesSelected>0);
            }

            else{
                helperCard->flicker(GameTheme::getFlickerColorByResource(resource));
            }

        });
        m_receiveCards.push_back(helperCard);
    }

    auto* giveLabel = new QLabel("You give", this);
    giveLabel->setProperty("class", "section");

    auto* receiveLabel = new QLabel("You receive", this);
    receiveLabel->setProperty("class", "section");

    layout->addWidget(giveLabel);
    layout->addWidget(m_giveRow);
    layout->addWidget(receiveLabel);
    layout->addWidget(m_receiveRow);

    layout->addWidget(m_sendButton);

    connect(m_sendButton, &QPushButton::clicked, this, [this]() {
        m_offer.give = selectedResources(m_giveCards);
        m_offer.receive = selectedResources(m_receiveCards);
        emit tradeSubmitted(m_offer);
        close();
    });

    adjustSize();
}

bool TradePopup::isOfferValid() const{
    bool validGive = false;
    bool validReceive = false;
    for(auto [resource, num] : m_offer.give){
        if (num>0){
            validGive = true;
            break;
        }
    }
    for(auto [resource, num] : m_offer.receive){
        if (num>0){
            validReceive= true;
            break;
        }
    }
    return validGive and validReceive;
}//FIXME iz nekog razloga ne radi??

std::map<ResourceType, int> TradePopup::selectedResources(std::vector<QCard*> cards) const
{
    std::map<ResourceType, int> resources;
    for (QCard* card : cards){
        CardSpec spec = card->spec();
        resources[spec.resource]=spec.countBadge;
    }
    return resources;
}

void TradePopup::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const qreal radius = 12.0;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    // soft white translucent background
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 240));
    p.drawRoundedRect(rr, radius, radius);

    // subtle border
    p.setPen(QPen(QColor(0,0,0,40), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, radius, radius);
}

void TradePopup::resetState() {
    m_offer.give.clear();
    m_offer.receive.clear();
    m_givesSelected = 0;
    m_receivesSelected = 0;

    for (QCard* c : m_giveCards)
        c->setCount(0);

    for (QCard* c : m_receiveCards)
        c->setCount(0);

    m_sendButton->setEnabled(false);
}

void TradePopup::closeEvent(QCloseEvent* e) {
    resetState();
    QWidget::closeEvent(e);
}
