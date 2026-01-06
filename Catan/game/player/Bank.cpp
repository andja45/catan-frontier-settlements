//
// Created by w on 23/12/2025.
//

#include "player/Bank.h"

#include <random>

#include "types/ResourceType.h"
#include "types/DevType.h"
enum class ResourceType;

const std::map<DevType,int> Bank::m_numberOfStandardDevCardsByType = {
	{DevType::VictoryPoint, 5},
	{DevType::Monopoly, 2},
	{DevType::RoadBuilding, 2},
	{DevType::Resources, 2},
	{DevType::Knight, 14}
};

Bank::Bank() {
	ResourcePack res;
	for (auto r : {ResourceType::Wood, ResourceType::Brick, ResourceType::Wool, ResourceType::Wheat, ResourceType::Ore}) {
		res.insert({r, m_standardNumOfResources});
	}
	initializeBank(ResourcePack{},m_numberOfStandardDevCardsByType);
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
	setLargestMilitary(true);
}

DevType Bank::takeRandomDev() {
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
