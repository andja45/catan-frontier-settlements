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

void Bank::initializeStandardBank(){

		for (ResourceType type : ResourceCardTypes){
			addResource(type, m_standardNumOfResources);
		}

		for (DevType type : DevCardTypes){
			for(int i =0; i<m_numberOfStandardDevCardsByType.at(type); i++){
				addDevCard(type);
			}
		}

		setLongestRoad(true);
		setLargestMilitary(true);
}

DevType Bank::takeRandomDev() {
}
