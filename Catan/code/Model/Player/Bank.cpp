//
// Created by w on 23/12/2025.
//

#include "../../../headers/Player/Bank.h"

void initializeStandardBank(){

		for (ResourceType type : ResourceCardTypes){
			addResource(type, m_standardNumOfResources);
		}

		for (DevType type : DevCardTypes){
			for(int i =0; i<m_numberOfStandardDevCardsByType[type]; i++){
				addDevCard(type);
			}
		}

		setLongestRoad(true);
		setLargestMilitary(true);
}