//
// Created by matija on 1/30/26.
//

#include "PlayerDetailedView.hpp"

#include <QGroupBox>
#include <QVBoxLayout>
#include <common/cards/QCardRow.h>

PlayerDetailedView::PlayerDetailedView(ResourceHolder *player, QWidget *parent) : FloatingPanel(parent), m_player(player) {
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    buildUi();
}

void PlayerDetailedView::refresh() {
    if (m_cards[0]) m_cards[0]->setSpec({CardKind::Resource, ResourceType::Wood,  DevCardType::None, m_player->getResources()[ResourceType::Wood]});
    if (m_cards[1]) m_cards[1]->setSpec({CardKind::Resource, ResourceType::Brick, DevCardType::None, m_player->getResources()[ResourceType::Brick]});
    if (m_cards[2]) m_cards[2]->setSpec({CardKind::Resource, ResourceType::Wool,  DevCardType::None, m_player->getResources()[ResourceType::Wool]});
    if (m_cards[3]) m_cards[3]->setSpec({CardKind::Resource, ResourceType::Wheat, DevCardType::None, m_player->getResources()[ResourceType::Wheat]});
    if (m_cards[4]) m_cards[4]->setSpec({CardKind::Resource, ResourceType::Ore,   DevCardType::None, m_player->getResources()[ResourceType::Ore]});
    if (m_cards[5]) m_cards[5]->setSpec({CardKind::Development, ResourceType::None, DevCardType::None,m_player->getNumOfDevCards()});
}

void PlayerDetailedView::buildUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(10,10,10,10);
    root->setSpacing(10);

    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto* titleBox = new QGroupBox(m_player->getName().data(), this);
    auto* paddingBox = new QVBoxLayout(titleBox);
    paddingBox->setContentsMargins(3, 5, 3, 3);
    paddingBox->setSpacing(0);

    auto* cardRow=new QCardRow(titleBox);
    m_cards= std::vector<QCard*>(6);

    m_cards[0]=cardRow->addCard({CardKind::Resource, ResourceType::Wood,  DevCardType::None, m_player->getResources()[ResourceType::Wood]});
    m_cards[1]=cardRow->addCard({CardKind::Resource, ResourceType::Brick, DevCardType::None, m_player->getResources()[ResourceType::Brick]});
    m_cards[2]=cardRow->addCard({CardKind::Resource, ResourceType::Wool,  DevCardType::None, m_player->getResources()[ResourceType::Wool]});
    m_cards[3]=cardRow->addCard({CardKind::Resource, ResourceType::Wheat, DevCardType::None, m_player->getResources()[ResourceType::Wheat]});
    m_cards[4]=cardRow->addCard({CardKind::Resource, ResourceType::Ore,   DevCardType::None, m_player->getResources()[ResourceType::Ore]});
    m_cards[5]=cardRow->addCard({CardKind::Development, ResourceType::None, DevCardType::None,m_player->getNumOfDevCards()});


    paddingBox->addWidget(cardRow);

    root->addWidget(titleBox, 1);
};

