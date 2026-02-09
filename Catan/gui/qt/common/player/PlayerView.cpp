//
// Created by matija on 1/30/26.
//

#include "PlayerView.hpp"

#include <QGroupBox>
#include <QVBoxLayout>
#include <common/cards/QCardRow.h>
#include <common/player/PlayerDot.h>
#include <common/theme/GameTheme.h>

PlayerView::PlayerView(Player *player, QWidget *parent) : FloatingPanel(parent), m_player(player){
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    //hint?
    buildUi();
}

void PlayerView::setBestRoad(bool bestRoad) {
    m_bestRoad = bestRoad;
}

void PlayerView::setBestKnights(bool bestKnights) {
    m_bestKnights = bestKnights;
}

void PlayerView::setBestPoints(bool bestPoints) {
    m_bestPoints = bestPoints;
}

void PlayerView::refresh() {
    if (m_resourceCard) {
        m_resourceCard->setSpec({
            CardKind::Resource,
            ResourceType::None, DevCardType::None,
            m_player->getNumOfResourceCards()
        });
    }

    if (m_devCard) {
        m_devCard->setSpec({
            CardKind::Development,
            ResourceType::None, DevCardType::None,
            m_player->getNumOfDevCards()
        });
    }

    m_roadBadge->setNum(m_player->getRoadLength());
    m_roadBadge->setGolden(m_player->hasLongestRoad());
    m_knBadge->setNum(m_player->getKnightsUsed());
    m_knBadge->setGolden(m_player->hasLargestMilitary());
    //m_vpBadge->setNum(m_player->getVictoryPointsUsed());
    m_pointsDot->setText(std::to_string(m_player->getTotalPoints()).data());

}

void PlayerView::buildUi() {

    auto* playerHbox = new QHBoxLayout(this);
    playerHbox->setAlignment(Qt::AlignHCenter);
    playerHbox->setContentsMargins(10,10,10,10);
    playerHbox->setSpacing(10);

    m_pointsDot = new TextInDot(GameTheme::getPlayerColor(m_player->getPlayerId()),Qt::white,std::to_string(m_player->getTotalPoints()).data() ,this);
    playerHbox->addWidget(m_pointsDot, 0, Qt::AlignVCenter);
    playerHbox->addWidget(new QLabel(QString::fromStdString(m_player->getName())), 0, Qt::AlignVCenter);
    playerHbox->addStretch(3);

    //auto* cardRow=new QCardRow();

    m_resourceCard = new QCard();

    m_resourceCard->setSpec({CardKind::Resource, ResourceType::None, DevCardType::None,m_player->getNumOfResourceCards()});
    playerHbox->addWidget(m_resourceCard);

    m_devCard = new QCard();
    m_devCard->setSpec({CardKind::Development, ResourceType::None, DevCardType::None,m_player->getNumOfDevCards()});
    playerHbox->addWidget(m_devCard);

    playerHbox->addStretch(2);

    auto* knLabel = new QLabel("Knights:", this);
    auto* rdLabel   = new QLabel("Roads:", this);
    //auto* vpLabel   = new QLabel("Points:", this);

    m_knBadge=new QCountBadge(0,this);
    m_roadBadge=new QCountBadge(0,this);
    //m_vpBadge=new QCountBadge(0,this);


    auto* badgeLayout1 = new QVBoxLayout();
    badgeLayout1->setContentsMargins(0,0,0,0);
    badgeLayout1->setSpacing(2);
    badgeLayout1->addWidget(knLabel, 0, Qt::AlignHCenter);
    badgeLayout1->addWidget(m_knBadge, 0, Qt::AlignHCenter);
    playerHbox->addLayout(badgeLayout1);
    playerHbox->setAlignment(badgeLayout1, Qt::AlignVCenter);


    auto* badgeLayout2 = new QVBoxLayout();
    badgeLayout2->setContentsMargins(0,0,0,0);
    badgeLayout2->setSpacing(2);
    badgeLayout2->addWidget(rdLabel, 0, Qt::AlignHCenter);
    badgeLayout2->addWidget(m_roadBadge, 0, Qt::AlignHCenter);
    playerHbox->addLayout(badgeLayout2);
    playerHbox->setAlignment(badgeLayout2, Qt::AlignVCenter);

    // auto* badgeLayout3 = new QVBoxLayout();
    // playerHbox->addLayout(badgeLayout3);
    // badgeLayout3->addWidget(vpLabel);
    // badgeLayout3->addWidget(m_vpBadge);

   }



