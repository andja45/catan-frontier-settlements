//
// Created by w on 23/12/2025.
//

#ifndef CATAN_BANK_H
#define CATAN_BANK_H
#include "../Types/DevType.hpp"
#include "ResourceHolder.h"
#include "../../headers/Types/TypeAliases.hpp"

class Bank: public ResourceHolder{
private:
	static const int m_standardNumOfResources = 20;
	static const std::map<ResourceType,int*> m_numberOfStandardDevCardsByType = {
		{DevType::VictoryPoint, 5},
		{DevType::Monopoly, 2},
		{DevType::RoadBuilding, 2},
		{DevType::Resources, 2},
        {DevType::Knight, 14}
	};

public:
    Bank(const std::string &name) : ResourceHolder(name) {}

	void initializeStandardBank(){}
};


#endif //CATAN_BANK_H