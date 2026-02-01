//
// Created by w on 23/12/2025.
//

#include "player/Bank.h"

#include <random>

#include "types/ResourceType.h"
#include "types/DevCardType.h"
enum class ResourceType;

const std::map<DevCardType,int> Bank::m_numberOfStandardDevCardsByType = {
	{DevCardType::VictoryPoint, 5},
	{DevCardType::Monopoly, 2},
	{DevCardType::RoadBuilding, 2},
	{DevCardType::YearOfPlenty, 2},
	{DevCardType::Knight, 14}
};

Bank::Bank() {
	ResourcePack res;
	for (auto r : {ResourceType::Wood, ResourceType::Brick, ResourceType::Wool, ResourceType::Wheat, ResourceType::Ore}) {
		res.insert({r, m_standardNumOfResources});
	}
	initializeBank(res,m_numberOfStandardDevCardsByType);
}

Bank::Bank(const ResourcePack &resources, const DevPack &devCards) {
	initializeBank(resources,devCards);
}

void Bank::initializeBank(const ResourcePack &res, const DevPack &devCards) {

	m_name="Bank";
	for(auto [fst, snd]:res){
		addResource(fst,snd);
	}
	for(auto [fst, snd]:devCards){
		for(int i =0; i<snd; i++){
			addDevCard(fst);
		}
	}

	setLongestRoad(true);
	setLargestArmy(true);
}

DevCardType Bank::takeRandomDev() {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<std::size_t> dist(0, m_devCards.size() - 1);
	auto it = std::next(m_devCards.begin(), dist(g));

	while (it->second == 0){
		it=std::next(m_devCards.begin(), dist(g));
	}
		removeDevCard(it->first);
	return it->first;
}
