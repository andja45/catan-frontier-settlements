//
// Created by w on 23/12/2025.
//

#ifndef CATAN_RESOURCEHOLDER_H
#define CATAN_RESOURCEHOLDER_H

#include "../Types/ResourceHolder.h"
class ResourceHolder {
private:
    std::string m_name ="";
	int m_numOfDevCards =0;
	int m_numOfResourceCards =0;
    std::map<ResourceType, int> m_resources;
    std::map<DevType, int> m_devCards;
    bool m_hasLongestRoad=false;
    bool m_hasLargestMilitary=false;

public:
    ResourceHolder(const std::string &name) : m_name(name) {}

    std::string getName() { return m_name; }

    void removeResource(ResourceType resourceType, int amount) { m_resources[resourceType]-= amount; m_numOfResourceCards--;}
    void addResource(ResourceType resourceType, int amount) { m_resources[resourceType]+= amount; m_numOfResourceCards++;}
    void addDevCard(DevType devType) { m_devCards[devType]++; m_numOfDevCards++;}
    void removeDevCard(DevType devType) { m_devCards[devType]--; m_numOfDevCards--;}

    bool hasLongestRoad() const {return m_hasLongestRoad;}
    bool hasLargestMilitary() const {return m_hasLargestMilitary;}

    int getNumOfDevCards() const {return m_numOfDevCards;}
    int getNumOfResourceCards() const {return m_numOfDevCards;};

    void setLongestRoad(bool longestRoad) {m_hasLongestRoad=longestRoad;}
    void setLargestMilitary(bool largestMilitary) {m_hasLargestMilitary=largestMilitary;}
};


#endif //CATAN_RESOURCEHOLDER_H