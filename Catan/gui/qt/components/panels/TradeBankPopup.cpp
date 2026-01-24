#include "TradeBankPopup.h"

TradeBankPopup::TradeBankPopup(Player* player, QWidget* parent)
    : FloatingPanel(parent)
{
    m_player = player;
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


    m_giveCards.clear();
    m_receiveCards.clear();
    m_giveRow = new QCardRow(this);
    m_receiveRow = new QCardRow(this);
    QCard* helperCard;
    auto helperBox = new QHBoxLayout();
    for(ResourceType resource : ResourceCardTypes){

        std::string labelname = "4:1";
        auto* rateLabel = new QLabel(QString::fromStdString(labelname));
        m_ratesLables[resource] = rateLabel;
        m_offer.give[resource] = 0;
        helperCard = m_giveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, 0}));
        helperBox->addWidget(rateLabel);
        connect(helperCard, &QCard::leftClicked, this, [this, helperCard, resource]() {
            if(m_playerResources[resource]>helperCard->spec().countBadge and m_tradeRates.at(helperCard->spec().resource)>helperCard->spec().countBadge){
                helperCard->incrementCount();
                m_offer.give[resource]++;
                if(m_offer.give[resource]==m_tradeRates.at(helperCard->spec().resource)){
                    m_validGive=true;
                }
                else{
                    m_validGive=false;
                }
            }
        });
        connect(helperCard, &QCard::rightClicked, this, [this, helperCard, resource]() {
            if(m_offer.give[resource]>0){
                helperCard->decrementCount();
                m_offer.give[resource]--;
                m_validGive=false;
            }
        });
        m_giveCards.push_back(helperCard);

        helperCard=m_receiveRow->addCard(CardSpec({CardKind::Resource, resource, DevCardType::None, 0}));
        connect(helperCard, &QCard::leftClicked, this, [this, helperCard,resource]() {
            if(helperCard->spec().countBadge<19){
                helperCard->incrementCount();
                m_offer.receive[resource]++;
            }
        });
        connect(helperCard, &QCard::rightClicked, this, [this, helperCard,resource]() {
            if(m_offer.receive[resource]>0){
                helperCard->decrementCount();
                m_offer.receive[resource]--;
            }

        });
        m_receiveCards.push_back(helperCard);
    }

    layout->addWidget(new QLabel("You give"));
    layout->addWidget(m_giveRow);

    layout->addLayout(helperBox);
    layout->addWidget(new QLabel("You receive"));
    layout->addWidget(m_receiveRow);

    m_sendButton = new QPushButton("Send Trade", this);
    m_sendButton->setEnabled(true);
    layout->addWidget(m_sendButton);

    connect(m_sendButton, &QPushButton::clicked, this, [this]() {
        emit tradeSubmitted(m_offer);
        close();
    });
}
void TradeBankPopup::updateTradeRates(){
    std::string labelname;
    for(ResourceType resource : ResourceCardTypes){

        if(m_player->has2for1Trade(resource))
        {
            m_tradeRates[resource] = 2;
            labelname = "2:1";
        }
        else if(m_player->has3for1Trade()){
            m_tradeRates[resource] = 3;
            labelname = "3:1";
        }
        else{
            m_tradeRates[resource] = 4;
            labelname = "4:1";
        }
        m_ratesLables[resource]->setText(QString::fromStdString(labelname));
    }
}
