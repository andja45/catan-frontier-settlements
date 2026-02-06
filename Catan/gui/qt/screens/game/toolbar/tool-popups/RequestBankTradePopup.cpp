#include "screens/game/toolbar/tool-popups/RequestBankTradePopup.h"

#include <qevent.h>

RequestBankTradePopup::RequestBankTradePopup(Player* player, QWidget* parent)
    : FloatingPanel(parent)
{
    m_player = player;
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
    auto rateRow = new QHBoxLayout();

    m_sendButton = new QPushButton("Send Trade", this);
    m_sendButton->setEnabled(false);
    layout->addWidget(m_sendButton);

    for (ResourceType resource : ResourceCardTypes) {
        std::string labelname = "4:1";
        auto* rateLabel = new QLabel(QString::fromStdString(labelname));

        m_ratesLables[resource] = rateLabel;
        m_offer.give[resource] = 0;

        auto* cardWithLabel = new QVBoxLayout();
        cardWithLabel->setSpacing(2);
        cardWithLabel->setAlignment(Qt::AlignHCenter);

        helperCard = m_giveRow->addCard(
            CardSpec({ CardKind::Resource, resource, DevCardType::None, 0 })
        );

        cardWithLabel->addWidget(helperCard, 0, Qt::AlignHCenter);
        cardWithLabel->addWidget(rateLabel, 0, Qt::AlignHCenter);

        rateRow->addLayout(cardWithLabel);

        connect(helperCard, &QCard::leftClicked, this,
            [this, helperCard, resource]() {
                if (m_player->getResources()[resource] >=
                        m_tradeRates.at(helperCard->spec().resource) &&
                    m_selectedGive == ResourceType::None) {

                    helperCard->setCount(
                        m_tradeRates.at(helperCard->spec().resource)
                    );
                    m_selectedGive = resource;
                    m_sendButton->setEnabled(
                        m_selectedReceive != ResourceType::None
                    );
                } else {
                    helperCard->flicker(
                        GameTheme::getFlickerColorByResource(resource)
                    );
                }
            }
        );

        connect(helperCard, &QCard::rightClicked, this,
            [this, helperCard, resource]() {
                if (m_selectedGive == resource) {
                    helperCard->setCount(0);
                    m_selectedGive = ResourceType::None;
                    m_sendButton->setEnabled(false);
                } else {
                    helperCard->flicker(
                        GameTheme::getFlickerColorByResource(resource)
                    );
                }
            }
        );

        m_giveCards.push_back(helperCard);

        helperCard = m_receiveRow->addCard(
            CardSpec({ CardKind::Resource, resource, DevCardType::None, 0 })
        );

        connect(helperCard, &QCard::leftClicked, this,
            [this, helperCard, resource]() {
                if (m_selectedReceive == ResourceType::None) {
                    helperCard->setCount(1);
                    m_selectedReceive = resource;
                    m_sendButton->setEnabled(
                        m_selectedGive != ResourceType::None
                    );
                } else {
                    helperCard->flicker(
                        GameTheme::getFlickerColorByResource(resource)
                    );
                }
            }
        );

        connect(helperCard, &QCard::rightClicked, this,
            [this, helperCard, resource]() {
                if (m_selectedReceive == resource) {
                    helperCard->setCount(0);
                    m_selectedReceive = ResourceType::None;
                    m_sendButton->setEnabled(false);
                } else {
                    helperCard->flicker(
                        GameTheme::getFlickerColorByResource(resource)
                    );
                }
            }
        );

        m_receiveCards.push_back(helperCard);
    }

    updateTradeRates();

    auto* giveLabel = new QLabel("You give", this);
    giveLabel->setProperty("class", "section");

    auto* receiveLabel = new QLabel("You receive", this);
    receiveLabel->setProperty("class", "section");

    layout->addWidget(giveLabel);
    layout->addLayout(rateRow);
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

void RequestBankTradePopup::updateTradeRates(){
    std::string labelname;
    for (ResourceType resource : ResourceCardTypes) {
        int rate = m_player->minBankTradeRatio(resource);

        m_tradeRates[resource] = rate;
        m_ratesLables[resource]->setText(
            QString("%1:1").arg(rate)
        );
    }
}

std::map<ResourceType, int> RequestBankTradePopup::selectedResources(std::vector<QCard*> cards) const
{
    std::map<ResourceType, int> resources;
    for (QCard* card : cards){
        CardSpec spec = card->spec();
        if (spec.countBadge>0)
            resources[spec.resource]=spec.countBadge;
    }
    return resources;
}

void RequestBankTradePopup::paintEvent(QPaintEvent*)
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

void RequestBankTradePopup::resetState() {
    m_selectedGive = ResourceType::None;
    m_selectedReceive = ResourceType::None;
    m_offer.give.clear();
    m_offer.receive.clear();

    for (QCard* card : m_giveCards)
        card->setCount(0);

    for (QCard* card : m_receiveCards)
        card->setCount(0);

    m_sendButton->setEnabled(false);
}

void RequestBankTradePopup::closeEvent(QCloseEvent *e) {
    e->ignore();
    closePopup();
}

void RequestBankTradePopup::closePopup(){
    resetState();
    hide();
}

void RequestBankTradePopup::showEvent(QShowEvent* event)
{
    updateTradeRates();
}