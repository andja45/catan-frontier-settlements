//
// Created by andja on 9.12.25..
//

#include "Player.h"
#include "../board/Board.h"

#include <random>
#include <player/Bank.h>

void Player::initStandardEmpty() {
	for (auto r : {ResourceType::Wood, ResourceType::Brick, ResourceType::Wool, ResourceType::Wheat, ResourceType::Ore}) {
		m_resources[r] = 0;
		addResource(r,15);
	}
	for (auto d : {DevCardType::VictoryPoint, DevCardType::Monopoly, DevCardType::RoadBuilding, DevCardType::YearOfPlenty, DevCardType::Knight}) {
		m_devCards[d] =0;
		addDevCard(d);
		addDevCard(d);
		addDevCard(d);
	}
}

void Player::addRoad(Edge *edge) {
	m_roads.push_back(edge);
	m_numOfRoadsLeft--;
}

void Player::addSettlement(Node *node) {
	m_buildings.push_back(node);
	m_numOfSettlementsLeft--;
}

void Player::addCity() {
	m_numOfCitiesLeft--;
	// buliding a city frees a settlement
	m_numOfSettlementsLeft++;
}

// if we decide to remove houses stored in player have trades cashed when build settlement is called
bool Player::has3for1Trade() const {
	for (const auto& node:m_buildings) {
		if (node==nullptr)
			continue;

		if (node->is3for1Trade())
			return true;
	}
	return false;
}

bool Player::hasTrade(ResourceType resourceType) const {
	for (const auto& node:m_buildings) {
		if (node==nullptr)
			continue;
		if (node->isTradeFor(resourceType))
			return true;
	}
	return false;
}

ResourcePack Player::takeRandomResources(int amount) {
	ResourcePack pack;
	for (int i = 0; i < amount; i++) {
		ResourceCardType rs = takeRandomResource();
		pack[rs]++;
	}
	return pack;
}

ResourceType Player::takeRandomResource() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<std::size_t> dist(0, m_resources.size() - 1);
	auto it = std::next(m_resources.begin(), dist(g));

	while (it->second == 0){
		it=std::next(m_resources.begin(), dist(g));
	}
	removeResource(it->first,1);
	return it->first;
}

int Player::minBankTradeRatio(ResourceType resource) const {
	if (has2for1Trade(resource))
		return 2;

	if (has3for1Trade())
		return 3;

	return 4;
}



