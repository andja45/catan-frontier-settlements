//
// Created by andja on 9.12.25..
//

#include "Player.h"


void Player::addSettlement(Node* node){
	node->setOwner(m_playerId);
	node->setNodeType(NodeType::Settlement);
	addVictoryPoint();
	if(node->hasTrade()){
		if(node->is3for1Trade()) give3for1Trade();
		else give2for1Trade(node->getTradeResource());
	}
}
void Player::addCity(Node* node){
	node->upgradeToCity();
	addVictoryPoint();


}
void Player::addRoad(Edge* newroad){
	newroad->setRoad(m_playerId);
}







bool Player::hasTrade(ResourceType resourceType) const {
}

bool Player::hasLongestRoad() const {
}

ResourcePack Player::takeRandomResources(int amount) {
}

ResourceType Player::takeRandomResource() {
}

