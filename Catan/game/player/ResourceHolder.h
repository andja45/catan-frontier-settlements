//
// Created by w on 23/12/2025.
//

#ifndef CATAN_RESOURCEHOLDER_H
#define CATAN_RESOURCEHOLDER_H
#include <map>
#include <string>

#include "../types/TypeAliases.h"

enum class DevType;
enum class ResourceType;

class ResourceHolder {
protected:
	int m_numOfDevCards =0;
	int m_numOfResourceCards =0;
    std::map<ResourceType, int> m_resources;
    std::map<DevType, int> m_devCards;
    bool m_hasLongestRoad=false;
    bool m_hasLargestMilitary=false;

    std::string m_name;
public:
    ResourceHolder(const std::string &name="") : m_name(name)  {}

    bool hasResource(ResourceType resourceType, int amount) const { return m_resources.at(resourceType)>=amount; }
    bool hasResources(ResourcePack pack) const {
        for (auto r:pack) {
            if (!hasResource(r.first,r.second)) return false;
        }
        return true;
    }

    void removeResource(ResourceType resourceType, int amount) { m_resources[resourceType]-= amount; m_numOfResourceCards-=amount;}
    void addResource(ResourceType resourceType, int amount) { removeResource(resourceType,-amount);}
    void addDevCard(DevType devType) { m_devCards[devType]++; m_numOfDevCards++;}
    void removeDevCard(DevType devType) { m_devCards[devType]--; m_numOfDevCards--;}

    bool hasLongestRoad() const {return m_hasLongestRoad;}
    bool hasLargestMilitary() const {return m_hasLargestMilitary;}

    int getNumOfDevCards() const {return m_numOfDevCards;}
    int getNumOfDevCards(DevType d) const {return m_devCards.at(d);}
    int getNumOfResourceCards() const {return m_numOfDevCards;};
    int getNumOfResourceCards(ResourceType r) const {return m_resources.at(r);}

    void setLongestRoad(bool longestRoad) {m_hasLongestRoad=longestRoad;}
    void setLargestMilitary(bool largestMilitary) {m_hasLargestMilitary=largestMilitary;}
};


#endif //CATAN_RESOURCEHOLDER_H