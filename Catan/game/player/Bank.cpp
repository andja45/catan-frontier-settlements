//
// Created by w on 23/12/2025.
//

#include "player/Bank.h"
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
	for (auto r : ResourceType::)
	initializeBank(ResourcePack{},m_numberOfStandardDevCardsByType);
}

Bank::Bank(const ResourcePack &res, const DevPack &devCards) {
	initializeBank(res,devCards);
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
}
