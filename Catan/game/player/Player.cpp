//
// Created by andja on 9.12.25..
//

#include "Player.h"

#include <random>


int Player::longestRoadLength() const { //TODO make graph from adjacent edges, (connect them through nodes) and do bfs

}


void Player::addRoad(Edge *edge) {
	m_roads.push_back(edge);
	m_numOfRoadsLeft--;
}

void Player::addSettlement(Node *node) {
	m_houses.push_back(node);
	m_numOfSettlementsLeft--;
}

void Player::addCity(Node *node) {
	m_numOfCitiesLeft--;
}

bool Player::has3for1Trade() const {
	for (const auto& node:m_houses) {
		if (node->is3for1Trade())
			return true;
	}
	return false;
}

bool Player::hasTrade(ResourceType resourceType) const {
	for (const auto& node:m_houses) {
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



