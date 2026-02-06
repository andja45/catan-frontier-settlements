//
// Created by w on 23/12/2025.
//

#ifndef CATAN_RESOURCEHOLDER_H
#define CATAN_RESOURCEHOLDER_H
#include <map>
#include <string>
#include <vector>

#include "../types/TypeAliases.h"
#include <types/DevCardType.h>

enum class DevCardType;
enum class ResourceType;

class ResourceHolder {
protected:
	int m_numOfDevCards =0;
	int m_numOfResourceCards =0;
    std::map<ResourceType, int> m_resources;
    std::map<DevCardType, int> m_devCards;
    bool m_hasLongestRoad=false;
    bool m_hasLargestMilitary=false;

    std::string m_name;
public:
    ResourceHolder(const std::string &name="") : m_name(name)  {}

    bool hasResource(ResourceType resourceType, int amount) const {
        auto it = m_resources.find(resourceType);
        const int have = (it == m_resources.end()) ? 0 : it->second;
        return have >= amount;
    }
    bool hasResources(ResourcePack pack) const {
        for (auto r:pack) {
            if (!hasResource(r.first,r.second)) return false;
        }
        return true;
    }

    void removeResource(ResourceType resourceType, int amount) { m_resources[resourceType]-= amount; m_numOfResourceCards-=amount;}
    void addResource(ResourceType resourceType, int amount) { removeResource(resourceType,-amount);}
    void addDevCard(DevCardType devType) { m_devCards[devType]++; m_numOfDevCards++;}
    void removeDevCard(DevCardType devType) { m_devCards[devType]--; m_numOfDevCards--;}
    void removeResources(const ResourcePack& pack) { for (auto r:pack) removeResource(r.first,r.second);}
    void addResources(const ResourcePack& pack) { for (auto r:pack) addResource(r.first,r.second);}

    bool hasLongestRoad() const {return m_hasLongestRoad;}
    bool hasLargestMilitary() const {return m_hasLargestMilitary;}

    int getNumOfDevCards() const {return m_numOfDevCards;}
    int getNumOfDevCards(DevCardType d) const {return m_devCards.at(d);}
    int getNumOfResourceCards() const {return m_numOfResourceCards;};
    int getNumOfResourceCards(ResourceType r)  {return m_resources[r];}
    std::map<ResourceType, int> getResources() { return m_resources; }
    std::map<DevCardType, int> getDevCards() const {return m_devCards; }

    void setLongestRoad(bool longestRoad) {m_hasLongestRoad=longestRoad;}
    void setLargestArmy(bool largestMilitary) {m_hasLargestMilitary=largestMilitary;}

    bool hasDevCard(DevCardType dev_card) const {return m_devCards.at(dev_card)>0;}
    bool hasDevCards() const {return m_numOfDevCards>0;}

    std::string getName() { return m_name; }
    ResourcePack getResources() const {return m_resources;}

};


#endif //CATAN_RESOURCEHOLDER_H
